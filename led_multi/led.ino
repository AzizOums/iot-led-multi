#include "led.h"

void changeColor(uint32_t c)
{
  pixels.clear();
  pixels.setPixelColor(PIXEL, c);
  pixels.show();
  if (color != c)
    color = c;
}

void changeBrightness(int b)
{
  pixels.setBrightness(b);
  pixels.show();
  if (brightness != b)
    brightness = b;
}

void setPurple()
{
  changeColor(pixels.Color(255, 0, 255));
}

void setGreen()
{
  changeColor(pixels.Color(173, 255, 47));
}

void setOrange()
{
  changeColor(pixels.Color(255, 69, 0));
}

void setWhite()
{
  changeColor(pixels.Color(248, 248, 255));
}

void shutDown()
{
  pixels.clear();
  pixels.show();
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
    if (state % 5 == 0)
      state = 0;
    if (!initialised)
      initialised = true;
  }
}