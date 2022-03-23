#include "initialise.h"
#include "mqttCtrl.h"

void stopServer()
{
  WiFi.softAPdisconnect(true);
  server.stop();
}

void initSettings()
{
  String s = server.arg("ssid");
  String p = server.arg("pwd");
  if (!s.isEmpty() && !p.isEmpty())
    setWifiParams(s, p);
  if (initialised)
  {
    stopServer();
    mqttSetup();
  }
}

void sendPage()
{
  server.send(200, "text/html", homePage);
}

void setupWifi()
{
  Serial.println("setting up wifi");
  WiFi.softAP(SSID, PWD);
  delay(100);
  WiFi.softAPConfig(IP, IP, subnet);
  delay(100);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(ip);

  server.on("/", sendPage);
  server.on("/init", initSettings);
  server.begin();
}