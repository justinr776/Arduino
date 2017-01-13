#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

void(* resetFunc) (void) = 0;
int NUMPIXELS;
#define PIN 6
unsigned int Color(byte r, byte g, byte b) {
  return ( ((unsigned int)g & 0x1F ) << 10 | ((unsigned int)b & 0x1F) << 5 | ((unsigned int)r & 0x1F));
}
Adafruit_NeoPixel strip = Adafruit_NeoPixel(EEPROM.read(11), PIN, NEO_GRB + NEO_KHZ800);

const int timeoutValue = 10;
volatile unsigned long interval = 0;
volatile int timeoutCounter;
long RPM_last;
int activation_rpm;
int shift_rpm = 0;
long previousMillis = 0;
long shiftinterval = 75;
boolean flashbool = true;
int prev_animation;
int prev_color;
boolean testbright = false;
long prev_variable;
int prev_menu;
boolean testdim = false;
int justfixed;

//These are stored memory variables for adjusting the (5) colors, activation RPM, shift RPM, brightness
//Stored in EEPROM Memory
int c1;
int c2;
int c3;
int c4;
int c5;
int sb;             //strip brightness
int dimsb;          // strip dim brightness
boolean dimmer = false;
boolean dimmerlogic = false;
int pixelanim = 1;
int senseoption;
int RPMscaler;
long shift_rpm1;
long shift_rpm2;
long shift_rpm3;
long shift_rpm4;
int seg1_start = 1;
int seg1_end = 1;
int seg2_start = 2;
int seg2_end = 2;
int seg3_start = 3;
int seg3_end = 3;
long activation_rpm1;
long activation_rpm2;
long activation_rpm3;
long activation_rpm4;
long cal1;
long cal2;
long cal3;
long cal4;
int rst = 0;
int cal;
long calfunc;
int prev_cal;

// COLOR VARIABLES - for use w/ the strips and translated into 255 RGB colors
uint32_t color1;
uint32_t color2;
uint32_t color3;
uint32_t flclr1;
uint32_t flclr2;

//uint8_t *SP;

//Creates a 32 wide table for our pixel animations
int rpmtable[32][3];

void ShiftLightLoop() {
  if (RPM < shift_rpm) {
    int a;
    for (a = 0; a < NUMPIXELS; a++) {
      if (RPM > rpmtable[a][0]) {
        switch (rpmtable[a][1]) {
          case 1:
            strip.setPixelColor(a, color1);
            break;

          case 2:
            strip.setPixelColor(a, color2);
            break;

          case 3:
            strip.setPixelColor(a, color3);
            break;
        }
      } else {
        strip.setPixelColor(a, strip.Color(0, 0, 0));
      }
      strip.show();
    }
  } else {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > shiftinterval) {
      previousMillis = currentMillis;
      flashbool = !flashbool;
      if (flashbool == true)
        for (int i = 0; i < NUMPIXELS; i++) {
          strip.setPixelColor(i, color3);
        }
      else
        for (int i = 0; i < NUMPIXELS; i++) {
          strip.setPixelColor(i, 0);
        }
      strip.show();
    }
  }
}

