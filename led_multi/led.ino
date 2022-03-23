#include "led.h"
#include "saveData.h"

void initLed()
{
  pixels.begin();
}

void changeColor(uint32_t c)
{
  pixels.clear();
  pixels.setPixelColor(PIXEL, c);
  pixels.show();
  if (color != c)
    color = c;
  if (!state)
    state = 1;
}

void changeBrightness(uint8_t b)
{
  pixels.setBrightness(b);
  pixels.show();
  if (brightness != b)
    brightness = b;
}

void setOrange()
{
  if (state != 1)
    state = 1;
  changeColor(pixels.Color(255, 69, 0));
}

void setPurple()
{
  if (state != 2)
    state = 2;
  changeColor(pixels.Color(255, 0, 255));
}

void setGreen()
{
  if (state != 3)
    state = 3;
  changeColor(pixels.Color(173, 255, 47));
}

void setWhite()
{
  if (state != 4)
    state = 4;
  changeColor(pixels.Color(248, 248, 255));
}

void shutDown()
{
  if (state != 0)
    state = 0;
  pixels.clear();
  pixels.show();
}

void setState(uint8_t c)
{
  switch (c % 5)
  {
  case 0:
    shutDown();
    break;
  case 1:
    setOrange();
    break;
  case 2:
    setPurple();
    break;
  case 3:
    setGreen();
    break;
  case 4:
    setWhite();
    break;
  }
}

// capteur de toucher
void touchControle()
{
  if (touchRead(TOUCH_PIN) < 25)
  {
    state++;
    setState(state);
  }
  if (touchRead(TOUCH_PIN_B) < 25)
  {
    stateB = ++stateB % 4;
    changeBrightness((255 / 4) * (stateB + 1));
  }
}