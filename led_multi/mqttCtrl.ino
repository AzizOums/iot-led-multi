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

void mqttControle()
{
  client.subscribe("iot/led", [](const String &payload)
                   { ledControle(payload); });
  client.subscribe("iot/brightness", [](const String &payload)
                   { brightnessControle(payload); });
  client.subscribe("iot/planification", [](const String &payload)
                   { planifControle(payload); });
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

// D HH:MM CCCCCC BBB
void addPlanif(String payload)
{
  if (!payload.isEmpty())
  {
    struct Planif p;
    String list[4];
    String t[2];
    split(payload, list, ' ');
    split(list[1], t, ':');

    p.day = list[0].toInt();
    p.color = list[2].toInt();
    p.brightness = list[3].toInt();
    p.hour = t[0].toInt();
    p.min = t[1].toInt();

    int x = containsPlanif(p);
    int i = x == -1 ? indexPlanif % MAXPLANIF : x;
    planifs[i] = p;
    if (x == -1)
    {
      nbPlanif++;
      indexPlanif = i + 1;
    }
  }
}

void deletePlanif(String payload)
{
  int index = payload.toInt();
  if (payload != "0" && index == 0)
    return;
  if (index < nbPlanif && index >= 0)
  {
    planifs[index] = planifs[nbPlanif - 1];
    nbPlanif--;
    indexPlanif--;
  }
}

void resetPlanif()
{
  nbPlanif = 0;
  indexPlanif = 0;
}

void comparePlanifs()
{
  for (int i = 0; i < nbPlanif; i++)
    if (isSameTime(planifs[i]))
    {
      uint32_t c = planifs[i].color;
      if (0 <= c && c < 5)
        setState(c);
      else
        changeColor(c);
      changeBrightness(planifs[i].brightness);
    }
}

void sendTelemetrie()
{
  String s = state ? "1" : "0";
  String c = state ? (String)color : "0";
  String b = state ? (String)brightness : "0";
  String msg = "{\"state\": " + s + ",\"brightness\": " + b + ",\"color\": " + c + "}";
  client.publish("iot/telemetrie", msg);
  Serial.println(state);
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

int containsPlanif(struct Planif p)
{
  struct Planif x;
  for (int i = 0; i < nbPlanif; i++)
  {
    x = planifs[i];
    if (x.day == p.day && x.hour == p.hour && x.min == p.min)
      return i;
  }
  return -1;
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