/*
  PWM Output is 100Hz and 10-50%
  Calculated using Handbrake Toggle, %Slip, %TPS, Accelerometer Values, Braking, Left foot Braking, and Speed
  Left Foot Braking Toggled by %TPS
  DCCD Fully Open if Handbrake ON
  Final=((%Slip_%TPS_RPM Map) OR Braking Map OR Left Foot Braking Map)+(Accelerometer_Speed Map)

  Feature where when launch is engaged, DCCD goes to full lock
*/
#include "DCCD.h"

void initializeDCCD() {
  pinMode(DCCDout, OUTPUT);         //DCCD output (100Hz and 0-50% DC)
  analogWriteFrequency(29, 100);
}

void DCCDOut() {
  analogWrite(DCCDout, finaldccdpower);
}

void DCCDControl() {
  if (handbrake == LOW) { //Checking state of handbrake
    finaldccdpower = 0;
  } else if (clutch == LOW && Launch == HIGH) { //Check for launch conditions
    finaldccdpower = 127; //PWM=50% for full lock on launch with timout on release
  } else if (dccdStatus == 1) { //Check for manual DCCD input
    finaldccdpower = mDCCDpower;//Use manual DCCD value
  } else if (brake == LOW) { //Checking Brake Pedal
    if (TPS > 20) {
      ; //Use Left Foot Brake Map + Accelerometer_Speed Map
    }   else {
      ;//Use Brake Map + Accelerometer_Speed Map
    }
    finaldccdpower = 127;
  } else {
    ;//Use %Slip_%TPS_RPM Map+ Accelerometer_Speed Map
  }

  if (TC == 1)
    finaldccdpower = finaldccdpower * (1 + slip); //Use slip as a multiplier when traction control is on.

  if (finaldccdpower > 127)
    finaldccdpower = 127;
  if (finaldccdpower < 26)
    finaldccdpower = 26;
    DCCDOut();
}