void buildarrays() {
  int x;  //RPM increment
  int y;  //starting point pixel address
  int ya; // second starting point pixel address (for middle-out animation only)
  int i;  //temporary for loop variable

  if (DEBUG) {
    Serial.println("PIXELANIM   ");
    Serial.println(pixelanim);
    Serial.println("  Activation RPM ");
    Serial.println(activation_rpm);
    Serial.println("  SHIFT RPM ");
    Serial.println(shift_rpm);
  }

  switch (pixelanim) {
    case 1:
      y = 0;
      x = ((shift_rpm - activation_rpm) / NUMPIXELS);
      for (i = 0; i < seg1_end + 1; i++) {
        rpmtable[i][0] = activation_rpm + (i * x);
        rpmtable[i][1] = 1;
      }
      for (i = seg1_end + 1; i < seg2_end + 1; i++) {
        rpmtable[i][0] = activation_rpm + (i * x);
        rpmtable[i][1] = 2;
      }
      for (i = seg2_end + 1; i < seg3_end + 1; i++) {
        rpmtable[i][0] = activation_rpm + (i * x);
        rpmtable[i][1] = 3;
      }
      break;

    case 2:
      if (((NUMPIXELS - 1) % 2) > 0) {
        x = ((shift_rpm - activation_rpm) / (NUMPIXELS / 2)); //EVEN PIXELS
      } else {
        x = ((shift_rpm - activation_rpm) / ((NUMPIXELS / 2) + 1)); //ODD PIXELS
      }

      ya = 0;   // SEGMENT 1
      for (i = seg1_start; i < seg1_end + 1; i++) {
        rpmtable[i][0] = activation_rpm + (ya * x);
        rpmtable[i][1] = 1;
        ya++;
      }

      if (((NUMPIXELS - 1) % 2) > 0) {
        ya = 0;
        for (i = seg1_start - 1; i > seg1_start - (seg1_end - seg1_start) - 2; i--) {
          rpmtable[i][0] = activation_rpm + (ya * x);
          rpmtable[i][1] = 1;
          ya++;
        }
      } else {
        ya = 1;
        for (i = seg1_start - 1; i > seg1_start - (seg1_end - seg1_start) - 1; i--) {
          rpmtable[i][0] = activation_rpm + (ya * x);
          rpmtable[i][1] = 1;
          ya++;
        }
      }

      if (seg2_start == seg2_end) {
        ya =  seg2_end - seg1_start;  //SEGMENT 2
      } else {
        ya =  seg2_start - seg1_start;
      }

      for (i = seg2_start; i < seg2_end + 1; i++) {
        rpmtable[i][0] = activation_rpm + (ya * x);
        rpmtable[i][1] = 2;
        ya++;
      }

      if (seg2_start == seg2_end) {
        ya =  seg2_end - seg1_start;  //SEGMENT 2
      } else {
        ya =  seg2_start - seg1_start;
      }

      if (((NUMPIXELS - 1) % 2) > 0) {
        for (i = seg1_start - (seg1_end - seg1_start) - 2; i > seg1_start - (seg2_end - seg1_start) - 2; i--) {
          rpmtable[i][0] = activation_rpm + (ya * x);
          rpmtable[i][1] = 2;
          ya++;
        }
      } else {
        for (i = seg1_start - (seg1_end - seg1_start) - 1; i > seg1_start - (seg2_end - seg1_start) - 1; i--) {
          rpmtable[i][0] = activation_rpm + (ya * x);
          rpmtable[i][1] = 2;
          ya++;
        }
      }

      if (seg3_start == seg3_end) {
        ya =  seg3_end - seg1_start;    //SEGMENT 3
      } else {
        ya =  seg3_start - seg1_start;    //SEGMENT 3
      }

      for (i = seg3_start; i < seg3_end + 1; i++) {
        rpmtable[i][0] = activation_rpm + (ya * x);
        rpmtable[i][1] = 3;
        ya++;
      }

      if (seg3_start == seg3_end) {
        ya =  seg3_end - seg1_start;
      } else {
        ya =  seg3_start - seg1_start;
      }

      if (((NUMPIXELS - 1) % 2) > 0) {
        for (i = seg1_start - (seg2_end - seg1_start) - 2; i > seg1_start - (seg3_end - seg1_start) - 2; i--) {
          rpmtable[i][0] = activation_rpm + (ya * x);
          rpmtable[i][1] = 3;
          ya++;
        }
      } else {
        for (i = seg1_start - (seg2_end - seg1_start) - 1; i > seg1_start - (seg3_end - seg1_start) - 1; i--) {
          rpmtable[i][0] = activation_rpm + (ya * x);
          rpmtable[i][1] = 3;
          ya++;
        }
      }
      break;

    case 3:
      y = 0;
      x = ((shift_rpm - activation_rpm) / NUMPIXELS);
      for (i = NUMPIXELS - 1; i > seg1_start - 1; i--) {
        rpmtable[i][0] = activation_rpm + (y * x);
        rpmtable[i][1] = 1;
        y++;
      }
      for (i = seg1_start - 1; i > seg2_start - 1; i--) {
        rpmtable[i][0] = activation_rpm + (y * x);
        rpmtable[i][1] = 2;
        y++;
      }
      for (i = seg2_start - 1; i > seg3_start - 1; i--) {
        rpmtable[i][0] = activation_rpm + (y * x);
        rpmtable[i][1] = 3;
        y++;
      }
      break;
  }

  if (DEBUG) {
    for (i = 0; i < NUMPIXELS; i++) {
      Serial.print(rpmtable[i][0]);
      Serial.print("  ");
      Serial.print(rpmtable[i][1]);
      Serial.print("  ");
      Serial.println(rpmtable[i][2]);
    }
  }
}

