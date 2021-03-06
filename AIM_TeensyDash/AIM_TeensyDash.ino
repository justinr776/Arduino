#include <SPI.h>
#include <RA8875.h>
#include "mcp_can.h"
#include "fonts/squarefont_14.c"

const int SPI_CS_PIN = 10;
//MCP_CAN CAN(SPI_CS_PIN);
/*
  Arduino's
  You are using 4 wire SPI here, so:
  MOSI:  11//Arduino UNO
  MISO:  12//Arduino UNO
  SCK:   13//Arduino UNO
  the rest of pin below:
*/
#define RA8875_CS 5// 10 //see below...
#define RA8875_RESET 0//any pin or nothing!

RA8875 tft = RA8875(RA8875_CS, RA8875_RESET);
void setup() {
  Serial.begin(115200);
  Serial.println("Starting");

  long unsigned debug_start = millis();
  tft.begin(RA8875_800x480);
  tft.fillWindow(RA8875_BLACK);
  tft.setTextColor(RA8875_WHITE);
  tft.setFont(&squarefont_14);
  tft.showCursor(NOCURSOR, false);
  tft.setFontScale(2);
  tft.setCursor(5, 235);
  tft.print("OILP");
  tft.setCursor(5, 330);
  tft.print("FUELP");
  tft.setCursor(5, 425);
  tft.print("VOLT");
  tft.setCursor(675, 235);
  tft.print("COOL");
  tft.setCursor(675, 330);
  tft.print("OILT");
  tft.setCursor(675, 425);
  tft.print("FLEX");
  tft.setCursor(766, 390);
  tft.print("%");
  tft.setCursor(320, 425);
  tft.print("ODO");
  tft.setCursor(525, 425);
  tft.print("MPG");
  tft.setCursor(315, 325);
  tft.print("MAP");
  tft.brightness(125);
}
uint16_t  RPM = 1000, PRPM = 0, TPSOverall = 0, TPS1 = 0, DiffFuelP = 0, ServoPos = 0, InjFlowRate = 0,
          CoolantP = 0, VehicleSpeed = 0, GearNumber = 0, SpdDiff = 0, FlagsLow = 0, FlagsHigh = 0,
          SlipLRGround = 0, KnockMax = 0, Inj1Duty = 0, Inj2Duty = 0, Inj3Duty = 0, Inj4Duty = 0, CalcChargTemp1 = 0,
          StoichRatio = 0, TargetLambda = 0, FuelInjDurOut1 = 0, FuelInjDurOut2 = 0, IgnTiming = 0,
          AsyncInjDur1 = 0, AsyncInjDur2 = 0, IdleEffortCL = 0, UnclippedIdleEffort = 0, IdleEffortDuty = 0,
          CuttingCond = 0, CurrentRPMLimit = 0, PitlaneRPMLimit = 0, FuelCut = 0, IgnCut = 0, FuelL = 50;
uint16_t ExtV = 0, FiveV = 0, SGNDV = 0,  IMAP = 0, EMAP = 0, ECT = 0, MAT = 0,
         OilT = 0, FuelT = 0, OilP = 0, FuelP = 0, Ethanol = 0;
float Lambda = 1, PLambda = 0,  TwelveV = 0, miles = 0;
boolean bExtV = false, bTwelveV = false, bFiveV = false, bSGNDV = false, bRPM = false, bIMAP = false, bEMAP = false,
        bTPSOverall = false, bTPS1 = false, bLambda = false, bECT = false, bMAT = false, bOilT = false, bFuelT = false,
        bOilP = false, bFuelP = false, bDiffFuelP = false, bServoPos = false, bCoolantP = false, bEthanol = false, bInjFlowRate = false,
        bVehicleSpeed = false, bGearNumber = false, bSpdDiff = false, bFlagsLow = false, bFlagsHigh = false, bSlipLRGround = false,
        bKnockMax = false, bInj1Duty = false, bInj2Duty = false, bInj3Duty = false, bInj4Duty = false, bCalcChargTemp1 = false,
        bStoichRatio = false, bTargetLambda = false, bFuelInjDurOut1 = false, bFuelInjDurOut2 = false, bIgnTiming = false,
        bAsyncInjDur1 = false, bAsyncInjDur2 = false, bIdleEffortCL = false, bUnclippedIdleEffort = false, bIdleEffortDuty = false,
        bCuttingCond = false, bCurrentRPMLimit = false, bPitlaneRPMLimit = false, bFuelCut = false, bIgnCut = false, bFuelL = false;
