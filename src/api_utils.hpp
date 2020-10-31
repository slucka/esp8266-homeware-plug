// #define CMD_SPEED       "sp"
// #define CMD_TURN        "tr"
// #define CMD_FORWARD     "f"
// #define CMD_TURN_LEFT   "l"
// #define CMD_TURN_RIGHT  "r"
// #define CMD_BACKWARD    "b"
// #define CMD_CENTER      "ce"
// #define CMD_STOP        "x"

// String command = "";
// int valueSpeed = 0;
// int valueDir = 0;
// bool hayNuevaData = false;

// String getCommand(){
//   hayNuevaData = false;
//   return command;
// }

// int getValueSpeed(){
//   return valueSpeed;
// }

// int getValueDir(){
//   return valueDir;
// }

// bool getDataIsAvailable(){
//   return hayNuevaData;
// }

// String parseDataToCmd(String id, String event, String valspeed, String valdir){

//   String cmd = "";

//   valueSpeed = valspeed.toInt();
//   valueDir = valdir.toInt();

//   if (event == "down"){
//     if (id == "rg01")
//     {
//       cmd = CMD_SPEED;
//     }
//     if (id == "rg02")
//     {
//       cmd = CMD_TURN;
//     }
//     if (id == "bt01")
//     {
//       cmd = CMD_FORWARD;
//     }
//     if (id == "bt02")
//     {
//       cmd = CMD_TURN_LEFT;
//     }
//     if (id == "bt03")
//     {
//       cmd = CMD_TURN_RIGHT;
//     }
//     if (id == "bt04")
//     {
//       cmd = CMD_BACKWARD;
//     }
//   }else if (event == "up"){
//     if (id == "rg01")
//     {
//       cmd = CMD_SPEED;
//     }
//     if (id == "rg02")
//     {
//       cmd = CMD_TURN;
//     }
//     if (id == "bt01")
//     {
//       cmd = CMD_STOP;
//     }
//     if (id == "bt02")
//     {
//       cmd = CMD_CENTER;
//     }
//     if (id == "bt03")
//     {
//       cmd = CMD_CENTER;
//     }
//     if (id == "bt04")
//     {
//       cmd = CMD_STOP;
//     }
//   }
//   return cmd;
// }

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
  // StaticJsonDocument<300> jconfig;
  // jconfig["deviceid"] = config_deviceid;
  // jconfig["ssid"] = config_ssid;
  // jconfig["password"] = config_password;
  // jconfig["mqtt_server"] = config_mqtt_server;
  // serializeJson(jconfig, json);

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