/************************* SUBROUTINES *************************/
//This subroutine reads the stored variables from memory
void getEEPROM() {
  sb = EEPROM.read(1);
  c1 = EEPROM.read(2);
  c2 = EEPROM.read(3);
  c3 = EEPROM.read(4);
  c4 = EEPROM.read(5);
  c5 = EEPROM.read(6);
  activation_rpm = EEPROM.read(7);
  pixelanim  = EEPROM.read(8);
  senseoption  = EEPROM.read(9);
  NUMPIXELS = EEPROM.read(11);
  RPMscaler = EEPROM.read(12);
  shift_rpm1 = EEPROM.read(13);
  shift_rpm2 = EEPROM.read(14);
  shift_rpm3 = EEPROM.read(15);
  shift_rpm4 = EEPROM.read(16);
  //DEBUG = EEPROM.read(17);
  //  seg1_start = EEPROM.read(18);
  //  seg1_end = EEPROM.read(19);
  //  seg2_start = EEPROM.read(20);
  //  seg2_end = EEPROM.read(21);
  //  seg3_start = EEPROM.read(22);
  //  seg3_end = EEPROM.read(23);
  activation_rpm1 = EEPROM.read(24);
  activation_rpm2 = EEPROM.read(25);
  activation_rpm3 = EEPROM.read(26);
  activation_rpm4 = EEPROM.read(27);
  cal = EEPROM.read(28);
  dimsb = EEPROM.read(30);
  dimmerlogic = EEPROM.read(31);

  activation_rpm = ((activation_rpm1 << 0) & 0xFF) + ((activation_rpm2 << 8) & 0xFFFF) + ((activation_rpm3 << 16) & 0xFFFFFF) + ((activation_rpm4 << 24) & 0xFFFFFFFF);
  shift_rpm = ((shift_rpm1 << 0) & 0xFF) + ((shift_rpm2 << 8) & 0xFFFF) + ((shift_rpm3 << 16) & 0xFFFFFF) + ((shift_rpm4 << 24) & 0xFFFFFFFF);

  buildarrays();
}

