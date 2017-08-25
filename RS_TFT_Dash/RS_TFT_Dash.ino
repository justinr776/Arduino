#include <SPI.h>
#include <RA8875.h>
/*
  Arduino's
  You are using 4 wire SPI here, so:
  MOSI:  11//Arduino UNO
  MISO:  12//Arduino UNO
  SCK:   13//Arduino UNO
  the rest of pin below:
*/
#define RA8875_CS 10 //see below...
/*
  Arduino's 8 bit: any
*/
#define RA8875_RESET 9//any pin or nothing!

RA8875 tft = RA8875(RA8875_CS, RA8875_RESET); //Teensy3/arduino's

void setup() {
  long unsigned debug_start = millis ();
  tft.begin(RA8875_800x480);
  tft.fillWindow(RA8875_BLACK);//fill window black
  tft.setTextColor(RA8875_WHITE);
  //tft.print("Hello World");
  //  tft.setCursor(50, 20); //set cursor work in pixel!!!
  //  tft.setTextColor(RA8875_RED, RA8875_GREEN);
  //  tft.print("Hello World");
  //  tft.setFontScale(1);//font x1
  //  int16_t currentX, currentY;
  //  tft.getCursor(currentX, currentY);
  //  //now we have the location, lets draw a white pixel
  //  tft.drawPixel(currentX, currentY, RA8875_WHITE); //did you see the white dot?
  //  tft.setFontScale(0);//font x0
  //  tft.setCursor(0, 50);
  //  tft.setTextColor(RA8875_YELLOW);
  //  tft.println("ABCDEF 1 2 3 4");//this time println!
  //  tft.setFontSpacing(5);//now give 5 pix spacing
  //  tft.setFontSpacing(0);//reset
  //  tft.setCursor(CENTER, CENTER); //this set text exact in the screen regardless his lenght and scale
  //  //now use the autocenter feature
  //  tft.drawRect((tft.width() / 2) - 25, (tft.height() / 2) - 100, 50, 50, RA8875_RED);
  //  tft.setTextColor(RA8875_YELLOW);
  //  //this will center the text
  //  tft.setCursor((tft.width() / 2), (tft.height() / 2) - 75, true);
  //  tft.showCursor(IBEAM, true); //activate cursor iBeam blinking
  //  tft.showCursor(UNDER, true); //activate cursor iBeam blinking
  //  tft.showCursor(BLOCK, true); //activate cursor iBeam blinking
  //  tft.showCursor(NOCURSOR, false); //deactivate cursor
  //  tft.setFontScale(0, 3); //font x0,x4
  //  //here's another unusual command, setFontAdvance enable/disable font advance
  //  //so you don't have to use setCursor a lot when you need to update numbers on screen
  //  tft.cursorIncrement(false);//turn off autoAdvance
  //  tft.print(" ");
  //  tft.cursorIncrement(true);//back to normal
  //  tft.setFontScale(2);//font x3

}

uint16_t ExtV = 0, TwelveV = 0, FiveV = 0, SGNDV = 0, RPM = 1000, IMAP = 0, EMAP = 0, TPSOverall = 0, TPS1 = 0,
         Lambda = 0, ECT = 0, MAT = 0, OilT = 0, FuelT = 0, OilP = 0, FuelP = 0, DiffFuelP = 0, ServoPos = 0,
         CoolantP = 0, Ethanol = 0, VehicleSpeed = 0, GearNumber = 0, SpdDiffinFlagsLow = 0, FlagsHigh = 0,
         SlipLRGround = 0, KnockMax = 0, Inj1Duty = 0, Inj2Duty = 0, Inj3Duty = 0, Inj4Duty = 0, CalcChargTemp1 = 0,
         StoichRatio = 0, TargetLambda = 0, FuelInjDurOut1 = 0, FuelInjDurOut2 = 0, IgnTiming = 0,
         AsyncInjDur1 = 0, AsyncInjDur2 = 0, IdleEffortCL = 0, UnclippedIdleEffort = 0, IdleEffortDuty = 0,
         CuttingCond = 0, CurrentRPMLimit = 0, PitlaneRPMLimit = 0, FuelCut = 0, IgnCut = 0;
