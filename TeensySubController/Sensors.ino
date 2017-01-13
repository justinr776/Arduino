/*
  ADC Inputs: Oil Pressure (31), Fuel Pressure (32), Coolant Pressure(33)
  Digital Inputs: Clutch(34), Brake(35), Handbrake(36), Rotory(11,12,13)
  I2C: Accelerometer(37,38)
*/
#include "Sensors.h"

void initializeSensors() {
  pinMode(clutchPin, INPUT);
  pinMode(brakePin, INPUT);
  pinMode(handbrakePin, INPUT);
  
  // Accel & Gyro Initialization
//  Wire.begin();
//  Wire.beginTransmission(MPU_addr);
//  Wire.write(0x6B);  // PWR_MGMT_1 register
//  Wire.write(0);     // set to zero (wakes up the MPU-6050)
//  Wire.endTransmission(true);
}

void readSensors(){
  oil = analogRead(oilPressPin);    //Read oil pressure
  fuel = analogRead(fuelPressPin);  //Read fuel pressure
  clt = analogRead(cltPressPin);    //Read coolant pressure
  clutch = digitalRead(clutchPin);
  brake = digitalRead(brakePin);
  handbrake = digitalRead(handbrakePin);
}

