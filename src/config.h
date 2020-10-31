
const char *ap_ssid     = "smartoblap";
const char *ap_password = "smartoblap";
const char *ap_hostname = "smartoblap";
//IPAddress ap_ip(192, 168, 1, 200);
IPAddress ap_ip(192, 168, 1, 1);
IPAddress ap_gateway(192, 168, 1, 1);
IPAddress ap_subnet(255, 255, 255, 0);

// const char *sta_ssid     = "";
// const char *sta_password = "";
// const char *sta_hostname = "";
// IPAddress sta_ip(192, 168, 1, 200);
// IPAddress sta_gateway(192, 168, 1, 1);
// IPAddress sta_subnet(255, 255, 255, 0);

//const char *ssid = "slucka_wifi_2";
//const char *password = "ivan2001";
//const char *mqtt_server = "192.168.1.107";

#define FILE_CONFIG "config.json"

String config_deviceid = String("");
String config_ssid = String("");
String config_password = String("");
String config_mqtt_server = String("");



// #define CMD_SPEED       "sp"
// #define CMD_TURN        "tr"
// #define CMD_FORWARD     "f"
// #define CMD_TURN_LEFT   "l"
// #define CMD_TURN_RIGHT  "r"
// #define CMD_BACKWARD    "b"
// #define CMD_CENTER      "ce"
// #define CMD_STOP        "x"
