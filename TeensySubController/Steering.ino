void initializePowerSteering() {
  pinMode(powerSteeringPin, OUTPUT);
  //SendPSPulse();
}

// Call this every 4 wheel pulses
//void SendPSPulse(){
//  digitalWrite(powerSteeringPin, 1);
//  digitalWrite(powerSteeringPin, 0);
//}

void powerSteering() {
  //Use 3D map to map pulses based on speed and agressiveness setting   (Stock is 1 pulse per 40cm or 1 pulse every 0.000248548 mi )
  //X-axis will be speed        Y-axis will be 3 modes which will be selected based on rotory and screen
  //Use value called "powersteeringout"
  analogWrite(powerSteeringPin, powersteeringout);
#if (DEBUG)
  Serial.println("Power Steering Power: " + powersteeringout);
#endif
}


