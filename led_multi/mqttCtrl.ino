// #include "WiFi.h"
#include "led.h"
#include "planif.h"
#include "secrets.h"
#include "saveData.h"
#include "mqttCtrl.h"

boolean isInit()
{
  return initialised && wifiSsid.length() > 0 && wifiPwd.length() > 0;
}

void callback(char *topic, byte *message, unsigned int length)
{
  String msg = "";
  for (int i = 0; i < length; i++)
    msg += (char)message[i];

  if (String(topic) == "iot/led")
    ledControle(msg);
  if (String(topic) == "iot/brightness")
    brightnessControle(msg);
  if (String(topic) == "iot/planification")
    planifControle(msg);
  if (String(topic) == "iot/disable-touch")
    mqttDisableTouch(msg);
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
  WiFi.setHostname(THINGNAME);
  WiFi.begin(wifiSsid.c_str(), wifiPwd.c_str());
  net.setCACert(cacert);
  net.setCertificate(client_cert);
  net.setPrivateKey(privkey);

  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
  client.setBufferSize(512);
  mqttConnect();
}

ulong lastReconnect = 0;

void mqttConnect()
{
  if (!WiFi.isConnected() && millis() - lastReconnect > 10000)
  {
    WiFi.reconnect();
    lastReconnect = millis();
  }
  if (WiFi.isConnected() && client.connect("ESP32Client"))
  {
    Serial.println("connected");
    mqttSubscribe();
  }
}

void mqttSubscribe()
{
  client.subscribe("iot/led");
  client.subscribe("iot/brightness");
  client.subscribe("iot/planification");
  client.subscribe("iot/disable-touch");
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
  Serial.println("trying to publish telemetrie");
  String s = state ? "1" : "0";
  String c = state ? (String)color : "0";
  String b = state ? (String)brightness : "0";
  String msg = "{\"state\": " + s + ",\"brightness\": " + b + ",\"color\": " + c + "}";
  client.publish("iot/telemetrie", msg.c_str());
}

void sendPlanifList()
{
  Serial.println("trying to publish planif");
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
  client.publish("iot/list-planif", msg.c_str());
}
