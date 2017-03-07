/*
  http://www.pjrc.com/teensy/K66P144M180SF5RMV2.pdf    Main datasheet
  https://forum.pjrc.com/threads/40173-FTM-Channel-compare-interrupts-(Teensy-3-5)   Timer info
*/
#define DEBUG 1
#include <math.h>
#include "Globals.h"
#include <Wire.h>
#include "CAN.h"
#include "DCCD.h"
#include "Display.h"
#include "Outputs.h"
#include "Steering.h"
#include "Sensors.h"
#include "ShiftLight.h"
#include "WheelDecoder.h"
#include "Speed.h"
#include "Shiftlight.h"
#include "Knock.h"
#include "Alternator.h"

void setup() {
#if (DEBUG)
  Serial.begin(250000);
  while (!Serial) ; // wait for Arduino Serial Monitor
  Serial.println("Teensy 3.5\n");
#endif
  //initializeSensors();
  //initializeOutputs();
  //initializeDCCD();
  initializeCANbus();
  //initializeDisplay();
  initializeWheelDecoder();
  initializeAlternator();
  //check_first_run();
  //buildarrays();
#if (DEBUG)
  Serial.println("End of Setup\n");
#endif
}

unsigned long timing;
void loop() {
  // TODO Set this up to run all necessary functions
#if (DEBUG)
  timing = micros();
#endif
  CANLoop();
  //UpdateDisplay();
  //ShiftLightLoop();
  //DCCDControl();
  //readSensors();
  WheelOperations();
  SetAlternatorPWM(BATTV);
#if (DEBUG)
  timing = micros() - timing;
  Serial.print("Timing: " + timing);
#endif
}
