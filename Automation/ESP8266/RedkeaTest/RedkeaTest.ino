#include <DHTesp.h>
#include "RedkeaESP8266.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Conversions.h"
char ssid[] = "4Rohr";
char pass[] = "ilovepie";
char deviceID[] = "-L3VrcB_6ae38aKnzPB4";
RedkeaESP8266 redkea;
#define DSPIN 12
#define DHTPIN 14

double houseTemp = 0, boilerTemp = 0, houseHumidity = 0;
OneWire  ds(DSPIN);  // on pin 10 (a 4.7K resistor is necessary)
DallasTemperature sensors(&ds);
DHTesp dht;

/**************************************************************************************************
  Use the following snippet if you want to send data from your device to a widget.
  Replace "mySenderFunction" with the name of the user function you chose when you created the widget.
  (Don't change "widgetID".)
***************************************************************************************************/
/*
  REDKEA_SENDER(mySenderFunction, widgetID)
  {
    // use the following lines to send a string to a text widget

    // String str = ""; // assign the string you want to display
    // redkea.sendToTextWidget(widgetID, str);
  }
  REDKEA_REGISTER_SENDER(redkea, mySenderFunction)
*/
/**************************************************************************************************
  Use the following snippet if you want to receive data from a widget.
  Replace "myReceiverFunction" with the name of the user function you chose when you created the widget.
  (Don't change "args.")
***************************************************************************************************/
/*
  REDKEA_RECEIVER(myReceiverFunction, args)
  {
  // use this line to receive a value from a toggle widget
    // bool toggleState = redkea.readFromToggleWidget(args);


    // use this line to receive a value from a touch widget
    // bool touchState = redkea.readFromTouchWidget(args);


    // use this line to receive a value from a slider widget
    // int sliderValue = redkea.readFromSliderWidget(args);
  }
  REDKEA_REGISTER_RECEIVER(redkea, myReceiverFunction)
*/
REDKEA_RECEIVER(FurnaceSwitch, args){
  // use this line to receive a value from a toggle widget
  bool toggleState = ~redkea.readFromToggleWidget(args);
  digitalWrite(D4, toggleState);
  // use this line to receive a value from a touch widget
  // bool touchState = redkea.readFromTouchWidget(args);
  // use this line to receive a value from a slider widget
  // int sliderValue = redkea.readFromSliderWidget(args);
}
REDKEA_REGISTER_RECEIVER(redkea, FurnaceSwitch)
REDKEA_SENDER(GetBoilerWaterTemp, widgetID){
  // use the following lines to send a string to a text widget
  String str = (String)boilerTemp; // assign the string you want to display
  redkea.sendToTextWidget(widgetID, str);
}
REDKEA_REGISTER_SENDER(redkea, GetBoilerWaterTemp)
REDKEA_SENDER(BasementTemp, widgetID){
  // use the following lines to send a string to a text widget
  String str = (String)houseTemp; // assign the string you want to display
  redkea.sendToTextWidget(widgetID, str);
}
REDKEA_REGISTER_SENDER(redkea, BasementTemp)
REDKEA_SENDER(BasementHumidity, widgetID){
  // use the following lines to send a string to a text widget
  String str = (String)houseHumidity; // assign the string you want to display
  redkea.sendToTextWidget(widgetID, str);
}
REDKEA_REGISTER_SENDER(redkea, BasementHumidity)

void setup() {
  Serial.begin(115200);
  redkea.begin(ssid, pass, deviceID);
  sensors.begin();
  pinMode(D4, OUTPUT);  
  dht.setup(DHTPIN, DHTesp::DHT22);
}

void getDHTInfo(){
  // Reading temperature for humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
  TempAndHumidity newValues = dht.getTempAndHumidity();
  // Check if any reads failed and exit early (to try again).
  if (dht.getStatus() != 0) {
    Serial.println("DHT11 error status: " + String(dht.getStatusString()));
  } else {    
  houseHumidity = newValues.humidity; 
  houseTemp = Fahrenheit(newValues.temperature);
  Serial.print("Humidity (%): ");
  Serial.println((float)newValues.humidity, 2);
  Serial.print("Temperature (°F): ");
  Serial.println(Fahrenheit(newValues.temperature), 2);
  }
}

void getDSInfo() {
  sensors.requestTemperatures();
  Serial.print("\nDS18 Temperature (°F) is: ");
  boilerTemp = sensors.getTempFByIndex(0);
  Serial.println(boilerTemp);
}

void loop() {
  redkea.loop();
  getDSInfo();
  getDHTInfo();
}