//This subroutine writes the stored variables to memory
void writeEEPROM() {
  byte four = (shift_rpm & 0xFF);
  byte three = ((shift_rpm >> 8) & 0xFF);
  byte two = ((shift_rpm >> 16) & 0xFF);
  byte one = ((shift_rpm >> 24) & 0xFF);

  byte activation_four = (activation_rpm & 0xFF);
  byte activation_three = ((activation_rpm >> 8) & 0xFF);
  byte activation_two = ((activation_rpm >> 16) & 0xFF);
  byte activation_one = ((activation_rpm >> 24) & 0xFF);

  EEPROM.write(1, sb);
  EEPROM.write(2, c1);
  EEPROM.write(3, c2);
  EEPROM.write(4, c3);
  EEPROM.write(5, c4);
  EEPROM.write(6, c5);
  EEPROM.write(7, activation_rpm);
  EEPROM.write(8, pixelanim);
  EEPROM.write(9, senseoption);
  EEPROM.write(11, NUMPIXELS);
  EEPROM.write(12, RPMscaler);
  EEPROM.write(13, four);
  EEPROM.write(14, three);
  EEPROM.write(15, two);
  EEPROM.write(16, one);
  //EEPROM.write(17, DEBUG);
  EEPROM.write(18, seg1_start);
  EEPROM.write(19, seg1_end);
  EEPROM.write(20, seg2_start);
  EEPROM.write(21, seg2_end);
  EEPROM.write(22, seg3_start);
  EEPROM.write(23, seg3_end);
  EEPROM.write(24, activation_four);
  EEPROM.write(25, activation_three);
  EEPROM.write(26, activation_two);
  EEPROM.write(27, activation_one);
  EEPROM.write(28, cal);
  EEPROM.write(30, dimsb);
  EEPROM.write(31, dimmerlogic);
}

//This sub clears the strip to all OFF
void clearStrip() {
  for ( int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
  }
}

//Helper Color Manager - This translates our 255 value into a meaningful color
uint32_t load_color(int cx) {
  unsigned int r, g, b;
  if (cx == 0) {
    r = 0;
    g = 0;
    b = 0;
  }

  if (cx > 0 && cx <= 85) {
    r = 255 - (cx * 3);
    g = cx * 3;
    b = 0;
  }

  if (cx > 85 && cx < 170) {
    r = 0;
    g = 255 - ((cx - 85) * 3);
    b = (cx - 85) * 3;
  }

  if (cx >= 170 && cx < 255) {
    r = (cx - 170) * 3;
    g = 0;
    b = 255 - ((cx - 170) * 3);
  }

  if (cx == 255) {
    r = 255;
    g = 255;
    b = 255;
  }

  r = (r / sb);
  g = (g / sb);
  b = (b / sb);

  return strip.Color(r, g, b);
}

void testlights(int color) {
  for (int a = 0; a < NUMPIXELS; a++) {
    if (color < 4) {
      if (rpmtable[a][1] == color) {
        switch (color) {
          case 1:
            strip.setPixelColor(a, color1);
            break;

          case 2:
            strip.setPixelColor(a, color2);
            break;

          case 3:
            strip.setPixelColor(a, color3);
            break;
        }
      } else {
        strip.setPixelColor(a, strip.Color(0, 0, 0));
      }
    } else {
      switch (rpmtable[a][1]) {
        case 1:
          strip.setPixelColor(a, color1);
          break;

        case 2:
          strip.setPixelColor(a, color2);
          break;

        case 3:
          strip.setPixelColor(a, color3);
          break;
      }
    }
    Serial.println(rpmtable[a][1]);
    strip.show();
  }
}

void check_first_run() {
  if (shift_rpm == 0) {
    strip.begin();
    strip.clear();
    //Serial.println("FIRST RUN! LOADING DEFAULTS");
    dimsb = 15;
    dimmerlogic = false;
    sb = 12;
    c1 = 79;
    c2 = 48;
    c3 = 1;
    c4 = 255;
    c5 = 3;

    activation_rpm = 500;
    shift_rpm = 6500;
    pixelanim  = 1;
    senseoption  = 2;
    NUMPIXELS = 16;
    //RPMscaler = EEPROM.read(12);
    //DEBUG = 0;
    seg1_start = 0;
    seg1_end = 6;
    seg2_start = 0;
    seg2_end = 10;
    seg3_start = 0;
    seg3_end = 16;
    cal = 30;
    color1 = load_color(65);//Color(15, 0, 0);
    color2 = load_color(40);//Color(0, 0, 15);
    color3 = load_color(25);//Color(0, 15, 0);
    //flclr1 == load_color(25);
    //flclr2 == load_color(85);
    //writeEEPROM();
    //resetFunc();
  }
}
