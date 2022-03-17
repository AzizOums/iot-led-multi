#include "mqttCtrl.h"

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

// D HH:MM CCCCCC BBB
void planifControle(String payload)
{
  if (!payload.isEmpty())
  {
    String list[4];
    split(payload, list, ' ');
    int i = indexPlanif % MAXPLANIF;
    planifs[i].day = list[0].toInt();
    planifs[i].color = list[2].toInt();
    planifs[i].brightness = list[3].toInt();
    String t[2];
    split(list[1], t, ':');
    planifs[i].hour = t[0].toInt();
    planifs[i].min = t[1].toInt();
    if (nbPlanif < MAXPLANIF)
      nbPlanif++;
    indexPlanif = i + 1;
  }
}

void comparePlanifs()
{
  for (int i = 0; i < nbPlanif; i++)
    if (isSameTime(planifs[i]))
    {
      changeColor(planifs[i].color);
      changeBrightness(planifs[i].color);
    }
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

void sendTelemetrie()
{
  String msg =
      "{\"state\": " + (String)((color != 0 ? 1 : 0)) +
      ",\"brightness\": " + (String)brightness +
      ",\"color\": " + (String)color + "}";
  client.publish("iot/telemetrie", msg);
}

void listPlanif()
{
  String msg = "{ \"nbPlanif\": " + (String)nbPlanif + ", \"planif\": [";
  for (int i = 0; i < nbPlanif; i++)
  {
    msg += "{\"day\":" + (String)planifs[i].day + "," +
           "\"hour\":" + (String)planifs[i].hour + "," +
           "\"min\":" + (String)planifs[i].min + "," +
           "\"color\":" + (String)planifs[i].color + "," +
           "\"brightness\":" + (String)planifs[i].brightness + "}";
    if (i < nbPlanif - 1)
      msg += ", ";
  }
  msg += "] }";
  client.publish("iot/list-planif", msg);
}

void getCurrentTime()
{
  time_t timestamp = time(NULL);
  ctime(&timestamp);
  now = timestamp;
  date = localtime(&now);
}

boolean isSameTime(struct Planif p)
{
  return date->tm_wday == p.day &&
         date->tm_hour == p.hour &&
         date->tm_min == p.min &&
         date->tm_sec >= 0 &&
         date->tm_sec < 3;
}

void split(String s, String list[], char c)
{
  int index = 0;
  int last = 0;
  for (int i = 0; i < s.length(); i++)
  {
    if (s.charAt(i) == c)
    {
      list[index] = s.substring(last, i);
      index++;
      last = i + 1;
    }
    else if (i == s.length() - 1)
      list[index] = s.substring(last, i + 1);
  }
}