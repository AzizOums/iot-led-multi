#ifndef _MQTT_CTRL_H_
#define _MQTT_CTRL_H_

#include <EspMQTTClient.h>

boolean touchDisabled = false;
boolean initialised = false;

const char *mqttServer = "test.mosquitto.org";
String wifiSsid;
String wifiPwd;

const char *ss = "default";
const char *pp = "default";

EspMQTTClient client(mqttServer, 1883);

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