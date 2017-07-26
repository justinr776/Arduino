#define WaterSensor 2 // Attach Water sensor to Arduino Digital Pin 8
#define LED 13 // Attach an LED to Digital Pin 9 (or use onboard LED)

void setup() {
  pinMode(WaterSensor, INPUT); // The Water Sensor is an Input
  pinMode(LED, OUTPUT); // The LED is an Output
}

void loop() {
  if ( digitalRead(WaterSensor) == LOW) //No Water
    digitalWrite(LED, LOW);
  else //Water
    digitalWrite(LED, HIGH);
}
