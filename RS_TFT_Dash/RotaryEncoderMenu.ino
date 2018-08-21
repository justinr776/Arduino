#include "RotaryEncoderMenu.h"
int button_press = 0;
byte displaySet = -1;
//static const int encoder0PinA = 2;
//static const int encoder0PinB = 3;
//static const int encoderbutton = 6;
volatile byte aFlag = 0, bFlag = 0;
volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0;
byte oldButtonState = HIGH;  // assume switch open because of pull-up resistor
const unsigned long debounceTime = 10;  // milliseconds
unsigned long buttonPressTime;  // when the switch last changed state
boolean buttonPressed = 0; // a flag variable
// Menu and submenu/setting declarations
byte displayMode = 0;   // This is which menu mode we are in at any given time (top level or one of the submenus)
byte modeMax = 3; // This is the number of submenus/settings you want
int dotSpace = 25;
//int f1 = 26, f2 = 45, f3 = 63; //TODO remove once merged

//Rotary encoder interrupt service routine for one encoder pin
void PinA() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; // read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && aFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos --; //decrement the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00000100) bFlag = 1; //signal that we're expecting pinB to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

//Rotary encoder interrupt service routine for the other encoder pin
void PinB() {
  cli(); //stop interrupts happening before we read pin values
  reading = PIND & 0xC; //read all eight pin values then strip away all but pinA and pinB's values
  if (reading == B00001100 && bFlag) { //check that we have both pins at detent (HIGH) and that we are expecting detent on this pin's rising edge
    encoderPos ++; //increment the encoder's position count
    bFlag = 0; //reset flags for the next turn
    aFlag = 0; //reset flags for the next turn
  }
  else if (reading == B00001000) aFlag = 1; //signal that we're expecting pinA to signal the transition to detent from free rotation
  sei(); //restart interrupts
}

void setDot(byte pos) {
  pos++;
  Serial.print("Dot Pos: ");
  Serial.print(pos);
  tft.fillRect(0, 0, 600, dotSpace, RA8875_BLACK);
  tft.fillRect(8, pos * 17 + pos * 10, 10, 10, RA8875_RED);
}

void displayValue(){
  tft.fillRect(700,0, 100, 45, RA8875_BLACK);
  tft.setCursor(700,0);
  tft.print(encoderPos);
}

void mainMenu() {
  tft.fillWindow(RA8875_BLACK);
  tft.setFontScale(4);
  setDot(0);
  encoderPos = 0;
  modeMax = 3;
  tft.setCursor(dotSpace, 0);
  tft.print("DISPLAY BRIGHTNESS");
  tft.setCursor(dotSpace, 55);
  tft.print("RESET TRIP");
  tft.setCursor(dotSpace, 110);
  tft.print("DCCD");
}

void initializeRotary() {
  Serial.begin(115200);
  pinMode (encoder0PinA, INPUT_PULLUP);
  pinMode (encoder0PinB, INPUT_PULLUP);
  pinMode (encoderbutton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), PinA, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder0PinB), PinB, RISING);
}

// Carry out common activities each time a setting is changed
void setAdmin(byte name, byte setting) {
  Serial.print("Setting ");
  Serial.print(name);
  Serial.print(" = ");
  Serial.println(setting);
  encoderPos = 0; // reorientate the menu index - optional as we have overflow check code elsewhere
  buttonPressed = 0; // reset the button status so one press results in one action
  displayMode = 0; // go back to top level of menu, now that we've set values
  Serial.println("Main Menu");
}

// brightness settings, resetting the trip, etc.
// have another sort of trip that could be set to x value and tell you when you need to change the oil since there is no real other reasoning for an odo.
// Later I will need a few menus for DCCD and other drive modes, but I can't use any of that right now, so it's a pretty low priority.

void rotaryMenu() {
  if (oldEncPos != encoderPos) {
    Serial.println(encoderPos);
    setDot(encoderPos);
    oldEncPos = encoderPos;
    displayValue();
  }
  byte buttonState = digitalRead (encoderbutton);
  if (buttonState != oldButtonState) {
    if (millis () - buttonPressTime >= debounceTime) { // debounce
      buttonPressTime = millis ();
      oldButtonState =  buttonState;
      if (buttonState == LOW) {
        buttonPressed = 1;
      }
      else {
        buttonPressed = 0;
      }
    }  // end if debounce time up
  } // end of state change

  //Main menu section
  if (displayMode == 0) {
    if (encoderPos > (modeMax + 10))
      encoderPos = modeMax; // check we haven't gone out of bounds below 0 and correct if we have
    else if (encoderPos > modeMax)
      encoderPos = 0; // check we haven't gone out of bounds above modeMax and correct if we have
    if (buttonPressed) {
      displayMode = encoderPos; // set the Mode to the current value of input if button has been pressed
      Serial.print("DisplayMode selected: ");
      Serial.println(displayMode);
      buttonPressed = 0; // reset the button status so one press results in one action
      if (displayMode == 1) {
        Serial.println("Display Brightness");
        encoderPos = 75;
        modeMax = 255;
      }
      if (displayMode == 2) {
        Serial.println("Trip Reset");
        modeMax = 2;
      }
      if (displayMode == 3) {
        Serial.println("DCCD");
      }
    }
  }
  if (displayMode == 1 && buttonPressed) {
    tft.brightness(encoderPos);
    modeMax = 3;
  } else if (displayMode == 2 && buttonPressed) {
    if (encoderPos == 1)
      miles = 0;
    modeMax = 3;
  } else if (displayMode == 3 && buttonPressed) {
    modeMax = 3;
  }
}