int f1 = 26, f2 = 45, f3 = 63;

//CAN In Variables
unsigned char len = 0;
unsigned char buf[8];
uint16_t inExtV, inTwelveV, inFiveV, inSGNDV, inRPM, inIMAP, inEMAP, inTPSOverall, inTPS1, inLambda, inECT, inMAT, inOilT, inFuelT, inOilP,
         inFuelP, inDiffFuelP, inServoPos, inCoolantP, inEthanol, inVehicleSpeed, inGearNumber, inSpdDiff, inFlagsLow, inFlagsHigh, inSlipLRGround,
         inKnockMax, inInj1Duty, inInj2Duty, inInj3Duty, inInj4Duty, inCalcChargTemp1, inStoichRatio, inTargetLambda, inFuelInjDurOut1,
         inFuelInjDurOut2, inIgnTiming, inAsyncInjDur1, inAsyncInjDur2, inIdleEffortCL, inUnclippedIdleEffort, inIdleEffortDuty,
         inCuttingCond, inCurrentRPMLimit, inPitlaneRPMLimit, inFuelCut, inIgnCut;
uint16_t  rawIMAP = 0, rawEMAP = 0, rawTPSOverall = 0, rawTPS1 = 0, rawTwelve, rawLambda,
          rawECT = 0, rawMAT = 0, rawOilT = 0, rawFuelT = 0, rawOilP = 0, rawFuelP = 0, rawDiffFuelP = 0, rawServoPos = 0,
          rawCoolantP = 0, rawEthanol = 0, rawVehicleSpeed = 0, rawGearNumber = 0, rawSpdDiff = 0,
          rawSlipLRGround = 0, rawKnockMax = 0, rawInj1Duty = 0, rawInj2Duty = 0, rawInj3Duty = 0, rawInj4Duty = 0, rawCalcChargTemp1 = 0,
          rawStoichRatio = 0, rawTargetLambda = 0, rawFuelInjDurOut1 = 0, rawFuelInjDurOut2 = 0, rawIgnTiming = 0,
          rawAsyncInjDur1 = 0, rawAsyncInjDur2 = 0, rawIdleEffortCL = 0, rawUnclippedIdleEffort = 0, rawIdleEffortDuty = 0,
          rawCuttingCond = 0, rawCurrentRPMLimit = 0, rawPitlaneRPMLimit = 0, rawFuelCut = 0, rawIgnCut = 0, rawFuelL = 50;

