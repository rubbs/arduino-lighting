#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "FastLED.h"
// Fastled constants
#define DATA_PIN    2
#define COLOR_ORDER GRB
#define NUM_LEDS    300
#define LED_TYPE    WS2812B
#define BRIGHTNESS  64
CRGB leds[NUM_LEDS];

#ifndef STASSID
#define STASSID "rswireless"
#define STAPSK  "rubenundrebecca0905"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

const String postForms = "<html>\
  <head>\
    <title>ESP8266 Web Server POST handling</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
  \  
    <h1>Controlling the led</h1><br>\
    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\">\
      <label for=\"brightness\">brightness:</label><br>\
      <input id=\"brightness\"type=\"number\" name=\"brightness\" value=\"255\"><br>\
      \
      <label for=\"r\">red:</label><br>\
      <input id=\"r\"type=\"number\" name=\"r\" value=\"255\"><br>\
      \
      <label for=\"g\">green:</label><br>\
      <input id=\"g\"type=\"number\" name=\"g\" value=\"255\"><br>\
      \
      <label for=\"b\">blue:</label><br>\
      <input id=\"b\"type=\"number\" name=\"b\" value=\"255\"><br>\
      \
      <input type=\"submit\" value=\"Submit\">\
    </form>\
  </body>\
</html>";

void handleRoot() {
  server.send(200, "text/html", postForms);
}

void handleForm() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    String message = "POST form was:\n";

    int r,g,b,bb = 0;
    
    for (uint8_t i = 0; i < server.args(); i++) {
      if(server.argName(i) == "brightness") {
        bb = server.arg(i).toInt();
      } else if (server.argName(i) == "r") {
        r = server.arg(i).toInt();
      } else if (server.argName(i) == "g") {
        g = server.arg(i).toInt();
      } else if (server.argName(i) == "b") {
        b = server.arg(i).toInt();
      }
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    // update the leds
    FastLED.setBrightness(bb);
    fill_solid (leds, NUM_LEDS, CRGB(r,g,b));
    FastLED.show();
        
    server.send(200, "text/plain", message);
  }
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setupLed(void) {
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
  FastLED.clear();
}

void wifiConnected(void) {
  const int maxBrightness = 150;
  fill_solid (leds, NUM_LEDS, CRGB::Green);
  for(int i=0; i<maxBrightness;i++) {
    FastLED.setBrightness(i);
    FastLED.show();
    delay(1);
  }
  for(int i=0; i<maxBrightness;i++) {
    FastLED.setBrightness(maxBrightness-i);
    FastLED.show();
    delay(1);
  }
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
}

void setup(void) {
  setupLed();
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(led, 1);
  wifiConnected();

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/postform/", handleForm);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
