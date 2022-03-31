#include "planif.h"
#include "saveData.h"
#include "mqttCtrl.h"

int getPlanif(int d, int h, int m, int s)
{
  for (int i = 0; i < nbPlanif; i++)
    if (isTime(planifs[i], d, h, m, s))
      return i;
  return -1;
}

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
      if (nbPlanif < MAXPLANIF)
        nbPlanif++;
      indexPlanif = i + 1;
    }

    if (client.isConnected())
      sendPlanifList();
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
    planifs[nbPlanif - 1] = {0};
    if (nbPlanif == MAXPLANIF)
      indexPlanif = MAXPLANIF;
    nbPlanif--;
    indexPlanif--;
    if (client.isConnected())
      sendPlanifList();
  }
}

void resetPlanif()
{
  nbPlanif = 0;
  indexPlanif = 0;
  if (client.isConnected())
    sendPlanifList();
}

boolean isTime(Planif p, int d, int h, int m, int s)
{
  return d == p.day && h == p.hour && m == p.min && s <= 1;
}

boolean isEqual(Planif p1, Planif p2)
{
  return p1.day == p2.day &&
         p1.hour == p2.hour &&
         p1.min == p2.min &&
         p1.color == p2.color &&
         p1.brightness == p2.brightness;
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