#include <Adafruit_NeoPixel.h>
#include <EspMQTTClient.h>
#include <time.h>

#define PIN 18
#define NUMPIXELS 16
#define DELAYVAL 500
#define MAXPLANIF 7

time_t now;
time_t last;
struct tm *date;

const char *ntpServer = "pool.ntp.org";
const String mqttServer = "test.mosquitto.org";
const String wifiSsid = "Aziz";
const String wifiPwd = "Aziz1998";

String planif[MAXPLANIF];
int nbPlanif = 0;
int indexPlanif = 0;

boolean initialised = false;
int rgb[3];
int brightness = 180;
int color = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
EspMQTTClient client("Aziz", "Aziz1998", "test.mosquitto.org");

void mqttSetup()
{
  client.enableDebuggingMessages();
  client.enableHTTPWebUpdater();
  client.enableOTA();
}

void ledControle(String payload)
{
  if (payload.startsWith("#") && payload.length() == 7)
    setColor(payload);
  else
  {
    int c = payload.toInt();
    if (c != color && c >= 0 && c <= 4)
    {
      setState(c);
      color = c;
    }
  }
}

void brightnessControle(String payload)
{
  if (payload)
  {
    int b = payload.toInt();
    if (b > 0 && b <= 255)
      changeBrightness(b);
  }
}

void planifControle(String payload)
{
  if (payload.length() == 13)
  {
    int i = indexPlanif % MAXPLANIF;
    planif[i] = payload;
    if (nbPlanif < MAXPLANIF)
      nbPlanif++;
    indexPlanif = i + 1;
  }
}

void mqttControle()
{
  client.subscribe("iot/led", [](const String &payload)
                   { ledControle(payload); });
  client.subscribe("iot/brightness", [](const String &payload)
                   { brightnessControle(payload); });
  client.subscribe("iot/planification", [](const String &payload)
                   { planifControle(payload); });
}

void sendTelemetrie()
{
  String s = "\"state\": " + (String)((color != 0 ? 1 : 0));
  String b = "\"brightness\": " + (String)(color != 0 ? brightness : 0);
  String c = "\"color\": \"rgb(" + (String)rgb[0] + ", " + (String)rgb[1] + ", " + (String)rgb[2] + ")\"";
  String msg = "{" + s + ", " + c + ", " + b + "}";
  client.publish("iot/telemetrie", msg);
}

void listPlanif()
{
  String msg = "{ \"nbPlanif\": " + (String)nbPlanif + ", \"planif\": [";
  for (int i = 0; i < nbPlanif; i++)
  {
    msg += "\"" + planif[i] + "\"";
    if (i != nbPlanif - 1)
      msg += ", ";
  }
  msg += "] }";
  client.publish("iot/list-planif", msg);
}

void onConnectionEstablished()
{
  mqttControle();
}

void changeColor(int rgbCol[])
{
  pixels.clear();
  for (int i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, pixels.Color(rgbCol[0], rgbCol[1], rgbCol[2]));
    pixels.show();
  }
  if (rgb != rgbCol)
    rgb[0], rgb[1], rgb[2] = rgbCol[0], rgbCol[1], rgbCol[2];
}

void changeBrightness(int b)
{
  brightness = b;
  pixels.setBrightness(b);
  pixels.show();
}

void setColor(String hex)
{
  char x[3];
  hex.substring(1, 3).toCharArray(x, 3);
  rgb[0] = (int)strtol(x, NULL, 16);
  hex.substring(3, 5).toCharArray(x, 3);
  rgb[1] = (int)strtol(x, NULL, 16);
  hex.substring(5, 7).toCharArray(x, 3);
  rgb[2] = (int)strtol(x, NULL, 16);
  changeColor(rgb);
}

// couleurs prédéfinies
void setPurple()
{
  int tmp[] = {255, 0, 255};
  changeColor(tmp);
}

void setGreen()
{
  int tmp[] = {173, 255, 47};
  changeColor(tmp);
}

void setOrange()
{
  int tmp[] = {255, 69, 0};
  changeColor(tmp);
}

void setWhite()
{
  int tmp[] = {248, 248, 255};
  changeColor(tmp);
}

// led off
void shutDown()
{
  pixels.clear();
  pixels.show();
}

// mettre l'une des couleurs prédéfinies
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
  if (touchRead(13) < 10)
  {
    color++;
    setState(color);
    if (color % 5 == 0)
      color = 0;
    if (!initialised)
      initialised = true;
  }
}

void getTime()
{
  time_t timestamp = time(NULL);
  ctime(&timestamp);
  now = timestamp;
  date = localtime(&now);
}

// s: D HH:MM C BBB
void compareTime(String s)
{
  int d = s.substring(0, 1).toInt();
  int h = s.substring(2, 4).toInt();
  int m = s.substring(5, 7).toInt();
  int c = s.substring(8, 9).toInt();
  int b = s.substring(10, s.length()).toInt();
  if (date->tm_wday == d && date->tm_hour == h && date->tm_min == m && date->tm_sec >= 0 && date->tm_sec < 3)
  {
    color = c;
    brightness = b;
    setState(color);
    changeBrightness(b);
  }
}

void comparePlanif()
{
  for (int i = 0; i < nbPlanif; i++)
  {
    compareTime(planif[i]);
  }
}

void setup()
{
  configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
  Serial.begin(115200);
  pixels.begin();
  mqttSetup();
  setState(0);
}

void loop()
{
  getTime();
  if (initialised)
  {
    client.loop();
    if (now - last >= 10)
    {
      sendTelemetrie();
      listPlanif();
      last = now;
    }
  }
  if (!client.isConnected())
    touchControle();
  if (nbPlanif > 0)
    comparePlanif();
  delay(DELAYVAL);
}
