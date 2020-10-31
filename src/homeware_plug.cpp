#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "LittleFS.h"
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>

#include "config.h"
#include "api_utils.hpp"
#include "server_utils.hpp"
#include "wifi_utils.hpp"

//Objects
WiFiClient espClient;
PubSubClient client(espClient);

//General

#define PIN_RELE (D7)


String json_s = "";
char json_c[200];
char deviceMode[30];

bool onOffValue = false;

long int tempUpdateTimeStamp = 0;

char msg[300];


void callback(char *topic, byte *payload, unsigned int length)
{

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  for (unsigned int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    json_c[i] = payload[i];
  }

  Serial.println();
  Serial.println(json_c);

  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, json_c);

  if (error)
  {

    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
  }
  else
  {

    onOffValue = doc["on"];

    if (onOffValue == true)
    {
      digitalWrite(PIN_RELE, HIGH);
    }
    else
    {
      digitalWrite(PIN_RELE, LOW);
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {

    Serial.print("Attempting MQTT connection...");

    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      delay(2000);

      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("hello", "hello world");
      // ... and resubscribe
      strcpy(msg, "device/");
      strcat(msg, config_deviceid.c_str());
      client.subscribe(msg);
      Serial.println(msg);
    }
    else
    {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Setup Wemos");

  Serial.println(F("Init LittleFS"));
  if (LittleFS.begin())
  {
    Serial.println(F("  LittleFS done!"));

    if (LittleFS.exists(FILE_CONFIG))
    {
      File configFile = LittleFS.open(F("/config.json"), "r");
      json_s = configFile.readString();
      configFile.close();

      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, json_s);
      if (!error)
      {
        config_deviceid = doc["deviceid"].as<String>();
        config_ssid = doc["ssid"].as<String>();
        config_password = doc["password"].as<String>();
        config_mqtt_server = doc["mqtt_server"].as<String>();

        Serial.print("config_deviceid:'");
        Serial.print(config_deviceid);
        Serial.println("'");
      }
    }

  }
  else
    Serial.println(F("  LittleFS fail!"));

  if (!config_deviceid.equals(""))
  {

    ConnectWiFi_STA(config_ssid.c_str(), config_password.c_str());

    Serial.println("Init PubSubClient");

    client.setServer(config_mqtt_server.c_str(), 1883);
    client.setCallback(callback);

    pinMode(PIN_RELE, OUTPUT);
    digitalWrite(PIN_RELE, LOW);
  }
  else
  {
    ConnectWiFi_AP(true);
  }

  Serial.println("Init WebServer");
  InitServer();

  Serial.println("Init WebSockets");
  InitWebSockets();

  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Setup Fin");
}

void loop()
{

  if (!config_deviceid.equals(""))
  {
    if (!client.connected())
    {
      digitalWrite(LED_BUILTIN, HIGH);

      reconnect();

      if (client.connected())
      {
        char onOffValueBuf[5];
        sprintf(onOffValueBuf, "%s", onOffValue ? "true" : "false");
        strcpy(msg, "{\"id\":\"");
        strcat(msg, config_deviceid.c_str());
        strcat(msg, "\",\"param\":\"on\",\"value\":");
        strcat(msg, onOffValueBuf);
        strcat(msg, ",\"intent\":\"request\"}");
        client.publish("device/control", msg);
        Serial.println(msg);
      }
    }

    client.loop();

    digitalWrite(LED_BUILTIN, HIGH); // Arduino: turn the LED on (HIGH)
    delay(900);
    digitalWrite(LED_BUILTIN, LOW); // Arduino: turn the LED off (LOW)
    delay(100);
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH); // Arduino: turn the LED on (HIGH)
    delay(100);
    digitalWrite(LED_BUILTIN, LOW); // Arduino: turn the LED off (LOW)
    delay(100);
  }
}