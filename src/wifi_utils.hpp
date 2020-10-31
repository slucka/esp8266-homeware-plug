const byte DNS_PORT = 53;
DNSServer dnsServer;

// void ConnectWiFi_STA(bool useStaticIP = false)
// {
//    Serial.println("");
//    WiFi.mode(WIFI_STA);
//    WiFi.begin(sta_ssid, sta_password);

//    if(useStaticIP)
//       WiFi.config(sta_ip, sta_gateway, sta_subnet);

//    while (WiFi.status() != WL_CONNECTED)
//    {
//      delay(100);
//      Serial.print('.');
//    }

//    Serial.println("");
//    Serial.print("Iniciado STA:\t");
//    Serial.println(sta_ssid);
//    Serial.print("IP address:\t");
//    Serial.println(WiFi.localIP());
// }

void ConnectWiFi_STA(const char *sta_ssid, const char *sta_password)
{
  // Serial.println("");
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(sta_ssid, sta_password);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(100);
  //   Serial.print('.');
  // }

  // Serial.println("");
  // Serial.print("Iniciado STA:\t");
  // Serial.println(sta_ssid);
  // Serial.print("IP address:\t");
  // Serial.println(WiFi.localIP());

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println("ConnectWiFi_STA");

  Serial.print("Connecting to ");
  Serial.println(sta_ssid);

  WiFi.begin(sta_ssid, sta_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(2000);
}

void ConnectWiFi_AP(bool useStaticIP = false)
{
  Serial.println("");
  Serial.println("ConnectWiFi_AP");

  WiFi.mode(WIFI_AP);

  // while (!WiFi.softAP(ap_ssid, ap_password))
  // {
  //   Serial.print(".");
  //   delay(100);
  // }

  String ssid = String(ap_ssid);
  ssid += "_";
  ssid += WiFi.macAddress();
  ssid.replace(":", "_");

  Serial.print("ssid:");
  Serial.println(ssid);
  while (!WiFi.softAP(ssid.c_str(), ap_password))
  {
    Serial.print(".");
    delay(100);
  }

  if (useStaticIP)
    WiFi.softAPConfig(ap_ip, ap_gateway, ap_subnet);


  // if DNSServer is started with "*" for domain name, it will reply with
  // provided IP to all DNS request
  dnsServer.start(DNS_PORT, "*", ap_gateway);    

  Serial.println("");
  Serial.print("Iniciado AP:\t");
  Serial.println(ap_ssid);
  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());
}
