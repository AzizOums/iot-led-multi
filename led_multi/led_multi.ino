#include "led.h"
#include "planif.h"
#include "mqttCtrl.h"
#include "saveData.h"

#define DELAYVAL 500

const char *ntpServer = "pool.ntp.org";
const char *tz = "CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";

time_t now;
time_t last;
struct tm *date;

void getCurrentTime()
{
  time_t timestamp = time(NULL);
  ctime(&timestamp);
  now = timestamp;
  date = localtime(&now);
}

void mqttSend()
{
  if (now - last >= 10)
  {
    sendTelemetrie();
    sendPlanifList();
    last = now;
  }
}

void comparePlanifs()
{
  int i = getPlanif(date->tm_wday, date->tm_hour, date->tm_min);
  if (i != -1)
  {
    uint32_t c = planifs[i].color;
    if (0 <= c && c < 5)
      setState(c);
    else
      changeColor(c);
    changeBrightness(planifs[i].brightness);
  }
}

void setup()
{
  configTzTime(tz, ntpServer);
  Serial.begin(115200);
  initLed();
  initEEPROM();
  restoreData();
  mqttSetup();
}

void loop()
{
  getCurrentTime();
  client.loop();
  if (client.isConnected())
    mqttSend();
  else
    touchControle();
  if (nbPlanif > 0)
    comparePlanifs();
  saveData();
  delay(DELAYVAL);
}
