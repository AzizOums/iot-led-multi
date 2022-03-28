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
  WiFi.softAP(SSID, PWD);
  delay(100);
  WiFi.softAPConfig(IP, IP, subnet);
  delay(100);
  server.on("/", sendPage);
  server.on("/init", initSettings);
  server.begin();
}