void UpdateDisplay() {
  if (bRPM) {
    int16_t val = (int16_t)(0.1143 * RPM);
    if (PRPM > RPM) {
      tft.fillRect(val, 0, 800 - val, 100, RA8875_BLACK);
      if (RPM < 5000 && PRPM > 5000)
        tft.fillRect(0, 0, val, 100, RA8875_GREEN);
      else if (RPM < 6500 && PRPM > 6500)
        tft.fillRect(0, 0, val, 100, RA8875_YELLOW);
    }
    else {
      uint16_t color = RPM > 6500 ? RA8875_RED : RPM > 5000 ? RA8875_YELLOW : RA8875_GREEN;
      tft.fillRect(0, 0, val, 100, color);
    }
    tft.setCursor(270, 75);
    tft.setFontScale(7);
    tft.fillRect(278, 107, 240, 72, RA8875_BLACK);
    tft.print(RPM);
    PRPM = RPM;
    bRPM = false;
  }
  if (bVehicleSpeed) {
    tft.setCursor(65, 83);
    tft.setFontScale(6);
    tft.fillRect(72, 111, 150, f3, RA8875_BLACK);
    tft.print(VehicleSpeed);
    bVehicleSpeed = false;
  }
  if (bLambda) { //.7 to 1.2
    tft.setCursor(600, 83);
    tft.setFontScale(6);
    tft.fillRect(607, 111, 175, f3, RA8875_BLACK);
    tft.print(Lambda);
    if (Lambda > 1.2)
      Lambda = 1.2;
    if (Lambda < .7)
      Lambda = .7;
    if (Lambda > 1) { //Draw red to the right
      if (PLambda < 1)
        tft.fillRect(200, 200, 200, 50, RA8875_BLACK); //Draw other side black
      int val = (Lambda - 1.0) * 1000;
      if (PLambda < Lambda)
        tft.fillRect(400, 200, val, 50, RA8875_RED); //Draw line out
      else
        tft.fillRect(400 + val, 200, 200 - val, 50, RA8875_BLACK); //Draw line in
    }
    else if (Lambda < 1) { //  Draw Blue to the left
      int val = (Lambda - .7) * 666;
      if (PLambda > 1)
        tft.fillRect(400, 200, 200, 50, RA8875_BLACK); //Draw other side black
      if (PLambda < Lambda)
        tft.fillRect(200, 200, val, 50, RA8875_BLACK); //Draw line in
      else
        tft.fillRect(200 + val, 200, 200 - val, 50, RA8875_BLUE); //Draw line outwards
    }
    else
      tft.fillRect(200, 200, 400, 50, RA8875_BLACK);
    PLambda = Lambda;
    bLambda = false;
  }
  if (bOilP) {
    tft.setCursor(5, 175);
    tft.setFontScale(4);
    tft.fillRect(10, 195, 110, f2, OilP < 36 ? RA8875_RED : RA8875_BLACK); // 206
    tft.print(OilP);
    bOilP = false;
  }
  if (bIMAP) {
    tft.setCursor(315, 265);
    tft.setFontScale(4);
    tft.fillRect(320, 285, 90, f2, RA8875_BLACK);
    tft.print(IMAP);
  }
  if (bFuelP) {
    tft.setCursor(5, 270);
    tft.setFontScale(4);
    tft.fillRect(10, 290, 110, f2, FuelP < 48 ? RA8875_RED : RA8875_BLACK); //275
    tft.print(FuelP);
    bFuelP = false;
  }
  if (bTwelveV) {
    tft.setCursor(5, 365);
    tft.setFontScale(4);
    tft.fillRect(10, 385, 145, f2, RA8875_BLACK);
    tft.print(TwelveV);
    bTwelveV = false;
  }
  if (bFuelL) { //220, 280, 70w, 200
    int val = 200 - (FuelL << 1);
    tft.fillRect(220, 280, 70, val, RA8875_WHITE);
    tft.fillRect(220, 280 + val, 70, 200 - val, RA8875_GREEN);
    tft.setCursor(215, 365);
    tft.setFontScale(4);
    tft.setTextColor(RA8875_BLACK);
    tft.print(FuelL);
    tft.setTextColor(RA8875_WHITE);
    bFuelL = false;
  }
  if (bECT) {
    tft.setCursor(675, 175);
    tft.setFontScale(4);
    tft.fillRect(680, 195, 110, f2, ECT > 208 ? RA8875_RED : RA8875_BLACK);//104
    tft.print(ECT);
    bECT = false;
  }
  if (bOilT) {
    tft.setCursor(675, 270);
    tft.setFontScale(4);
    tft.fillRect(680, 290, 110, f2, OilT > 200 ? RA8875_RED : RA8875_BLACK); //120
    tft.print(OilT);
    bOilT = false;
  }
  if (bEthanol) {
    tft.setCursor(675, 365);
    tft.setFontScale(4);
    tft.fillRect(680, 385, 90, f2, RA8875_BLACK);
    tft.print(Ethanol);
    bEthanol = false;
  }
}

void UpdateMiles() {
  tft.setCursor(315, 365);
  tft.setFontScale(4);
  tft.fillRect(320, 385, 150, f2, RA8875_BLACK);
  tft.print(miles);
}

