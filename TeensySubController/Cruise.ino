/********************************************************
   PID Adaptive Tuning Example
   One of the benefits of the PID library is that you can
   change the tuning parameters at any time.  this can be
   helpful if we want the controller to be aggressive at some
   times, and conservative at others.   in the example below
   we set the controller to use Conservative Tuning Parameters
   when we're near setpoint and more aggressive Tuning
   Parameters when we're farther away.
 ********************************************************/
//Still need output
#include "PID_v1.h"

//Define Variables we'll be connecting to
double Setpoint, ThrottleOut;   //Change to match needed

//Define the aggressive and conservative Tuning Parameters
double aggKp = 4, aggKi = 0.2, aggKd = 1;
double consKp = 1, consKi = 0.05, consKd = 0.25;
double pidOut = 0;

//Specify the links and initial tuning parameters
PID myPID(&speedavg, &pidOut, &Setpoint, consKp, consKi, consKd, DIRECT);

void initializeCruise() {
  //initialize the variables we're linked to
  Setpoint = SetSpeed;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void cruise() {
  if (clutch == HIGH && brake == HIGH) {
    double gap = abs(Setpoint - speedavg); //distance away from setpoint
    if (gap < 10) { //we're close to setpoint, use conservative tuning parameters
      myPID.SetTunings(consKp, consKi, consKd);
    } else {
      //we're far from setpoint, use aggressive tuning parameters
      myPID.SetTunings(aggKp, aggKi, aggKd);
    }
    myPID.Compute();
    analogWrite(127, ThrottleOut);  //Set up proper output
  }
}
