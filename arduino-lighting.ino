#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>

#include <ESP8266HTTPUpdateServer.h>

// local libraries
#include "led.h"

struct {
  char r;
  char g;
  char b;
  char brightness;
} PersistenceStruct;




#ifndef STASSID
#define STASSID "rswireless"
#define STAPSK  "rubenundrebecca0905"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
LedController ledController;

const int led = LED_BUILTIN;

String createIndexPage(char r, char g, char b, char brightness) {
  char buff[2];
  String postForms = "<html>\n"
                     "\n"
                     "<head>\n"
                     "    <title>Wohnzimmer</title>\n"
                     "    <style>\n"
                     "        body {\n"
                     "            font-family: Arial, Helvetica, Sans-Serif;\n"
                     "            Color: #000088;\n"
                     "        }\n"
                     "\n"
                     "        /* Style the label to display next to the inputs */\n"
                     "        label {\n"
                     "            padding: 12px 12px 12px 0;\n"
                     "            display: inline-block;\n"
                     "        }\n"
                     "\n"
                     "        /* Style the container */\n"
                     "        .container {\n"
                     "            border-radius: 5px;\n"
                     "            background-color: #f2f2f2;\n"
                     "            padding: 20px;\n"
                     "        }\n"
                     "\n"
                     "        /* Style inputs, select elements and textareas */\n"
                     "        input[type=range],\n"
                     "        select,\n"
                     "        textarea {\n"
                     "            width: 100%;\n"
                     "            padding: 12px;\n"
                     "            border: 1px solid #ccc;\n"
                     "            border-radius: 4px;\n"
                     "            box-sizing: border-box;\n"
                     "            resize: vertical;\n"
                     "        }\n"
                     "        /* Style inputs, select elements and textareas */\n"
                     "        input[type=range],\n"
                     "        textarea {\n"
                     "        width: 100%;\n"
                     "        padding: 12px;\n"
                     "        border: 1px solid #ccc;\n"
                     "        border-radius: 4px;\n"
                     "        box-sizing: border-box;\n"
                     "        resize: vertical;\n"
                     "        }\n"
                     "        #color{\n"
                     "        height: 44px;\n"
                     "        border: none;\n"
                     "        overflow: hidden;\n"
                     "        background-color: #f2f2f2;\n"
                     "        }\n"
                     "        #color::-moz-focus-inner {\n"
                     "        border: 0;\n"
                     "        }\n"
                     "        #color:focus {\n"
                     "        outline: none;\n"
                     "        }\n"
                     "        #color option{\n"
                     "        width: 80px;\n"
                     "        font-size: 1.2em;\n"
                     "        padding: 10px 0;\n"
                     "        text-align: center;\n"
                     "        margin-right: 20px;\n"
                     "        display: inline-block;\n"
                     "        cursor: pointer;\n"
                     "        border-radius: 5px;\n"
                     "        color: rgb(100, 100, 100);\n"
                     "        }\n"
                     "    </style>"
                     "</head>\n"
                     "\n"
                     "<body>\n"
                     "\n"
                     "    <h1>Controlling the led</h1><br>\n"
                     "    <form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postform/\" id=\"form\">\n"
                     "        <label for=\"brightness\">brightness:</label><br>\n"
                     "        <div>\n"
                     "            <input id=\"brightness\" type=\"range\" min=\"1\" max=\"255\" name=\"brightness\" value=\"";
  sprintf(buff, "%d", brightness);
  postForms.concat(buff);
  postForms.concat("\"><br>\n");

  postForms.concat("        </div>\n"
                   "\n"
                   "        <label for=\"color\">Color:</label><br>\n"
                   "        <div>\n"
                   "                            <select name=\"color\" id=\"color\" multiple>\n"
                   " <option style=\"background-color: #FFFFFF;\" value=\"#FFFFFF\">Weiß</option>\n"
                   " <option style=\"background-color: #FFA500;\" value=\"#FFA500\">Abend</option>\n"
                   " <option style=\"background-color: #0000FF;\" value=\"#0000FF\">Blau</option>\n"
                   " <option style=\"background-color: #FF0000;\" value=\"#FF0000\">Rot</option>\n"
                   " <option style=\"background-color: #00ff00;\" value=\"#00ff00\">Grün</option>\n"
                   "  </select>\n"
                   "        </div>\n"
                   "    </form>\n"
                   "</body>\n"
                   "\n"
                   "<script>\n"
                   "    var slider = document.getElementById(\"brightness\");\n"
                   "    var colorpicker = document.getElementById(\"color\");\n"
                   "\n"
                   "    slider.onchange = function(){\n"
                   "        console.log(\"brightness changed\")\n"
                   "        document.getElementById(\"form\").submit();\n"
                   "    }\n"
                   "    colorpicker.onchange = function(){\n"
                   "        console.log(\"colorpicker changed\");\n"
                   "        document.getElementById(\"form\").submit();\n"
                   "    }\n"
                   "</script>\n"
                   "\n"
                   "</html>");
  return postForms;
}

void handleRoot() {
  server.send(200, "text/html", createIndexPage(PersistenceStruct.r, PersistenceStruct.g, PersistenceStruct.b, PersistenceStruct.brightness));
}

void handleForm() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    String message = "POST form was:\n";
    String color = "no color";

    int r, g, b, bb = 0;

    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i) == "brightness") {
        bb = server.arg(i).toInt();
      } else if (server.argName(i) == "color") {
        color = server.arg(i);
      }
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    parseColor(color, r, g, b);

    // update the leds
    ledController.show(r,g,b,bb);

    // persist values
    eeprom_persist(r, g, b, bb);

    Serial.println(message);

    // redirect to home page
    server.sendHeader("Location", "/");
    server.send(303);//
  }
}

void parseColor(String color, int& r, int&g, int&b) {
  int parsedColors[] = {0, 0, 0};
  char buff[3];
  for (int i = 0; i < 3; i++) {
    // offset is always 1 as we start with (#)
    int start = 1 + i * 2;
    String substr = color.substring(start, start + 2);
    substr.toCharArray(buff, sizeof(buff));
    parsedColors[i] = strtol(buff, 0, 16);
  }
  r = parsedColors[0];
  g = parsedColors[1];
  b = parsedColors[2];
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

void setup(void) {
  ledController.setup();
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  Serial.println("");
  eeprom_setup();
  // setup leds with values from eeprom
  ledController.show(PersistenceStruct.r, PersistenceStruct.g, PersistenceStruct.b,PersistenceStruct.brightness);

  WiFi.hostname("wohnzimmer");
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
  ledController.success();
  
  ledController.show(PersistenceStruct.r, PersistenceStruct.g, PersistenceStruct.b,PersistenceStruct.brightness);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/postform/", handleForm);

  server.onNotFound(handleNotFound);

  httpUpdater.setup(&server);
  server.begin();
  Serial.println("HTTP server started");
  Serial.printf("HTTPUpdateServer ready! Open http://%s/update in your browser\n", WiFi.localIP().toString().c_str());
}

void loop(void) {
  //aunit::TestRunner::run(); // only run once
  server.handleClient();
}
