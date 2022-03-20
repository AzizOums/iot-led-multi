#include "saveData.h"
#include "led.h"

void initEEPROM()
{
  EEPROM.begin(MAXSIZE);
}

void saveData()
{
  boolean t[4] = {saveState(), saveColor(), saveBrightness(), savePlanifs()};
  boolean test = false;
  for (int i = 0; i < 4; i++)
    test = test || t[i];
  if (test)
    EEPROM.commit();
  if (t[0])
    Serial.println("saved state");
  if (t[1])
    Serial.println("saved color");
  if (t[2])
    Serial.println("saved brightness");
  if (t[3])
    Serial.println("saved planifications");
}

void restorePlanifs()
{
  Serial.println("restore pplanif");
  nbPlanif = getNbPlanif();
  indexPlanif = getIndexPlanif();
  Serial.printf("nbPlanif: %d \nindexPlanif: %d \n", nbPlanif, indexPlanif);
  for (int i = 0; i < nbPlanif; i++)
    planifs[i] = getPlanif(i);
}

void restoreData()
{
  state = getState();
  color = getColor();
  brightness = getBrightness();
  restorePlanifs();

  changeBrightness(brightness);
  if (state)
    changeColor(color);
  else
    setState(state);
}

boolean saveState()
{
  if (getState() != state)
  {
    EEPROM.put(addrState, state);
    return true;
  }
  return false;
}

boolean saveColor()
{
  if (getColor() != color)
  {
    EEPROM.put(addrColor, color);
    return true;
  }
  return false;
}

boolean saveBrightness()
{
  if (getBrightness() != brightness)
  {
    EEPROM.put(addrBrightness, brightness);
    return true;
  }
  return false;
}

uint8_t getState()
{
  uint8_t s;
  EEPROM.get(addrState, s);
  return s;
}

uint32_t getColor()
{
  uint32_t c;
  EEPROM.get(addrColor, c);
  return c;
}

uint8_t getBrightness()
{
  uint8_t b;
  EEPROM.get(addrBrightness, b);
  return b;
}

// boolean equals(Planif p[], Planif p2[])
// {
//   for (int i = 0; i < nbPlanif; i++)
//     if (!isEqual(p[i], p2[i]))
//       return false;
//   return true;
// }

boolean savePlanifs()
{
  // Planif p[MAXPLANIF];
  // getPlanifs(p);
  // if (!equals(p, planifs))
  // {
  //   EEPROM.put(addrPlanif, planifs);
  //   return true;
  // }
  boolean res = false;
  res = res || saveNbPlanif();
  res = res || saveIndexPlanif();
  for (int i = 0; i < nbPlanif; i++)
    res = res || savePlanif(i);
  return res;
}

boolean savePlanif(int i)
{
  if (!isEqual(getPlanif(i), planifs[i]))
  {
    int a = addrPlanif + (i * sizeof(Planif));
    EEPROM.put(a, planifs[i]);
    Serial.printf("save planif %d\n", i);
    return true;
  }
  return false;
}

boolean saveNbPlanif()
{
  if (getNbPlanif() != nbPlanif)
  {
    EEPROM.put(addrNbPlanif, nbPlanif);
    return true;
  }
  return false;
}

boolean saveIndexPlanif()
{
  if (getIndexPlanif() != indexPlanif)
  {
    EEPROM.put(addrIndexPlanif, indexPlanif);
    return true;
  }
  return false;
}

// void getPlanifs(Planif p[])
// {
//   EEPROM.get(addrPlanif, p);
// }

Planif getPlanif(int i)
{
  Planif p;
  int a = addrPlanif + (i * sizeof(Planif));
  EEPROM.get(a, p);
  return p;
}

uint8_t getNbPlanif()
{
  uint8_t n;
  EEPROM.get(addrNbPlanif, n);
  return n;
}

uint8_t getIndexPlanif()
{
  uint8_t i;
  EEPROM.get(addrIndexPlanif, i);
  return i;
}