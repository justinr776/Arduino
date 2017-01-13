#include <avr/wdt.h>

int  MCEnable = 2, idleIn = 3, MCD2 = 4, MCSF = 8, MC1 = 9, MC2 = 10,
     oilPin = 13, MCAmpDraw = 14, mPPS = 16, mTPS = 17, sPPS = 18, sTPS = 19;
volatile int mTPSVal, sTPSVal, mPPSVal, sPPSVal, diff, idle, ampDraw;
int val = 150,  fin, idleOffset = 0, adjust = 0, cTPS = 0, adjPPSVal = 0,
    offset = 67, pHit = 770; //Was 780
bool error = false;
#define DEBUG 1

int commandedTPS[] = {
  291, 274, 257, 250, 246, 242, 240, 238, 236, 234, 232, 230,
  228, 226, 224, 222, 218, 212, 205, 198, 192, 186, 183, 180,
  177, 174, 171, 168, 165, 162, 159, 156, 153, 150, 147, 144,
  141, 138, 135, 132, 129, 126, 123, 120, 117, 114
};
int pedalToTPS[] = {
  870, 865, 860, 855, 850, 845, 840, 835, 830, 825, 820, 815, 810, 805, 800, 795,
  790, 785, 780, 775, 770, 765, 760, 755, 750, 745, 740, 735, 730, 725, 720, 715,
  710, 705, 700, 695, 690, 685, 680, 675, 670, 665, 660, 655, 650, 645, 640, 635,
  630, 625, 620, 615, 610, 605, 600, 595, 590, 585, 580, 575, 570, 565, 560, 555,
  550, 545, 540, 535, 530, 525, 520, 515, 510, 505, 500, 495, 490, 485, 480, 475,
  470, 465, 460, 455, 450, 445, 440, 435, 430, 425, 420, 415, 410, 405, 400, 395,
  390, 385, 380, 375, 370, 365, 360, 355, 350, 345, 340, 335, 330, 325, 320, 315,
  310, 305, 300, 295, 290, 285, 280, 275, 270, 265, 260, 255, 250, 245, 240, 235,
  230, 225, 220, 215, 210, 205, 200, 195, 190, 185, 180, 175, 170, 165, 160, 155, 150, 145, 140
};


void setup() {
  cli();
  wdt_reset();
  wdt_enable(WDTO_120MS);
//  MCUSR &= 0b11110111;
//  WDTCSR |= (1 << WDCE) | (1 << WDE);
//  WDTCSR = 0b00011100; // 32k cycles = ~.25s
  pinMode(mTPS, INPUT);
  pinMode(sTPS, INPUT);
  pinMode(mPPS, INPUT);
  pinMode(sPPS, INPUT);
  pinMode(MCAmpDraw, INPUT);
  pinMode(idleIn, INPUT_PULLUP);
  pinMode(MCSF, INPUT_PULLUP);
  pinMode(oilPin, INPUT_PULLUP);
  pinMode(MC1, OUTPUT);
  pinMode(MC2, OUTPUT);
  pinMode(MCEnable, OUTPUT);
  pinMode(MCD2, OUTPUT);

  digitalWrite(MCEnable, 1);
  digitalWrite(MCD2, 1);

  TCCR1A = (TCCR1A & 0x0F) | 0xE0; //Set OCR1A/B to be inverted of each other in normal PWM
  TCNT1 = 0; //Set Timer1's counter to 0
  TIFR1 = 0x0; //Reset all timer interrupt flags
#if (DEBUG)
  Serial.begin(250000);
#endif
  sei();
}

void TPS() { //Throttle Position Sensors
  mTPSVal = analogRead(mTPS);
  sTPSVal = analogRead(sTPS);
#if (DEBUG)
  Serial.print("\nMain TPS: ");
  Serial.print(mTPSVal);
  Serial.print("\tSub TPS: ");
  Serial.print(sTPSVal);
#endif
}

void PPS() { // Pedal Position Sensors
  mPPSVal = analogRead(mPPS);
  sPPSVal = analogRead(sPPS);
#if (DEBUG)
  Serial.print("\tMain PPS: ");
  Serial.print(mPPSVal);
  Serial.print("\tSub PPS: ");
  Serial.print(sPPSVal);
#endif
  //Adjust MC to reflect new position
  val = mPPSVal >> 3;
  if (mPPSVal < pHit) { // Pedal % high enough to kick out of idle
    fin = val + offset;
    if (mPPSVal > 793)
      adjPPSVal = 198; //793 >> 2
    else if (mPPSVal < 209)
      adjPPSVal = 52; //209 >> 2
    else
      adjPPSVal = mPPSVal >> 2;
    cTPS = pedalToTPS[adjPPSVal - 52]; ; //cTPS should be the equivalent mTPS position
  } else { // Idle
    fin = val + idleOffset;
    if (fin < 125) //Was 101
      fin = 125;
    else if (fin > 170)
      fin = 170;
    cTPS = commandedTPS[fin - 125]; //90 //cTPS should be the equivalent mTPS position
  }
  if (error)
    cTPS = 110;
  adjust = (mTPSVal - cTPS) >> 1;
#if (DEBUG)
  Serial.print("\nFinal Value: ");
  Serial.print(fin);
  Serial.print(" \tcTPS Value: ");
  Serial.print(cTPS); \
  Serial.print(" \tFinal Adj Value: ");
  Serial.print(adjust);
#endif
  fin += adjust;
  if (fin > 245)
    fin = 245;
  else if (fin < 40)
    fin = 40;
#if (DEBUG)
  Serial.print(" \tFinal MC: ");
  Serial.print(fin);
#endif
  writeMCVal(fin);
}

void writeMCVal(int val) {
  analogWrite(MC1, val);
  analogWrite(MC2, val);
}

int pComp, tComp;
void CheckPositions() {
  tComp = mTPSVal - sTPSVal;
  pComp = mPPSVal - sPPSVal;
  if (pComp > 0 || -60 > pComp)
    error = true;
  else if (tComp > 40 || -240 > tComp)
    error = true;
  else
    error = false;
}

void Idle() {
  idle = pulseIn(idleIn, HIGH);
  idleOffset = idle >> 5;
#if (DEBUG)
  Serial.print("\nIdle: ");
  Serial.print(idle);
  Serial.print(" \tIdle Offset: ");
  Serial.print(idleOffset);
#endif
}

void CheckMotorController() {
  if (digitalRead(MCSF) == LOW) {
    digitalWrite(MCD2, 0);
    digitalWrite(MCD2, 1);
  }
}

void ReadAmpDraw() {
  ampDraw = analogRead(MCAmpDraw);
  while (ampDraw > 50) {
    digitalWrite(MCD2, 0);
#if (DEBUG)
    Serial.print("\nOver Amp Draw:\t");
    Serial.print(ampDraw);
#endif
    digitalWrite(MCD2, 1);
    ampDraw = analogRead(MCAmpDraw);
  }
  digitalWrite(MCD2, 1);
#if (DEBUG)
  Serial.print("\nExit Amp Draw:");
  Serial.print(ampDraw);
#endif
}

void loop() {
#if (DEBUG)
  unsigned long start, finished, el;
  start = micros();
#endif
  digitalWrite(MCD2, 1);
  TPS();
  CheckPositions();
  ReadAmpDraw();
  PPS();
#if (DEBUG)
  finished = micros();
  el = finished - start;
  Serial.print(el);
  Serial.print("\n");
  Serial.print("\n");
#endif
  Idle();
  if (digitalRead(oilPin) == LOW)
    error = true;
  wdt_reset();
}
