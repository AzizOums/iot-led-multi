#include "mqttCtrl.h"
#include "planif.h"
#include "led.h"

void onConnectionEstablished()
{
  mqttControle();
}

void mqttSetup()
{
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
