#ifndef _PLANIF_H_
#define _PLANIF_H_

#define MAXPLANIF 5

struct Planif
{
  uint8_t day;
  uint8_t hour;
  uint8_t min;
  uint32_t color;
  uint8_t brightness;
};

Planif planifs[MAXPLANIF];
uint8_t nbPlanif = 0;
uint8_t indexPlanif = 0;

int getPlanif(int d, int h, int m);
void addPlanif(String payload);
void deletePlanif(String payload);
void resetPlanif();

boolean isTime(Planif p, int d, int h, int m);
boolean isEqual(Planif p1, Planif p2);
int containsPlanif(struct Planif p);

void split(String s, String list[], char c);

#endif