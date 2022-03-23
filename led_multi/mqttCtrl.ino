#include "mqttCtrl.h"
#include "planif.h"
#include "led.h"
#include "saveData.h"

void onConnectionEstablished()
{
  mqttControle();
}

void setWifiParams(String ssid, String pwd)
{
  wifiSsid = ssid;
  wifiPwd = pwd;
  initialised = true;
  saveMqttInfo();
}

void mqttSetup()
{
  Serial.println("init mqtt");
  WiFi.begin(wifiSsid.c_str(), wifiPwd.c_str());
  client.enableDebuggingMessages();
  client.enableHTTPWebUpdater();
  client.enableOTA();
  client.setMaxPacketSize(512);
}

void mqttControle()
{
  client.subscribe("iot/led", [](const String &payload)
                   { ledControle(payload); });
  client.subscribe("iot/brightness", [](const String &payload)
                   { brightnessControle(payload); });
  client.subscribe("iot/planification", [](const String &payload)
                   { planifControle(payload); });
  client.subscribe("iot/disable-touch", [](const String &payload)
                   { mqttDisableTouch(payload); });
}

void ledControle(String payload)
{
  uint32_t c = payload.toInt();
  if (c > 4)
    changeColor(c);
  else if (c >= 0 && c <= 4)
    setState(c);
}

void brightnessControle(String payload)
{
  if (!payload.isEmpty())
  {
    int b = payload.toInt();
    if (b > 0 && b <= 255)
      changeBrightness(b);
  }
}

void planifControle(String payload)
{
  if (payload.length() == 1)
    deletePlanif(payload);
  else if (payload == "reset")
    resetPlanif();
  else
    addPlanif(payload);
}

void mqttDisableTouch(String payload)
{
  if (payload == "1")
    touchDisabled = true;
  else if (payload == "0")
    touchDisabled = false;
}

void sendTelemetrie()
{
  String s = state ? "1" : "0";
  String c = state ? (String)color : "0";
  String b = state ? (String)brightness : "0";
  String msg = "{\"state\": " + s + ",\"brightness\": " + b + ",\"color\": " + c + "}";
  client.publish("iot/telemetrie", msg);
  Serial.println(state);
}

void sendPlanifList()
{
  String msg = "{ \"nbPlanif\": " + (String)nbPlanif + ", \"planif\": [";
  for (int i = 0; i < nbPlanif; i++)
  {
    Planif p = planifs[i];
    msg +=
        "{\"day\":" + (String)p.day + "," +
        "\"hour\":" + (String)p.hour + "," +
        "\"min\":" + (String)p.min + "," +
        "\"color\":" + (String)p.color + "," +
        "\"brightness\":" + (String)p.brightness + "}";
    if (i < nbPlanif - 1)
      msg += ", ";
  }
  msg += "] }";
  client.publish("iot/list-planif", msg);
}
