#include <OneWire.h>
#include <DallasTemperature.h>
#include "TempConversions.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

const char* ssid = "4Rohr";
const char* password = "ilovepie";
WiFiServer server(1337);
#define DSPIN D3

OneWire  ds(DSPIN);  // on pin 10 (a 4.7K resistor is necessary)
DallasTemperature sensors(&ds);
uint8_t addr;
double houseTemp = 0, boilerTemp = 0, houseHumidity = 0;

void setup(void) {
  Serial.begin(115200);
  sensors.begin();
  WiFi.begin(ssid, password);
  server.begin();
  //Wait for WIFI connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  printWiFiStatus();
//  Serial.print("Resolution: ");
//  Serial.print(sensors.getResolution(0));
  Serial.print("Connected Devices: ");
  Serial.print(sensors.getDeviceCount());
}

void printWiFiStatus() {
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void getDSInfo() {
  sensors.requestTemperatures();
  Serial.print("\nDS18 Temperature (°F) is: ");
  boilerTemp = sensors.getTempFByIndex(0);
  Serial.println(boilerTemp);
}

void loop(void) {
  getDSInfo();
}
