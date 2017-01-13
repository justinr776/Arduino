#include "Outputs.h"
int slipcalc = 0;
int slipthresh = 10;

void initializeOutputs() {
  pinMode(clutchoutPin, OUTPUT);    //USE CANBUS IF POSSIBLE
  pinMode(tractionoutPin, OUTPUT);  //Out to the metro to alter throttle opening
  pinMode(tachoutPin, OUTPUT);      //Tach output
  pinMode(speedout1Pin, OUTPUT);    //Speedometer

  MCG_C1 = 0b01000110;
  //  MCG_C2 = 0b00000000;
  //  MCG_C3 = 0b00000000;
  //  MCG_C4 = 0b00000000;
  FTM0_MODE |= FTM_MODE_WPDIS; // Write Protection Disable
  FTM0_MODE |= FTM_MODE_FTMEN; // Flex Timer module enable
  FTM0_MODE |= FTM_MODE_INIT; // Perform init functions
  //  FTM0_ELS0B = 0x0044; // Channel Interrupt Enable & Rising Edge Input Capture

  FTM0_SC |= 0x0040; // Timer Overflow Interrupt Enable
  FTM0_SC |= FTM_SC_CLKS(0b10); // MCG Slow IRC
  FTM0_SC |= FTM_SC_PS(0b000); // 1 Prescaler (32kHz)
  FTM0_SC &= 0xFF5F; // Count Up Only & Clear TOF Flag
  FTM0_CNTIN = 0x0000; //Shouldn't be needed, but just in case
  FTM0_CNT = 0x0000; // Reset the count to zero
  FTM0_MOD = 0xFFFF; // max modulus = 65535
}

void FTM0_SpeedOut() {
  digitalWrite(tachoutPin, 1);
  digitalWrite(tachoutPin, 0);
}

void ClutchOut() {
  if (digitalRead(clutchPin) == LOW)  {
    digitalWrite(clutchoutPin, HIGH);
  }  else  {
    digitalWrite(clutchoutPin, LOW);
  }
}

void TractionOut() {
  if (slip > slipthresh) {  //Get to where TS can be used to set a value, or use a map   Maybe use rotary to set threshold
    slipcalc = 1 - (slip - slipthresh);
    analogWrite(DBWOut, slipcalc); //PWM out to DBW based on amount past threshold
  }
}

void TachOut() {
  // TODO Calculate the time for the interrupt to pulse the tach at based on the current rpm, set the timer count to value, and create ISR
  //byte countTo = 480000 / RPM;
  // Set Timer count to value to t to trigger interrupt
}

void SpeedOut() {
  analogWrite(speedout1Pin, speedavg);
}


void FTM0_ISR() {
  if (FTM0_C0SC & FTM_CSC_CHF) {
    FTM0_C0SC &= ~FTM_CSC_CHF;
  }
  if (FTM0_C1SC & FTM_CSC_CHF) {
    FTM0_C1SC &= ~FTM_CSC_CHF;
  }
  if (FTM0_C2SC & FTM_CSC_CHF) {
    FTM0_C2SC &= ~FTM_CSC_CHF;
  }
  if (FTM0_C3SC & FTM_CSC_CHF) {
    FTM0_C3SC &= ~FTM_CSC_CHF;
  }
  if (FTM0_C4SC & FTM_CSC_CHF) {
    FTM0_C4SC &= ~FTM_CSC_CHF;
  }
  if (FTM0_C5SC & FTM_CSC_CHF) {
    FTM0_C5SC &= ~FTM_CSC_CHF;
  }
  if (FTM0_C6SC & FTM_CSC_CHF) {
    FTM0_C6SC &= ~FTM_CSC_CHF;
  }
  if (FTM0_C7SC & FTM_CSC_CHF) {
    FTM0_C7SC &= ~FTM_CSC_CHF;
  }
}
