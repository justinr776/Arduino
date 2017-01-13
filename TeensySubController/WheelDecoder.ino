/*
  https://forum.arduino.cc/index.php?topic=126163.0
  https://www.pjrc.com/teensy/td_libs_Encoder.html
  https://forum.pjrc.com/threads/40783-Interrupt-vectors-for-Teensy-3-5
  http://www.pjrc.com/teensy/td_timing_IntervalTimer.html

  Timer interrupts on (24,25,26,27)
  44 teeth per revolution
*/
byte psPulse = 0;
int x[12][12] = {0};
void initializeWheelDecoder() {
  pinMode(rfPin, INPUT);
  pinMode(lfPin, INPUT);
  pinMode(rrPin, INPUT);
  pinMode(lrPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(rfPin), RFISR, HIGH);
  attachInterrupt(digitalPinToInterrupt(lfPin), LFISR, HIGH);
  attachInterrupt(digitalPinToInterrupt(rrPin), RRISR, HIGH);
  attachInterrupt(digitalPinToInterrupt(lrPin), LRISR, HIGH);
}

void Speedometer() {
  if (psPulse >= 8) {
    digitalWrite(speedout1Pin, 1);
    digitalWrite(speedout1Pin, 0);
    psPulse = 0;
  }
}

void RFISR() {
  rf++;
  psPulse++;
  Speedometer();
}
void LFISR() {
  lf++;
  psPulse++;
  Speedometer();
}
void RRISR() {
  rr++;
}
void LRISR() {
  lr++;
}

void ResetCounts() {
  rf = 0;
  lf = 0;
  rr = 0;
  lr = 0;
}



