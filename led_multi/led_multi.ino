#include <time.h>

#include "EspMQTTClient.h"
#include <Adafruit_NeoPixel.h>

#define PIN       18
#define NUMPIXELS 16
#define DELAYVAL  500
#define MAXPLANIF 7

time_t now;
time_t last;
struct tm* date;
const char* ntpServer = "pool.ntp.org";

String planif[MAXPLANIF];
int nbPlanif = 0;
int indexPlanif = 0;

boolean initialised = false;
int brightness = 180;
int color = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
EspMQTTClient client("Aziz", "Aziz1998", "test.mosquitto.org");

void mqttSetup() {
  client.enableDebuggingMessages();
  client.enableHTTPWebUpdater();
  client.enableOTA();
}

void ledControle(String payload) {
  int c = -1;
  if(payload == "0") c = 0;
  else if(payload == "1") c = 1;
  else if(payload == "2") c = 2;
  else if(payload == "3") c = 3;
  else if(payload == "4") c = 4;
  if(c != -1 && c != color) {
    setState(c);
    color = c;
  }
}

void brightnessControle(String payload) {
  if(payload) {
    int b = payload.toInt();
    if(b > 0 && b <= 255) changeBrightness(b);
  }
}

void planifControle(String payload) {
  if(payload.length() == 13) {
    int i = indexPlanif % MAXPLANIF;
    planif[i] = payload;
    if(nbPlanif < MAXPLANIF) nbPlanif++;
    indexPlanif = i + 1;
  }
}

void mqttControle() {
  client.subscribe("iot/led", [](const String & payload) {
    ledControle(payload);
  });
  client.subscribe("iot/brightness", [](const String & payload) {
    brightnessControle(payload);
  });
  client.subscribe("iot/planification", [](const String & payload) {
    planifControle(payload);
  });
}

void sendTelemetrie() {
  String s = "\"state\": " + (String)((color != 0 ? 1 : 0));
  String b = "\"brightness\": " + (String)(color != 0 ? brightness : 0);
  String c = "\"color\": ";
  switch (color) {
    case 0: c += "\"None\""; break;
    case 1: c += "\"Orange\""; break;
    case 2: c += "\"Purple\""; break;
    case 3: c += "\"Green\""; break;
    case 4: c += "\"White\""; break;
  }
  String msg = "{" + s + ", " + c + ", " + b + "}";
  client.publish("iot/telemetrie", msg);
}

void listPlanif() {
  String msg = "{ \"nbPlanif\": " + (String)nbPlanif + ", \"planif\": [";
  for(int i=0; i<nbPlanif; i++) {
    msg += "\"" + planif[i] + "\"";
    if(i != nbPlanif - 1) msg += ", ";
  }
  msg += "] }";
  client.publish("iot/list-planif", msg);
}

void onConnectionEstablished() {
  mqttControle();
}

void changeColor(int r, int g, int b) {
  pixels.clear();
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  }
}

void changeBrightness(int b) {
  brightness = b;
  pixels.setBrightness(b);
  pixels.show();
}

void setPurple() {
  changeColor(255,0,255);
}

void setGreen() {
  changeColor(173,255,47);
}

void setOrange() {
  changeColor(255,69,0);
}

void setWhite() {
  changeColor(248,248,255);
}

void shutDown() {
  pixels.clear();
  pixels.show();
}

void setState(int c) {
  switch (c % 5) {
    case 0: shutDown(); break;
    case 1: setOrange(); break;
    case 2: setPurple(); break;
    case 3: setGreen(); break;
    case 4: setWhite(); break;
  }
}

void touchControle() {
  if (touchRead(13) < 10) {
    color ++;
    setState(color);
    if(color % 5 == 0) color = 0;
    if(!initialised) initialised = true;
  }
}

void getTime() {
  time_t timestamp = time(NULL);
  ctime(&timestamp);
  now = timestamp;
  date = localtime(&now);
}

// s doit être du format: D HH:MM C BBB
void compareTime(String s) {
  int d = s.substring(0, 1).toInt();
  int h = s.substring(2, 4).toInt();
  int m = s.substring(5, 7).toInt();
  int c = s.substring(8, 9).toInt();
  int b = s.substring(10, s.length()).toInt();
  if(date->tm_wday == d && date->tm_hour == h && date->tm_min == m && date->tm_sec >= 0 && date->tm_sec < 3) {
    color = c;
    brightness = b;
    setState(color);
    changeBrightness(b);
  }
}

void comparePlanif() {
  for(int i=0; i<nbPlanif; i++) {
    compareTime(planif[i]);
  }
}

void setup() {
  configTzTime("CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00", ntpServer);
  Serial.begin(115200);
  pixels.begin();
  mqttSetup();
  setState(0);
}

void loop() {
  getTime();
  if(initialised) {
    client.loop();
    if (now - last >= 10) {
      sendTelemetrie();
      listPlanif();
      last = now;
    }
  }
  if(!client.isConnected())
    touchControle();
  if(nbPlanif > 0) comparePlanif();
  delay(DELAYVAL);
}
