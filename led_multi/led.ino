#include "led.h"

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

void changeBrightness(int b)
{
  pixels.setBrightness(b);
  pixels.show();
  if (brightness != b)
    brightness = b;
}

void setOrange()
{
  changeColor(pixels.Color(255, 69, 0));
  state = 1;
}

void setPurple()
{
  changeColor(pixels.Color(255, 0, 255));
  state = 2;
}

void setGreen()
{
  changeColor(pixels.Color(173, 255, 47));
  state = 3;
}

void setWhite()
{
  changeColor(pixels.Color(248, 248, 255));
  state = 4;
}

void shutDown()
{
  pixels.clear();
  pixels.show();
  state = 0;
}

void setState(int c)
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
  int t = touchRead(TOUCH_PIN);
  if (t < 25)
  {
    state++;
    setState(state);
    if (!initialised)
      initialised = true;
  }
}