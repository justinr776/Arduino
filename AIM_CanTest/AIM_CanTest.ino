#include <SPI.h>
#include "mcp_can.h"

const int SPI_CS_PIN = 9;
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_1000KBPS))              // Haltech uses 1,000Kbps aka 1Mbps
  {
    Serial.println("CAN BUS Failure...");
    delay(100);
  }
  Serial.println("CAN BUS is ready!");
}

unsigned char len = 0;
unsigned char buf[8];
uint16_t inExtV, inTwelveV, inFiveV, inSGNDV, inRPM, inIMAP, inEMAP, inTPSOverall, inTPS1, inLambda, inECT, inMAT, inOilT, inFuelT, inOilP,
         inFuelP, inDiffFuelP, inServoPos, inCoolantP, inEthanol, inVehicleSpeed, inGearNumber, inSpdDiffinFlagsLow, inFlagsHigh, inSlipLRGround,
         inKnockMax, inInj1Duty, inInj2Duty, inInj3Duty, inInj4Duty, inCalcChargTemp1, inStoichRatio, inTargetLambda, inFuelInjDurOut1,
         inFuelInjDurOut2, inIgnTiming, inAsyncInjDur1, inAsyncInjDur2, inIdleEffortCL, inUnclippedIdleEffort, inIdleEffortDuty,
         inCuttingCond, inCurrentRPMLimit, inPitlaneRPMLimit, inFuelCut, inIgnCut;

void loop() {

  if (CAN_MSGAVAIL == CAN.checkReceive())  {
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    unsigned char canId = CAN.getCanId();
    Serial.println("-----------------------------\nCAN ID: ");
    Serial.println(canId, HEX);
    for (int i = 0; i < len; i++) {
      Serial.print(buf[i], HEX);
      Serial.print("\t");
    }
    Serial.println();

    switch (canId) {
      case 304:
        inExtV = buf[0] << 8 + buf [1];
        inTwelveV = buf[2] << 8 + buf [3];
        inFiveV = buf[4] << 8 + buf [5];
        inSGNDV = buf[6] << 8 + buf [7];
        break;
      case 310:
        inRPM = buf[0] << 8 + buf [1];
        break;
      case 361:
        inIMAP = buf[0] << 8 + buf [1];
        inEMAP = buf[4] << 8 + buf [5];
        break;
      case 363:
        inTPSOverall = buf[0] << 8 + buf [1];
        inTPS1 = buf[2] << 8 + buf [3];
        break;
      case 364:
        inLambda = buf[2] << 8 + buf [3];
        inECT = buf[6] << 8 + buf [7];
        break;
      case 365:
        inMAT = buf[0] << 8 + buf [1];
        inOilT = buf[2] << 8 + buf [3];
        inFuelT = buf[4] << 8 + buf [5];
        inOilP = buf[6] << 8 + buf [7];
        break;
      case 366:
        inFuelP = buf[0] << 8 + buf [1];
        inDiffFuelP = buf[2] << 8 + buf [3];
        inServoPos = buf[4] << 8 + buf [5];
        inCoolantP = buf[6] << 8 + buf [7];
        break;
      case 367:
        inEthanol = buf[4] << 8 + buf [5];
        inVehicleSpeed = buf[6] << 8 + buf [7];
        break;
      case 368:
        inGearNumber = buf[0] << 8 + buf [1];
        inSpdDiff = buf[6] << 8 + buf [7];
        break;
      case 36C:
        inFlagsLow = buf[4] << 8 + buf [5];
        inFlagsHigh = buf[6] << 8 + buf [7];
        break;
      case 375:
        inSlipLRGround = buf[0] << 8 + buf [1];
        inKnockMax = buf[6] << 8 + buf [7];
        break;
      case 378:
        inInj1Duty = buf[0] << 8 + buf [1];
        inInj2Duty = buf[2] << 8 + buf [3];
        inInj3Duty = buf[4] << 8 + buf [5];
        inInj4Duty = buf[6] << 8 + buf [7];
        break;
      case 381:
        inCalcChargTemp1 = buf[0] << 8 + buf [1];
        inStoichRatio = buf[4] << 8 + buf [5];
        inTargetLambda = buf[6] << 8 + buf [7];
        break;
      case 38A:
        inFuelInjDurOut1 = buf[4] << 8 + buf [5];
        inFuelInjDurOut2 = buf[6] << 8 + buf [7];
        break;
      case 3A3:
        inIgnTiming = buf[0] << 8 + buf [1];
        break;
      case 3A5:
        inAsyncInjDur1 = buf[0] << 8 + buf [1];
        inAsyncInjDur2 = buf[4] << 8 + buf [5];
        break;
      case 3AA:
        inIdleEffortCL = buf[0] << 8 + buf [1];
        inUnclippedIdleEffort = buf[4] << 8 + buf [5];
        inIdleEffortDuty = buf[6] << 8 + buf [7];
        break;
      case 3B0:
        inCuttingCond = buf[0] << 8 + buf [1];
        inCurrentRPMLimit = buf[2] << 8 + buf [3];
        inPitlaneRPMLimit = buf[4] << 8 + buf [5];
        break;
      case 3B1:
        inFuelCut = buf[4] << 8 + buf [5];
        inIgnCut = buf[6] << 8 + buf [7];
        break;
    }
  }
}
