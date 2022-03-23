#include "led.h"
#include "planif.h"
#include "mqttCtrl.h"
#include "saveData.h"
#include "initialise.h"

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
    Serial.println("trying to publish");
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
  Serial.begin(115200);
  initEEPROM();
  initLed();
  restoreData();

  if (isInit())
    mqttSetup();
  else
    setupWifi();

  configTzTime(tz, ntpServer);
}

// maintenir le pin 32 et 33 pendant 3 secondes pour reset
void reset()
{
  if (touchRead(32) < 25 && touchRead(33) < 25)
  {
    delay(3000);
    if (touchRead(32) < 25 && touchRead(33) < 25)
    {
      Serial.println("reseting ...");
      resetMemory();
      ESP.restart();
    }
  }
}

void loop()
{
  if (isInit())
  {
    getCurrentTime();

    client.loop();
    if (client.connected())
      mqttSend();
    else
      mqttConnect();

    if (nbPlanif > 0)
      comparePlanifs();

    if (!touchDisabled || !client.connected())
    {
      touchControle();
      if (touchRead(27) < 15)
        reset();
    }
    saveData();
  }
  else
    server.handleClient();

  delay(DELAYVAL);
}
