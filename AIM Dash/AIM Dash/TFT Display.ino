﻿#include <SPI.h>
#include <RA8875.h>
#include "fonts/squarefont_14.c"
#include <Wire.h>
/*
Arduino's
You are using 4 wire SPI here, so:
MOSI:  11//Arduino UNO
MISO:  12//Arduino UNO
SCK:   13//Arduino UNO
the rest of pin below:
*/
#define RA8875_CS 10 //see below...
#define RA8875_RESET 9//any pin or nothing!

RA8875 tft = RA8875(RA8875_CS, RA8875_RESET); //Teensy3/arduino's
void setup() {
	long unsigned debug_start = millis();
	tft.begin(RA8875_800x480);
	tft.fillWindow(RA8875_BLACK);//fill window RED
	tft.setTextColor(RA8875_WHITE);
	tft.setFont(&squarefont_14);
	tft.showCursor(NOCURSOR, false);
	tft.setFontScale(2);
	tft.setCursor(45, 235);
	tft.print("OILP");
	tft.setCursor(45, 330);
	tft.print("FUELP");
	tft.setCursor(45, 425);
	tft.print("VOLT");
	tft.setCursor(675, 235);
	tft.print("COOL");
	tft.setCursor(675, 330);
	tft.print("OILT");
	tft.setCursor(675, 425);
	tft.print("FLEX");
	tft.setCursor(766, 390);
	tft.print("%");
	tft.brightness(125);
	SetTestValues();
	Wire.begin(8);
	Wire.onReceive(wireReceive);
}

uint16_t  RPM = 1000, PRPM = 0, IMAP = 0, EMAP = 0, TPSOverall = 0, TPS1 = 0,
ECT = 0, MAT = 0, OilT = 0, FuelT = 0, OilP = 0, FuelP = 0, DiffFuelP = 0, ServoPos = 0,
CoolantP = 0, Ethanol = 0, VehicleSpeed = 0, GearNumber = 0, SpdDiffinFlagsLow = 0, FlagsHigh = 0,
SlipLRGround = 0, KnockMax = 0, Inj1Duty = 0, Inj2Duty = 0, Inj3Duty = 0, Inj4Duty = 0, CalcChargTemp1 = 0,
StoichRatio = 0, TargetLambda = 0, FuelInjDurOut1 = 0, FuelInjDurOut2 = 0, IgnTiming = 0,
AsyncInjDur1 = 0, AsyncInjDur2 = 0, IdleEffortCL = 0, UnclippedIdleEffort = 0, IdleEffortDuty = 0,
CuttingCond = 0, CurrentRPMLimit = 0, PitlaneRPMLimit = 0, FuelCut = 0, IgnCut = 0, FuelL = 50;
float ExtV = 0, TwelveV = 0, FiveV = 0, SGNDV = 0, Lambda = 1, PLambda = 0;
boolean bExtV = false, bTwelveV = false, bFiveV = false, bSGNDV = false, bRPM = false, bIMAP = false, bEMAP = false,
bTPSOverall = false, bTPS1 = false, bLambda = false, bECT = false, bMAT = false, bOilT = false, bFuelT = false,
bOilP = false, bFuelP = false, bDiffFuelP = false, bServoPos = false, bCoolantP = false, bEthanol = false,
bVehicleSpeed = false, bGearNumber = false, bSpdDiffinFlagsLow = false, bFlagsHigh = false, bSlipLRGround = false,
bKnockMax = false, bInj1Duty = false, bInj2Duty = false, bInj3Duty = false, bInj4Duty = false, bCalcChargTemp1 = false,
bStoichRatio = false, bTargetLambda = false, bFuelInjDurOut1 = false, bFuelInjDurOut2 = false, bIgnTiming = false,
bAsyncInjDur1 = false, bAsyncInjDur2 = false, bIdleEffortCL = false, bUnclippedIdleEffort = false, bIdleEffortDuty = false,
bCuttingCond = false, bCurrentRPMLimit = false, bPitlaneRPMLimit = false, bFuelCut = false, bIgnCut = false, bFuelL = false;
int f1 = 26, f2 = 45, f3 = 63;

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
		tft.setCursor(45, 175);
		tft.setFontScale(4);
		tft.fillRect(50, 195, 110, f2, RA8875_BLACK);
		tft.print(OilP);
		bOilP = false;
	}
	if (bFuelP) {
		tft.setCursor(45, 270);
		tft.setFontScale(4);
		tft.fillRect(50, 290, 110, f2, RA8875_BLACK);
		tft.print(FuelP);
		bFuelP = false;
	}
	if (bTwelveV) {
		tft.setCursor(45, 365);
		tft.setFontScale(4);
		tft.fillRect(50, 385, 145, f2, RA8875_BLACK);
		tft.print(TwelveV);
		bTwelveV = false;
	}
	if (bFuelL) { //220, 280, 70w, 200
		int val = 200 - (FuelL << 1);
		tft.fillRect(220, 280, 70, val, RA8875_WHITE);
		tft.fillRect(220, 280 + val, 70, 200 - val, RA8875_GREEN);
		bFuelL = false;
	}
	if (bECT) {
		tft.setCursor(675, 175);
		tft.setFontScale(4);
		tft.fillRect(680, 195, 110, f2, RA8875_BLACK);
		tft.print(ECT);
		bECT = false;
	}
	if (bOilT) {
		tft.setCursor(675, 270);
		tft.setFontScale(4);
		tft.fillRect(680, 290, 110, f2, RA8875_BLACK);
		tft.print(OilT);
		bOilT = false;
	}
	if (bEthanol) {
		tft.setCursor(675, 365);
		tft.setFontScale(4);
		tft.fillRect(680, 385, 88, f2, RA8875_BLACK);
		tft.print(Ethanol);
		bEthanol = false;
	}
}
boolean Lup = true;
void SetTestValues() {
	bFuelP = true;
	FuelP = 58;
	bFuelL = true;
	FuelL = 50;
	bEthanol = true;
	Ethanol = 10;
	bECT = true;
	ECT = 188;
	bTwelveV = true;
	TwelveV = 12.62;
	bOilT = true;
	OilT = 225;
	bOilP = true;
	bLambda = true;
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

void wireReceive(int howMany) {
	byte id = Wire.read();
	byte high = Wire.read();
	byte low = Wire.read();

}

void loop() {
	delay(50);
	//SetTestValues();
	UpdateDisplay();
	SetTestValues();
}