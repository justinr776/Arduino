#include <SPI.h>
#include "mcp_can.h"
#include "Wire.h"
#define debug 1
const int SPI_CS_PIN = 9;
const int FUELPIN = A0;
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  pinMode(INPUT_PULLUP, FUELPIN);

#if debug
  Serial.begin(115200);
#endif
  while (CAN_OK != CAN.begin(CAN_1000KBPS))              // Haltech uses 1,000Kbps aka 1Mbps
  {
#if debug
    Serial.println("CAN BUS Failure...");
#endif
    delay(50);
  }
#if debug
  Serial.println("CAN BUS is ready!");
#endif
  Wire.begin();
}

int inFuelLevel = 0, FuelLevel = 0;
byte len = 0; //unsigned char len = 0;
byte buf[8]; //unsigned char buf[8];
uint16_t inExtV, inTwelveV, inFiveV, inSGNDV, inRPM, inIMAP, inEMAP, inTPSOverall, inTPS1, inLambda, inECT, inMAT, inOilT, inFuelT, inOilP,
         inFuelP, inDiffFuelP, inServoPos, inCoolantP, inEthanol, inVehicleSpeed, inGearNumber, inSpdDiff, inFlagsLow, inFlagsHigh, inSlipLRGround,
         inKnockMax, inInj1Duty, inInj2Duty, inInj3Duty, inInj4Duty, inCalcChargTemp1, inStoichRatio, inTargetLambda, inFuelInjDurOut1,
         inFuelInjDurOut2, inIgnTiming, inAsyncInjDur1, inAsyncInjDur2, inIdleEffortCL, inUnclippedIdleEffort, inIdleEffortDuty,
         inCuttingCond, inCurrentRPMLimit, inPitlaneRPMLimit, inFuelCut, inIgnCut, inInjFlowRate, inCruiseState, inCruiseSpeed;
uint16_t  RPM = 1000, IMAP = 0, EMAP = 0, TPSOverall = 0, TPS1 = 0, Twelve, Lambda, FlagsLow,
          ECT = 0, MAT = 0, OilT = 0, FuelT = 0, OilP = 0, FuelP = 0, DiffFuelP = 0, ServoPos = 0,
          CoolantP = 0, Ethanol = 200, VehicleSpeed = 0, GearNumber = 0, SpdDiff = 0, FlagsHigh = 0,
          SlipLRGround = 0, KnockMax = 0, Inj1Duty = 0, Inj2Duty = 0, Inj3Duty = 0, Inj4Duty = 0, CalcChargTemp1 = 0,
          StoichRatio = 0, TargetLambda = 0, FuelInjDurOut1 = 0, FuelInjDurOut2 = 0, IgnTiming = 0,
          AsyncInjDur1 = 0, AsyncInjDur2 = 0, IdleEffortCL = 0, UnclippedIdleEffort = 0, IdleEffortDuty = 0,
          CuttingCond = 0, CurrentRPMLimit = 0, PitlaneRPMLimit = 0, FuelCut = 0, IgnCut = 0, FuelL = 50, InjFlowRate = 0,
          CruiseState = 0, CruiseSpeed = 0;

void sendWireMessage(byte id, uint16_t value) {
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(id);              // sends one byte
  Wire.write((value >> 8) & 0xFF);
  Wire.write(value & 0xFF);
  Wire.endTransmission();
}