boolean bExtV = false, bTwelveV = false, bFiveV = false, bSGNDV = false, bRPM = false, bIMAP = false, bEMAP = false,
        bTPSOverall = false, bTPS1 = false, bLambda = false, bECT = false, bMAT = false, bOilT = false, bFuelT = false,
        bOilP = false, bFuelP = false, bDiffFuelP = false, bServoPos = false, bCoolantP = false, bEthanol = false,
        bVehicleSpeed = false, bGearNumber = false, bSpdDiffinFlagsLow = false, bFlagsHigh = false, bSlipLRGround = false,
        bKnockMax = false, bInj1Duty = false, bInj2Duty = false, bInj3Duty = false, bInj4Duty = false, bCalcChargTemp1 = false,
        bStoichRatio = false, bTargetLambda = false, bFuelInjDurOut1 = false, bFuelInjDurOut2 = false, bIgnTiming = false,
        bAsyncInjDur1 = false, bAsyncInjDur2 = false, bIdleEffortCL = false, bUnclippedIdleEffort = false, bIdleEffortDuty = false,
        bCuttingCond = false, bCurrentRPMLimit = false, bPitlaneRPMLimit = false, bFuelCut = false, bIgnCut = false;

void UpdateDisplay() {
  if (bRPM) {
    uint16_t color = RPM > 6500 ? RA8875_RED : RPM > 5000 ? RA8875_MAGENTA : RA8875_GREEN;
    int16_t val = (int16_t)(0.1143 * RPM);
    tft.fillRect(val, 0, 800-val, 100, RA8875_BLACK);
    tft.fillRect(0, 0, val, 100, color);
    tft.setCursor(290, 100);
    tft.setFontScale(3);
    tft.fillRect(290, 100, 125, 60, RA8875_BLACK);
    tft.print(RPM);
    bRPM = false;
  }
  if (bVehicleSpeed) {
    tft.setCursor(45, 125);
    tft.setFontScale(2);
    tft.fillRect(45, 125, 100, 50, RA8875_BLACK);
    tft.print(VehicleSpeed);
    bVehicleSpeed = false;
  }  
  if (bOilP) {
    tft.setCursor(45, 175);
    tft.setFontScale(2);
    tft.fillRect(45, 175, 100, 50, RA8875_BLACK);
    tft.print(OilP);
    bOilP = false;
  }
  if (bLambda) {
    tft.setCursor(290, 175);
    tft.setFontScale(2);
    tft.fillRect(290, 175, 100, 50, RA8875_BLACK);
    tft.print(Lambda);
    bLambda = false;
  }
  if (bTwelveV) {
    tft.setCursor(590, 175);
    tft.setFontScale(2);
    tft.fillRect(590, 175, 100, 50, RA8875_BLACK);
    tft.print(TwelveV);
    bTwelveV = false;
  }
  if (bFuelP) {
    tft.setCursor(590, 225);
    tft.setFontScale(2);
    tft.fillRect(590, 225, 100, 50, RA8875_BLACK);
    tft.print(FuelP);
    bFuelP = false;
  }
  if (bOilT) {
    tft.setCursor(45, 235);
    tft.setFontScale(2);
    tft.fillRect(45, 235, 100, 50, RA8875_BLACK);
    tft.print(OilT);
    bOilT = false;
  }
  if (bOilP) {
    tft.setCursor(45, 175);
    tft.setFontScale(2);
    tft.fillRect(45, 175, 100, 50, RA8875_BLACK);
    tft.print(OilP);
    bOilP = false;
  }
  if (bECT) {
    tft.setCursor(45, 175);
    tft.setFontScale(2);
    tft.fillRect(45, 175, 100, 50, RA8875_BLACK);
    tft.print(ECT);
    bECT = false;
  }
  if (bEthanol) {
    tft.setCursor(45, 250);
    tft.setFontScale(2);
    tft.fillRect(45, 250, 100, 50, RA8875_BLACK);
    tft.print(Ethanol);
    bEthanol = false;
  }
}

void SetTestValues(){  
  bOilP = true;
  bRPM = true;
  bVehicleSpeed = true;
  RPM += 100;
  if (RPM > 7000)
    RPM = 500;
  VehicleSpeed = RPM/100;
  OilP = 60;
}

void loop() {
  delay(100);
  SetTestValues();
  UpdateDisplay();
}
