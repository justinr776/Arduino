//#include "Globals.h"
//#include "Wheel_Decoders.h"
void Speed();
void Slip();

void WheelOperations() {
  rfVal = rf;
  lfVal = lf;
  rrVal = rr;
  lrVal = lr;
#if (DEBUG)
  Serial.print("RF: ");
  Serial.print(rf);
  Serial.print("LF: ");
  Serial.print(lf);
  Serial.print("RR: ");
  Serial.print(rr);
  Serial.print("LR: ");
  Serial.print(lr);
#endif
  ResetCounts();
  Speed();
  Slip();
}

void Speed() {
  favg = (rfVal + lfVal) / 2;     //Calculate average front speed
  ravg = (rrVal + lrVal) / 2;     //Calculate average rear speed
  speedavg = (favg + ravg) / 2; //Calculate average wheel speed
}

void Slip() {
  slip = abs(favg / ravg - 1);  //Calculate slip
}

