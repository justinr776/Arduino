#include <OneWire.h>
#include <DallasTemperature.h>
#include "TempConversions.h"
#include <dht.h>
#define DHT11PIN A0
#define DSPIN 2

dht DHT11;
OneWire  ds(DSPIN);  // on pin 10 (a 4.7K resistor is necessary)
DallasTemperature sensors(&ds);
uint8_t addr;
double houseTemp = 0, boilerTemp = 0, houseHumidity = 0;

void setup(void) {
  Serial.begin(115200);
  sensors.begin();
  Serial.print("Resolution: ");
  Serial.println(sensors.getResolution());
}

void getDHTInfo(){  
  int chk = DHT11.read11(DHT11PIN);
  Serial.println("\nRead DHT11 sensor: ");
  switch (chk)  {
    case DHTLIB_OK:
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
  houseHumidity = DHT11.humidity; 
  houseTemp = Fahrenheit(DHT11.temperature);
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  Serial.print("Temperature (°F): ");
  Serial.println(Fahrenheit(DHT11.temperature), 2);
}

void getDSInfo(){   
  sensors.requestTemperatures();
  Serial.print("\nDS18 Temperature (°F) is: ");
  boilerTemp = sensors.getTempFByIndex(0);
  Serial.println(boilerTemp);
}

void loop(void) {
  getDHTInfo();
  getDSInfo();
}
