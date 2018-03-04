#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C - NEED TO GROUND CSB as well
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
unsigned long delayTime;

double Fahrenheit(double celsius) {
  return 1.8 * celsius + 32;
}

void setup() {
  Serial.begin(115200);
  while (!bme.begin(0x76)) { //OR 0x77
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    delay(250);
  }
  delayTime = 5000;
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  Serial.print("Temperature = ");
  Serial.print(Fahrenheit(bme.readTemperature()));
  Serial.println(" *F");

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  Serial.println();
}

void loop() {
  printValues();
  delay(delayTime);
}

