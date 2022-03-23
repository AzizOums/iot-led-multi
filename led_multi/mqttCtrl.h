#ifndef _MQTT_CTRL_H_
#define _MQTT_CTRL_H_

#include <ssl_client.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

boolean touchDisabled = false;
boolean initialised = false;

const short MQTT_PORT = 8883;
const char THINGNAME[] = "ESP32";

// const char *mqttServer = "test.mosquitto.org";

String wifiSsid;
String wifiPwd;

WiFiClientSecure net;
// WiFiClient espClient;
PubSubClient client(net);

boolean isInit();

void callback(char *topic, byte *message, unsigned int length);
void setWifiParams(String ssid, String pwd);
void mqttSetup();
void mqttConnect();
void mqttSubscribe();

void ledControle(String payload);
void brightnessControle(String payload);
void planifControle(String payload);
void mqttDisableTouch(String payload);

void sendTelemetrie();
void sendPlanifList();

#endif