#include <LiquidCrystal_I2C.h> //Addr: 0x3F, 20 chars & 4 lines
LiquidCrystal_I2C lcd(0x3F, 20, 4); // Might be 0x27

volatile int encoder0Pos = 0;
int encoder0PinALast = HIGH;
int button_press = LOW;
byte buttonhit = 0;
unsigned long buttonLastPress = 0;
unsigned long lastEncoderRead = 0;
byte displaySet = -1;

//  attachInterrupt(digitalPinToInterrupt(encoder0PinA), RotaryISR, CHANGE);
//  detachInterrupt(digitalPinToInterrupt(encoder0PinA));

void displayPrint(char const *c) { // Print a char[] or string to the LCD
  byte x = 0;
  while (true) {
    if (c[x] != '\0')
      lcd.print(c[x]);
    else
      break;
    x++;
  }
}

void displayPrintInt(int val, int numOfChars) { // Print an int value as characters with the given size.
  char c[numOfChars + 1];
  sprintf(c, "%d", val);
  byte x = 0;
  while (x < numOfChars) { //true
    if (c[x] != '\0')
      lcd.print(c[x]);
    else
      lcd.print(' '); //break;
    x++;
  }
}

void RotaryISR() {
  if (encoder0PinALast == HIGH && millis() - lastEncoderRead > 300 ) {
    if (digitalRead(encoder0PinB) == LOW && millis() - buttonLastPress > 500) {
      encoder0Pos++;
    } else {
      encoder0Pos--;
    }
    lastEncoderRead = millis();
  }
  encoder0PinALast = digitalRead(encoder0PinA);
}

void initializeDisplay() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  displayPrint("Hello world!");
  pinMode (encoder0PinA, INPUT_PULLUP);
  pinMode (encoder0PinB, INPUT_PULLUP);
  pinMode (encoderbutton, INPUT_PULLUP);
}

void MainDisplay() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  displayPrint("OIL ");
  lcd.setCursor(9, 0);
  displayPrint("COOLANT ");
  lcd.setCursor(0, 1);
  displayPrint("FUEL ");
  lcd.setCursor(9, 1);
  displayPrint("LAUNCH ");
  lcd.setCursor(0, 2);
  displayPrint("MPG ");
  lcd.setCursor(9, 2);
  displayPrint("AVG EGT");
}

void TempMainDisplay() {
  //lcd.clear();
  lcd.setCursor(0, 0);
  displayPrint("RPM ");
  lcd.setCursor(9, 0);
  displayPrint("SPEED ");
  lcd.setCursor(0, 1);
  displayPrint("MPG ");
  lcd.setCursor(9, 1);
  displayPrint("COOLANT ");
  lcd.setCursor(0, 2);
  displayPrint("DCCD ");  //AUT or MAN
  lcd.setCursor(9, 2);
  displayPrint("TRIP ");
}

void MainMenu() {
  lcd.setCursor(0, 1);
  displayPrint("MAIN DISPLAY");
  lcd.setCursor(1, 1);
  displayPrint("DCCD");
  lcd.setCursor(2, 1);
  displayPrint("STEERING");
  lcd.setCursor(3, 1);
  displayPrint("CRUISE");
  lcd.setCursor(4, 1);    //If possible to scroll
  displayPrint("SHIFT LIGHT");
}

void DCCDMenu() {
  lcd.setCursor(0, 1);
  displayPrint("MODE:AUTO      BACK");
  lcd.setCursor(2, 0);
  displayPrint("DCCD LOCKUP");
}

void SteeringMenu() {
  lcd.setCursor(0, 1);
  displayPrint("AGGRESSIVE     BACK");
  lcd.setCursor(1, 1);
  displayPrint("SMOOTH");
  lcd.setCursor(2, 1);
  displayPrint("MANUAL");
}

void CruiseMenu() {
  lcd.setCursor(0, 1);
  displayPrint("CRUISE ON OFF  BACK");
  lcd.setCursor(1, 1);
  displayPrint("CRUISE SET");
  lcd.setCursor(3, 1);
  displayPrint("SPEED  ");
  lcd.setCursor(3, 9);
  displayPrint("RST TRIP");
}

void ShiftlightMenu() {
  lcd.setCursor(0, 1);
  displayPrint("LEFT-RIGHT     BACK");
  lcd.setCursor(1, 1);
  displayPrint("CENTER");
  lcd.setCursor(2, 1);
  displayPrint("SHIFT ");
}

void DisplayNavigation() { //TODO Not even close to done
  byte leave = 0;
  while (!leave) {
    button_press = digitalRead(encoderbutton);
    if (button_press == 0 && buttonhit == 0 && millis() - buttonLastPress > 500) {
      buttonhit = 1;
      buttonLastPress = millis();
    } else
      buttonhit = 0;

    UpdateDisplay();
  }
}

void UpdateDisplay() {
  if (displaySet != 1) {
    TempMainDisplay();
    displaySet = 1;
  }
  if (displaySet == 1) {
    lcd.setCursor(4, 0);
    displayPrintInt(RPM, 4);
    lcd.setCursor(17, 0);
    displayPrintInt(speedavg, 3);
    lcd.setCursor(5, 1);
    displayPrintInt(mpg, 3);
    lcd.setCursor(17, 1);
    displayPrintInt(CLT, 3);
    lcd.setCursor(5, 2);
    if (dccdStatus == 0)
      displayPrint("AUT");
    else
      displayPrint("MAN");
    lcd.setCursor(17, 2);
    displayPrintInt(trip, 3);
  }
  //  if (displayMode == 0) { // Main Menu
  //    MainMenu();
  //  } else if (displayMode == 1) { // Main Display
  //    TempMainDisplay();
  //  } else if (displayMode == 2) { // DCCD
  //    DCCDMenu();
  //  } else if (displayMode == 3) { // Steering
  //    SteeringMenu();
  //  } else if (displayMode == 4) { // Cruise
  //    CruiseMenu();
  //  } else if (displayMode == 5) { // Shiftlight
  //    SiftlightMenu();
  //  }
}

