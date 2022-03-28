#ifndef _MQTT_CTRL_H_
#define _MQTT_CTRL_H_

#include <EspMQTTClient.h>

boolean touchDisabled = false;
boolean initialised = false;
String wifiSsid;
String wifiPwd;

const char *user = "ESP32";
const char *pwd = "ESP32Password";
const char *mqttServer = "35.180.55.122";

EspMQTTClient client(mqttServer, 1884, user, pwd);

void setWifiParams(String ssid, String pwd);
void mqttSetup();
void onConnectionEstablished();

void mqttControle();
void ledControle(String payload);
void brightnessControle(String payload);
void planifControle(String payload);
void mqttDisableTouch(String payload);

void sendTelemetrie();
void sendPlanifList();

#endif