void UpdateMPG(float MPG) {
  if (!(MPG > 0 && MPG < 250))
    MPG = 0;
  tft.setCursor(465, 365);
  tft.setFontScale(4);
  tft.fillRect(470, 385, 210, f2, RA8875_BLACK);
  tft.print(MPG);
}

boolean Lup = true;
void SetTestValues() {
  bFuelP = true;
  FuelP = 585 / 10;
  bFuelL = true;
  FuelL = 50;
  bIMAP = true;
  IMAP = 50.55;
  bEthanol = true;
  Ethanol = 22.55;
  bECT = true;
  ECT = 225;
  bTwelveV = true;
  TwelveV = 12.62;
  bOilT = true;
  OilT = 225;
  bOilP = true;
  bLambda = true;
  InjFlowRate = 120;
  if (Lambda >= 1.2)
    Lup = false;
  else if (Lambda <= .7)
    Lup = true;
  if (Lup)
    Lambda += .01;
  else
    Lambda -= .01;
  bRPM = true;
  bVehicleSpeed = true;
  RPM += 200;
  if (RPM > 7000)
    RPM = 500;
  VehicleSpeed = RPM / 100;
  OilP = 60;
}

unsigned long start = millis();
void loop() {
  //  if (CAN_MSGAVAIL == CAN.checkReceive()) {
  //    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
  //    unsigned char canId = CAN.getCanId();
  //    //    Serial.println("-----------------------------\nCAN ID: ");
  //    //    Serial.println(canId, HEX);
  //    //    for (int i = 0; i < len; i++) {
  //    //      Serial.print(buf[i], HEX);
  //    //      Serial.print("\t");
  //    //    }
  //    //    Serial.println();
  //    switch (canId) {
  //      case 0x04:
  //        inExtV = buf[0] << 8 + buf[1];
  //        inTwelveV = buf[2] << 8 + buf[3];
  //        if (rawTwelve != inTwelveV) {
  //          TwelveV = inTwelveV / 1000;
  //          bTwelveV = true;
  //          rawTwelve = inTwelveV;
  //        }
  //        inFiveV = buf[4] << 8 + buf[5];
  //        inSGNDV = buf[6] << 8 + buf[7];
  //        break;
  //      case 0x10:
  //        inRPM = buf[0] << 8 + buf[1];
  //        if (inRPM != RPM) {
  //          bRPM = true;
  //          RPM = inRPM;
  //        }
  //        break;
  //      case 0x61:
  //        inIMAP = buf[0] << 8 + buf[1];
  //        if (rawIMAP != inIMAP) {
  //          IMAP = inIMAP / 10;
  //          bIMAP = true;
  //          rawIMAP = inIMAP;
  //        }
  //        inEMAP = buf[4] << 8 + buf[5];
  //        break;
  //      case 0x63:
  //        inTPSOverall = buf[0] << 8 + buf[1];
  //        inTPS1 = buf[2] << 8 + buf[3];
  //        break;
  //      case 0x64:
  //        inLambda = buf[2] << 8 + buf[3];
  //        if (rawLambda != inLambda) {
  //          Lambda = inLambda / 1000;
  //          bLambda = true;
  //          rawLambda = inLambda;
  //        }
  //        inECT = buf[6] << 8 + buf[7];
  //        if (ECT != inECT) {
  //          ECT = inECT / 10;
  //          bECT = true;
  //          ECT = inECT;
  //        }
  //        break;
  //      case 0x65:
  //        inMAT = buf[0] << 8 + buf[1];
  //        inOilT = buf[2] << 8 + buf[3];
  //        if (rawOilT != inOilT) {
  //          OilT = inOilT / 10;
  //          bOilT = true;
  //          rawOilT = inOilT;
  //        }
  //        inFuelT = buf[4] << 8 + buf[5];
  //        inOilP = buf[6] << 8 + buf[7];
  //        if (rawOilP != inOilP) {
  //          OilP = inOilP / 10;
  //          bOilP = true;
  //          rawOilP = inOilP;
  //        }
  //        break;
  //      case 0x66:
  //        inFuelP = buf[0] << 8 + buf[1];
  //        if (rawFuelP != inFuelP) {
  //          FuelP = inFuelP / 10;
  //          bFuelP = true;
  //          rawFuelP = inFuelP;
  //        }
  //        inDiffFuelP = buf[2] << 8 + buf[3];
  //        inServoPos = buf[4] << 8 + buf[5];
  //        inCoolantP = buf[6] << 8 + buf[7];
  //        break;
  //      case 0x67:
  //        inEthanol = buf[4] << 8 + buf[5];
  //        if (rawEthanol != inEthanol) {
  //          Ethanol = inEthanol / 10;
  //          bEthanol = true;
  //          rawEthanol = inEthanol;
  //        }
  //        inVehicleSpeed = buf[6] << 8 + buf[7];
  //        if (rawVehicleSpeed != inVehicleSpeed) {
  //          VehicleSpeed = inVehicleSpeed / 10;
  //          bVehicleSpeed = true;
  //          rawVehicleSpeed = inVehicleSpeed;
  //        }
  //        break;
  //      case 0x68:
  //        inGearNumber = buf[0] << 8 + buf[1];
  //        inSpdDiff = buf[6] << 8 + buf[7];
  //        break;
  //      case 0x6C:
  //        inFlagsLow = buf[4] << 8 + buf[5];
  //        if (FlagsLow != inFlagsLow) {
  //          FlagsLow = inFlagsLow;
  //          bFlagsLow = true;
  //        }
  //        inFlagsHigh = buf[6] << 8 + buf[7];
  //        if (FlagsHigh != inFlagsHigh) {
  //          FlagsHigh = inFlagsHigh;
  //          bFlagsHigh = true;
  //        }
  //        break;
  //      case 0x75:
  //        inSlipLRGround = buf[0] << 8 + buf[1];
  //        inKnockMax = buf[6] << 8 + buf[7];
  //        break;
  //      case 0x78:
  //        inInj1Duty = buf[0] << 8 + buf[1];
  //        inInj2Duty = buf[2] << 8 + buf[3];
  //        inInj3Duty = buf[4] << 8 + buf[5];
  //        inInj4Duty = buf[6] << 8 + buf[7];
  //        break;
  //      case 0x81:
  //        inCalcChargTemp1 = buf[0] << 8 + buf[1];
  //        inStoichRatio = buf[4] << 8 + buf[5];
  //        inTargetLambda = buf[6] << 8 + buf[7];
  //        break;
  //      case 0x8A:
  //        inFuelInjDurOut1 = buf[4] << 8 + buf[5];
  //        inFuelInjDurOut2 = buf[6] << 8 + buf[7];
  //        break;
  //      case 0xA3:
  //        inIgnTiming = buf[0] << 8 + buf[1];
  //        break;
  //      case 0xA5:
  //        inAsyncInjDur1 = buf[0] << 8 + buf[1];
  //        inAsyncInjDur2 = buf[4] << 8 + buf[5];
  //        break;
  //      case 0xAA:
  //        inIdleEffortCL = buf[0] << 8 + buf[1];
  //        inUnclippedIdleEffort = buf[4] << 8 + buf[5];
  //        inIdleEffortDuty = buf[6] << 8 + buf[7];
  //        break;
  //      case 0xB0:
  //        inCuttingCond = buf[0] << 8 + buf[1];
  //        inCurrentRPMLimit = buf[2] << 8 + buf[3];
  //        inPitlaneRPMLimit = buf[4] << 8 + buf[5];
  //        break;
  //      case 0xB1:
  //        inFuelCut = buf[4] << 8 + buf[5];
  //        inIgnCut = buf[6] << 8 + buf[7];
  //        break;
  //    }
  //  }
  //  if (millis() - start > 495) {
  //    start = millis();
  //    float temp = VehicleSpeed * 0.0001388;
  //    miles += temp;
  //    UpdateMiles();
  //    UpdateMPG(temp / (InjFlowRate * 0.0000022));
  //  }
  delay(10);
  SetTestValues();
  UpdateDisplay();
}

