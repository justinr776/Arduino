int button_press = 0;
byte displaySet = -1;
volatile byte aFlag = 0, bFlag = 0;
volatile byte encoderPos = 0; //this variable stores our current value of encoder position. Change to int or uin16_t instead of byte if you want to record a larger range than 0-255
volatile byte oldEncPos = 0; //stores the last encoder position value so we can compare to the current reading and see if it has changed (so we know when to print to the serial monitor)
volatile byte reading = 0;
byte oldButtonState = HIGH;  // assume switch open because of pull-up resistor
const unsigned long debounceTime = 10;  // milliseconds
unsigned long buttonPressTime;  // when the switch last changed state
boolean buttonPressed = 0; // a flag variable
// Menu and submenu/setting declarations // This is which menu mode we are in at any given time (top level or one of the submenus)
byte modeMax = 2; // This is the number of submenus/settings you want
byte firstButtonPress = 0;
bool displayed = false;
int dotSpace = 25;

void setDot(byte pos) {
  pos++;
  tft.fillRect(0, 0, dotSpace, 600, RA8875_BLACK);
  tft.fillRect(8, pos * 17 + pos * 10, 10, 10, RA8875_RED);
}

void displayValue() { // Text on the bottom with the encoder value
  tft.setFontScale(4);
  tft.fillRect(675, 0, 125, 65, RA8875_BLACK);
  tft.setCursor(675, 0);
  tft.print(oldEncPos);
}

void mainMenu() {
  tft.fillWindow(RA8875_BLACK);
  tft.setFontScale(4);
  setDot(0);
  encoderPos = 0;
  modeMax = 2;
  tft.setCursor(dotSpace, 0);
  tft.print("DISPLAY BRIGHTNESS");
  tft.setCursor(dotSpace, 55);
  tft.print("RESET TRIP");
  tft.setCursor(dotSpace, 110);
  tft.print("EXIT");
}

void buttonPressedSubMenu() {
  if (displayMode == 1 ) {
    tft.brightness(oldEncPos);
    modeMax = 2;
    leftDisplayMode = true;
    displayMode = 255;
  } else if (displayMode == 2) {
    if (oldEncPos == 1) {
      miles = 0;
      mileCounter = 0;
      EEPROM.put(0, miles);
      buttonPressed = 0;
      UpdateMiles();
    }
    leftDisplayMode = true;
    displayMode = 255;
    modeMax = 2;
  } else if (displayMode == 3) {
    if (oldEncPos = 0) {
      leftDisplayMode = true;
      displayMode = 255;
      modeMax = 2;
    } else
      displayMode = 0;
  }
}

  // brightness settings, resetting the trip, etc.
  // have another sort of trip that could be set to x value and tell you when you need to change the oil since there is no real other reasoning for an odo.
  // Later I will need a few menus for DCCD and other drive modes, but I can't use any of that right now, so it's a pretty low priority.

void rotaryMenu() {
  tft.setTextColor(RA8875_WHITE);
  byte buttonState = digitalRead (encoderbutton);
  if (buttonState != oldButtonState) {
    if (millis () - buttonPressTime >= debounceTime) { // debounce
      buttonPressTime = millis ();
      oldButtonState =  buttonState;
      if (buttonState == LOW) {
        buttonPressed = 1;
        Serial.println("Button Pressed");
      }
      else {
        buttonPressed = 0;
        Serial.println("Button DePressed");
      }
    }  // end if debounce time up
  } // end of state change

  if (oldEncPos != encoderPos) {
    Serial.println(encoderPos);
    oldEncPos = encoderPos;
    if (displayMode != 255)
      displayValue();
    if (displayMode == 1)
      tft.brightness(encoderPos);
  }
  Serial.print("Display Mode: ");
  Serial.println(displayMode);

  if (displayMode == 255 && buttonPressed == 1) {
    displayMode = 0;
    buttonPressed = 0;
    encoderPos = 0;
    oldEncPos = 0;
    modeMax = 2;
  }

  // Modify encoder pos to match the menu options
  if (displayMode != 255) {
    if (oldEncPos < 0) { // check we haven't gone out of bounds below 0 and correct if we have
      oldEncPos = modeMax;
      encoderPos = modeMax;
    } else if (oldEncPos > modeMax) { // check we haven't gone out of bounds above modeMax and correct if we have
      encoderPos = 0;
      oldEncPos = 0;
    }
    setDot(oldEncPos);
  }

  //Main menu section
  if (displayMode == 0) {
    if (buttonPressed) {
      displayMode = oldEncPos + 1; // set the Mode to the current value of input if button has been pressed
    } else {
      if (!displayed) {
        mainMenu();
        setDot(oldEncPos);
        displayed = true;
      }
    }
    buttonPressed = 0; // reset the button status so one press results in one action
    if (displayMode == 1) {// Display Brightness
      Serial.println("Display Brightness");
      tft.fillWindow(RA8875_BLACK);
      tft.setCursor(dotSpace, 0);
      tft.print("DISPLAY BRIGHTNESS");
      tft.setCursor(dotSpace, 55);
      tft.print("CLICK TO SET");
      encoderPos = 55;
      oldEncPos = 55;
      modeMax = 254;
    } else if (displayMode == 2) {//Trip Reset
      tft.fillWindow(RA8875_BLACK);
      tft.setFontScale(4);
      setDot(0);
      encoderPos = 0;
      tft.setCursor(dotSpace, 0);
      tft.print("Exit");
      tft.setCursor(dotSpace, 55);
      tft.print("Reset");
      modeMax = 1;
    } else if (displayMode == 3) {//Exit
      tft.fillWindow(RA8875_BLACK);
      tft.setFontScale(4);
      setDot(0);
      encoderPos = 0;
      tft.setCursor(dotSpace, 0);
      tft.print("Exit");
      tft.setCursor(dotSpace, 55);
      tft.print("Back");
      modeMax = 1;
    }
  } else {
    displayed = false;
  }

  // Button Pressed in a menu option
  if (buttonPressed == 1) {
    buttonPressedSubMenu();
  }
}

void initializeRotary() {
  pinMode (encoder0PinA, INPUT_PULLUP);
  pinMode (encoder0PinB, INPUT_PULLUP);
  pinMode (encoderbutton, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoder0PinA), PinA, RISING);
  attachInterrupt(digitalPinToInterrupt(encoder0PinB), PinB, RISING);
}

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

/**
  // Carry out common activities each time a setting is changed
  void setAdmin(byte name, byte setting) {
  Serial.print("Setting ");
  Serial.print(name);
  Serial.print(" = ");
  Serial.println(setting);
  encoderPos = 0; // reorientate the menu index - optional as we have overflow check code elsewhere
  buttonPressed = 0; // reset the button status so one press results in one action
  displayMode = 255;
  Serial.println("Main Menu");
  }

*/
