#ifndef _MQTT_CTRL_
#define _MQTT_CTRL_

#include <EspMQTTClient.h>
#include "led.h"

#define MAXPLANIF 7

time_t now;
time_t last;
struct tm *date;

struct Planif
{
  int day;
  int hour;
  int min;
  uint32_t color;
  int brightness;
};

const char *ntpServer = "pool.ntp.org";
const char *mqttServer = "test.mosquitto.org";
const char *wifiSsid = "Aziz";
const char *wifiPwd = "Aziz1998";

// String planif[MAXPLANIF];
struct Planif planifs[MAXPLANIF];
int nbPlanif = 0;
int indexPlanif = 0;

EspMQTTClient client(wifiSsid, wifiPwd, mqttServer);

void mqttSetup();
void onConnectionEstablished();

void ledControle(String payload);
void planifControle(String payload);
void mqttControle();

void sendTelemetrie();
void listPlanif();

void comparePlanifs();
void getCurrentTime();
boolean isSameTime(struct Planif p);

// split a string into list
void split(String s, String list[], char c);

#endif