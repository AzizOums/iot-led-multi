#ifndef _SAVE_DATA_H_
#define _SAVE_DATA_H_

#include <EEPROM.h>
#include "planif.h"

int addrState = 0;
int addrColor = addrState + sizeof(state);
int addrBrightness = addrColor + sizeof(color);
int addrNbPlanif = addrBrightness + sizeof(brightness);
int addrIndexPlanif = addrNbPlanif + sizeof(nbPlanif);
int addrPlanif = addrIndexPlanif + sizeof(indexPlanif);

const int MAXSIZE = 5 * sizeof(uint8_t) + sizeof(uint32_t) + sizeof(planifs) + 3 * sizeof(String);

void initEEPROM();

void saveData();
void restoreData();
void restorePlanifs();

boolean saveState();
boolean saveColor();
boolean saveBrightness();

uint8_t getState();
uint32_t getColor();
uint8_t getBrightness();

boolean savePlanifs();
boolean saveNbPlanif();
boolean saveIndexPlanif();
boolean savePlanif(int i);

uint8_t getNbPlanif();
uint8_t getIndexPlanif();
Planif getPlanif(int i);

#endif