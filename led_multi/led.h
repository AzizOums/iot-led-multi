#ifndef _LED_H_
#define _LED_H_

#include <Adafruit_NeoPixel.h>

#define PIN 18
#define TOUCH_PIN 13
#define TOUCH_PIN_B 14
#define TOUCH_LIMIT 20

#define PIXEL 0
#define NUM_PIXELS 1

uint8_t brightness = 180;
uint32_t color = 0;
uint8_t state = 0;
uint8_t stateB = 0;

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void initLed();

void changeColor(uint32_t c);
void changeBrightness(uint8_t b);

// Off
void shutDown(); // state: 0

// Couleurs prédéfinies
void setOrange(); // state: 1
void setPurple(); // state: 2
void setGreen();  // state: 3
void setWhite();  // state: 4

// Mettre l'une des couleurs prédéfinies ou off
void setState(uint8_t c);

// Utiliser le capteur de toucher pour changer de couleur
void touchControle();

#endif