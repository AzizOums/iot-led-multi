#include "saveData.h"
#include "mqttCtrl.h"
#include "led.h"

void initEEPROM()
{
  EEPROM.begin(MAXSIZE);
}

void resetMemory()
{
  initialised = false;
  wifiSsid = "";
  wifiPwd = "";
  nbPlanif = 0;
  indexPlanif = 0;
  for (int i = 0; i < MAXPLANIF; i++)
    planifs[i] = {0};
  state = 0;
  brightness = 0;
  color = 0;
  saveData();
  savePlanifs();
  saveMqttInfo();
}

void saveData()
{
  saveState();
  saveColor();
  saveBrightness();
  savePlanifs();
  EEPROM.commit();
}

void restorePlanifs()
{
  nbPlanif = getNbPlanif();
  indexPlanif = getIndexPlanif();
  for (int i = 0; i < nbPlanif; i++)
    planifs[i] = getPlanif(i);
}

void restoreMqttInfo()
{
  wifiSsid = getSSID();
  wifiPwd = getPWD();
  initialised = getInitialised();
}

void restoreData()
{
  restoreMqttInfo();
  restorePlanifs();

  state = getState();
  color = getColor();
  brightness = getBrightness();

  changeBrightness(brightness);
  if (state != 0)
    changeColor(color);
  else
    setState(state);
}

// SAVE

boolean saveState()
{
  if (getState() != state)
  {
    EEPROM.writeUChar(addrState, state);
    Serial.println("save sate");
  }
}

boolean saveColor()
{
  if (getColor() != color)
  {
    EEPROM.writeUInt(addrColor, color);
    Serial.println("save color");
  }
}

boolean saveBrightness()
{
  if (getBrightness() != brightness)
  {
    return EEPROM.writeUChar(addrBrightness, brightness);
    Serial.println("save brightness");
  }
}

boolean savePlanifs()
{
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
    EEPROM.commit();

    Serial.println("save Planif");
    return true;
  }
  return false;
}

boolean saveNbPlanif()
{
  if (getNbPlanif() != nbPlanif)
  {
    return EEPROM.writeUChar(addrNbPlanif, nbPlanif);
    Serial.println("save nb planif");
  }
  return false;
}

boolean saveIndexPlanif()
{
  if (getIndexPlanif() != indexPlanif)
  {
    return EEPROM.writeUChar(addrIndexPlanif, indexPlanif);
    Serial.println("save index planif");
  }
  return false;
}

boolean saveMqttInfo()
{
  saveInitialised();
  saveSSID();
  savePWD();
  EEPROM.commit();
  return true;
}

boolean saveString(int addr, String s)
{
  if (!EEPROM.readString(addr).equals(s))
  {
    return EEPROM.writeString(addr, s);
    Serial.println("save string : " + s);
  }
  return false;
}

boolean saveSSID()
{
  return saveString(addrSSID, wifiSsid);
}

boolean savePWD()
{
  return saveString(addrPWD, wifiPwd);
}

boolean saveInitialised()
{
  if (getInitialised() != initialised)
  {
    return EEPROM.writeBool(addrInit, initialised);
    Serial.println("save init");
  }
  return false;
}

// GET
uint8_t getState()
{
  return EEPROM.readUChar(addrState);
}

uint32_t getColor()
{
  return EEPROM.readUInt(addrColor);
}

uint8_t getBrightness()
{
  return EEPROM.readUChar(addrBrightness);
}

Planif getPlanif(int i)
{
  Planif p;
  int a = addrPlanif + (i * sizeof(Planif));
  EEPROM.get(a, p);
  return p;
}

uint8_t getNbPlanif()
{
  return EEPROM.readUChar(addrNbPlanif);
}

uint8_t getIndexPlanif()
{
  return EEPROM.readUChar(addrIndexPlanif);
}

String getString(int addr)
{
  return EEPROM.readString(addr);
}

String getSSID()
{
  return getString(addrSSID);
}

String getPWD()
{
  return getString(addrPWD);
}

boolean getInitialised()
{
  return EEPROM.readBool(addrInit);
}