byte count = 0;
void loop() {
  if (CAN_MSGAVAIL == CAN.checkReceive())  {
    CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf
    unsigned char canId = CAN.getCanId();
#if debug
//    Serial.print("-----------------------------\nCAN ID:\t");
//    Serial.println(canId, HEX);
    //    for (int i = 0; i < len; i++) {
    //      Serial.print(buf[i], HEX);
    //      Serial.print("\t");
    //    }
#endif
    switch (canId) {
      case 0x04:
        inExtV = (buf[0] << 8) + buf [1];
        inTwelveV = (buf[2] << 8) + buf[3];
        if (Twelve != inTwelveV) {
          sendWireMessage(1, inTwelveV);
          Twelve = inTwelveV;
        }
        inFiveV = (buf[4] << 8) + buf [5];
        inSGNDV = (buf[6] << 8) + buf [7];
        break;
      case 0x10:
        inRPM = (buf[0] << 8) + buf [1];
        if (inRPM != RPM) {
          sendWireMessage(2, inRPM);
          RPM = inRPM;
        }
        break;
      case 0x61:
        inIMAP = (buf[0] << 8) + buf [1];
        if (IMAP != inIMAP) {
          sendWireMessage(3, inIMAP);
          IMAP = inIMAP;
        }
        inEMAP = (buf[4] << 8) + buf [5];
        break;
      case 0x63:
        inTPSOverall = (buf[0] << 8) + buf [1];
        inTPS1 = (buf[2] << 8) + buf [3];
        break;
      case 0x64:
        inLambda = (buf[2] << 8) + buf [3];
        if (Lambda != inLambda) {
          sendWireMessage(4, inLambda);
          Lambda = inLambda;
        }
        inECT = (buf[6] << 8) + buf [7];
        if (ECT != inECT) {
          sendWireMessage(5, inECT);
          ECT = inECT;
        }
        break;
      case 0x65:
        inMAT = (buf[0] << 8) + buf [1];
        inOilT = (buf[2] << 8) + buf [3];
        if (inOilT == 655 || inOilT < 0)
          inOilT = 0;
        if (OilT != inOilT) {
          sendWireMessage(6, inOilT);
          OilT = inOilT;
        }
        inFuelT = (buf[4] << 8) + buf [5];
        inOilP = (buf[6] << 8) + buf [7];
        if (inOilP == 655 || inOilP < 0)
          inOilP = 0;
        if (OilP != inOilP) {
          sendWireMessage(7, inOilP);
          OilP = inOilP;
        }
        break;
      case 0x66:
        inFuelP = (buf[0] << 8) + buf [1];
        if (FuelP != inFuelP) {
          sendWireMessage(8, inFuelP);
          FuelP = inFuelP;
        }
        inDiffFuelP = (buf[2] << 8) + buf [3];
        inServoPos = (buf[4] << 8) + buf [5];
        inCoolantP = (buf[6] << 8) + buf [7];
        break;
      case 0x67:
        inEthanol = (buf[4] << 8) + buf [5];
        if (Ethanol != inEthanol) {
          sendWireMessage(9, inEthanol);
          Ethanol = inEthanol;
        }
        inVehicleSpeed = (buf[6] << 8) + buf [7];
        if (VehicleSpeed != inVehicleSpeed) {
          sendWireMessage(10, inVehicleSpeed);
          VehicleSpeed = inVehicleSpeed;
        }
        break;
      case 0x68:
        inGearNumber = (buf[0] << 8) + buf [1];
        inSpdDiff = (buf[6] << 8) + buf [7];
        break;
      case 0x6C:
        inFlagsLow = (buf[4] << 8) + buf [5];
        if (FlagsLow != inFlagsLow) {
          sendWireMessage(11, inFlagsLow);
          FlagsLow = inFlagsLow;
        }
        inFlagsHigh = (buf[6] << 8) + buf [7];
        if (FlagsHigh != inFlagsHigh) {
          sendWireMessage(11, inFlagsHigh);
          FlagsHigh = inFlagsHigh;
        }
        break;
      case 0x75:
        inSlipLRGround = (buf[0] << 8) + buf [1];
        inKnockMax = (buf[6] << 8) + buf [7];
        break;
      case 0x78:
        inInj1Duty = (buf[0] << 8) + buf [1];
        inInj2Duty = (buf[2] << 8) + buf [3];
        inInj3Duty = (buf[4] << 8) + buf [5];
        inInj4Duty = (buf[6] << 8) + buf [7];
        break;
      case 0x81:
        inCalcChargTemp1 = (buf[0] << 8) + buf [1];
        inStoichRatio = (buf[4] << 8) + buf [5];
        inTargetLambda = (buf[6] << 8) + buf [7];
        break;
      case 0x84:
        inInjFlowRate = (buf[4] << 8) + buf [5];
        if (InjFlowRate != inInjFlowRate) {
          sendWireMessage(13, inInjFlowRate);
          InjFlowRate = inInjFlowRate;
#if debug
//          Serial.println(InjFlowRate);
#endif
        }
        break;
      case 0x86: //TODO Change Vars
        inInj1Duty = (buf[0] << 8) + buf [1];
        inInj2Duty = (buf[2] << 8) + buf [3];
        inInj3Duty = (buf[4] << 8) + buf [5];
        inInj4Duty = (buf[6] << 8) + buf [7];
        break;
      case 0x8A:
        inFuelInjDurOut1 = (buf[4] << 8) + buf [5];
        inFuelInjDurOut2 = (buf[6] << 8) + buf [7];
        break;
      case 0xA3:
        inIgnTiming = (buf[0] << 8) + buf [1];
        break;
      case 0xA5:
        inAsyncInjDur1 = (buf[0] << 8) + buf [1];
        inAsyncInjDur2 = (buf[4] << 8) + buf [5];
        break;
      case 0xAA:
        inIdleEffortCL = (buf[0] << 8) + buf [1];
        inUnclippedIdleEffort = (buf[4] << 8) + buf [5];
        inIdleEffortDuty = (buf[6] << 8) + buf [7];
        break;
      case 0xB0:
        inCuttingCond = (buf[0] << 8) + buf [1];
        inCurrentRPMLimit = (buf[2] << 8) + buf [3];
        inPitlaneRPMLimit = (buf[4] << 8) + buf [5];
        break;
      case 0xB1:
        inFuelCut = (buf[4] << 8) + buf [5];
        inIgnCut = (buf[6] << 8) + buf [7];
        break;
      case 0xB5:
        inCruiseState = (buf[2] << 8) + buf [3];
        if (inCruiseState != CruiseState) {
          sendWireMessage(15, inCruiseState);
          CruiseState = inCruiseState;
        }
        inCruiseSpeed = (buf[4] << 8) + buf [5];
        if (inCruiseSpeed != CruiseSpeed) {
          sendWireMessage(16, inCruiseSpeed);
          CruiseState = inCruiseState;
        }
        break;
    }
  }
  count++;
  if (count > 240) {
    readFuelLevel();
    count = 0;
  }
}

int fuelLvl[4];// = new int[4];
byte index = 0;
void readFuelLevel() {
  inFuelLevel = analogRead(FUELPIN);
  FuelLevel = 100 - (inFuelLevel / 3);
#if debug
//  Serial.print("\nFuel Level: ");
//  Serial.println(inFuelLevel);
//  Serial.print("\nFuel Level Mod: ");
//  Serial.println(FuelLevel);
#endif
  if (FuelLevel > 100)
    FuelLevel = 100;
  else if (FuelLevel < 0)
    FuelLevel = 0;
  if (index > 3) { //Apply Smoothing to the fuel level
    index = 0;
    FuelLevel = (fuelLvl[0] + fuelLvl[1] + fuelLvl[2] + fuelLvl[3]) / 4;
    sendWireMessage(14, FuelLevel);
  } else {
    fuelLvl[index] = FuelLevel;
    index++;
  }
}

