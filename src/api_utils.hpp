
void onCommandSet(AsyncWebSocketClient *client, String request)
{
  Serial.println("onCommandSet");

  StaticJsonDocument<200> jdata;
  DeserializationError error = deserializeJson(jdata, request);
  if (error)
  {
    Serial.println("Error!");
    return;
  }

  String id = jdata["id"];
  config_deviceid = jdata["deviceid"].as<String>();
  config_ssid = jdata["ssid"].as<String>();
  config_password = jdata["password"].as<String>();
  config_mqtt_server = jdata["mqtt_server"].as<String>();

  jdata.remove("id");
  String json;

  Serial.println("onCommandSet Save Canfig");
  serializeJson(jdata, json);
  Serial.println(json);
  File configFile = LittleFS.open(F("/config.json"), "w");
  configFile.write(json.c_str());
  configFile.close();
  Serial.println("onCommandSet End");

}

void onCommandGet(AsyncWebSocketClient *client, String request)
{
  String response;
  StaticJsonDocument<300> jdata;
  jdata["id"] = random(0, 10);
  jdata["deviceid"] = config_deviceid;
  jdata["ssid"] = config_ssid;
  jdata["password"] = config_password;
  jdata["mqtt_server"] = config_mqtt_server;
  serializeJson(jdata, response);

  client->text(response);
}

void onCommandReset(AsyncWebSocketClient *client, String request)
{
  Serial.println("Resetting ESP");
  ESP.restart(); //ESP.reset();
}