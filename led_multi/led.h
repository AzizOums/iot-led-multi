#ifndef _LED_H
#define _LED_H

#include <Adafruit_NeoPixel.h>

#define PIN 18
#define TOUCH_PIN 13

#define PIXEL 0
#define NUM_PIXELS 1

boolean initialised = false;
int brightness = 180;
uint32_t color = 0;
int state = 0;

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void changeColor(uint32_t c);
void changeBrightness(int b);

// Off
void shutDown(); // state: 0

// Couleurs prédéfinies
void setOrange(); // state: 1
void setPurple(); // state: 2
void setGreen();  // state: 3
void setWhite();  // state: 4

// Mettre l'une des couleurs prédéfinies ou off
void setState(int c);

// Utiliser le capteur de toucher pour changer de couleur
void touchControle();

#endif