#include "led.h"
#include "mqttCtrl.h"

#define DELAYVAL 500

void setup()
{
  configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
  Serial.begin(115200);
  pixels.begin();
  mqttSetup();
  setState(0);
}

void loop()
{
  getCurrentTime();
  if (initialised)
  {
    client.loop();
    if (now - last >= 10)
    {
      sendTelemetrie();
      listPlanif();
      last = now;
    }
  }
  if (!client.isConnected())
    touchControle();
  if (nbPlanif > 0)
    comparePlanifs();
  delay(DELAYVAL);
}
