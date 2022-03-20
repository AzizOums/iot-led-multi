#ifndef _MQTT_CTRL_H_
#define _MQTT_CTRL_H_

#include <EspMQTTClient.h>

const char *mqttServer = "test.mosquitto.org";
const char *wifiSsid = "Aziz";
const char *wifiPwd = "Aziz1998";

EspMQTTClient client(wifiSsid, wifiPwd, mqttServer);

void mqttSetup();
void onConnectionEstablished();

void mqttControle();
void ledControle(String payload);
void brightnessControle(String payload);
void planifControle(String payload);

void sendTelemetrie();
void sendPlanifList();

#endif