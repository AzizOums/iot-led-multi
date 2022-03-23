#ifndef _SAVE_DATA_H_
#define _SAVE_DATA_H_

#include <EEPROM.h>
#include "planif.h"

int addrInit = 0;
int addrSSID = addrInit + sizeof(boolean);
int addrPWD = addrSSID + sizeof(String);
int addrState = addrPWD + sizeof(String);
int addrColor = addrState + sizeof(uint8_t);
int addrBrightness = addrColor + sizeof(uint32_t);
int addrNbPlanif = addrBrightness + sizeof(uint8_t);
int addrIndexPlanif = addrNbPlanif + sizeof(uint8_t);
int addrPlanif = addrIndexPlanif + sizeof(uint8_t);

const int MAXSIZE = 6 * sizeof(uint8_t) + sizeof(uint32_t) + sizeof(planifs) + 2 * sizeof(String);

void initEEPROM();
void saveData();
void restorePlanifs();
void restoreMqttInfo();
void restoreData();

// SAVE
boolean saveState();
boolean saveColor();
boolean saveBrightness();
boolean savePlanifs();
boolean savePlanif(int i);
boolean saveNbPlanif();
boolean saveIndexPlanif();
boolean saveMqttInfo();
boolean saveString(int addr, String s);
boolean saveSSID();
boolean savePWD();
boolean saveInitialised();

// GET
uint8_t getState();
uint32_t getColor();
uint8_t getBrightness();
Planif getPlanif(int i);
uint8_t getNbPlanif();
uint8_t getIndexPlanif();
String getString(int addr);
String getSSID();
String getPWD();
boolean getInitialised();

#endif