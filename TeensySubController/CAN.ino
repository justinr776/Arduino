#include <FlexCAN.h>

int baud = 500000; // Specify CAN Baudrate. Currently 125k, 250k, 500k, 1M are supported in teensydrino 1.20
FlexCAN CANbus(baud, 1);
static CAN_message_t rxmsg;

void initializeCANbus(void) {
  pinMode(canStandby, OUTPUT);
  digitalWrite(canStandby, 0); // Ground to enable canbus
  CANbus.begin();
}

// Read all of the CAN Message Id's for the variables desired
void CANLoop() {
  byte readAllCAN = 0;
  while (CANbus.read(rxmsg)) {
    switch (rxmsg.id) { // BaseId = 1520
      case 1520:
        RPM = (uint16_t((rxmsg.buf[7]) | (rxmsg.buf[6] << 8)));
        readAllCAN++;
        break;
      case 1521:
        engine = (rxmsg.buf[4]);
        readAllCAN++;
        break;
      case 1522:
        MAP = ((int16_t((rxmsg.buf[3]) | (rxmsg.buf[2] << 8))) / 10);
        MAT = ((int16_t((rxmsg.buf[5]) | (rxmsg.buf[4] << 8))) / 10);
        CLT = ((int16_t((rxmsg.buf[7]) | (rxmsg.buf[6] << 8))) / 10);
        #if (DEBUG)
          Serial.println("MAP: " + MAP);
          Serial.println("MAT: " + MAT);
          Serial.println("CLT: " + CLT);
        #endif
        readAllCAN++;
        break;
      case 1523:
        TPS = ((int16_t((rxmsg.buf[1]) | (rxmsg.buf[0] << 8))) / 10);
        BATTV = ((int16_t((rxmsg.buf[3]) | (rxmsg.buf[2] << 8))) / 10);
        #if (DEBUG)
          Serial.println("TPS: " + TPS);
          Serial.println("Battery Voltage " + BATTV);
        #endif
        //GSI2 = ((int16_t((rxmsg.buf[5]) | (rxmsg.buf[4] << 8))) / 10);
        readAllCAN++;
        break;
      case 1524:
        knock = ((int16_t((rxmsg.buf[1]) | (rxmsg.buf[0] << 8))) / 10);
        readAllCAN++;
        break;
      case 1567:
        fuel_pct = ((int16_t((rxmsg.buf[1]) | (rxmsg.buf[0] << 8))) / 10);
        readAllCAN++;
        break;
      case 1572: // CanIn1, CanIn2, CanOut - All 1
        break;
      default: // not a broadcast packet
        break;
    }
    if (readAllCAN >= 6)
      break;
  }
}
//construct string
//    String text = "ID: 0x";
//    text = String(text + String(rxmsg.id, HEX));
//    text = String(text + " DLC: ");
//    text = String(text + String(rxmsg.len, HEX));
//    //check if DLC is >0 append string as required
//    if (rxmsg.len >= 1)    {
//      text = String(text + " DATA: ");
//    }
//    //construct string for available bytes (trunctate to DLC to avoid reading garbage)
//    for ( int idx = 0; idx < rxmsg.len; ++idx )    {
//      text = String(text + String(rxmsg.buf[idx], HEX));
//      text = String(text + " ");
//    }
//    Serial.println(text);

//    switch (rxmsg.id) {
//      case 1512:  // MAP, RPM, CLT, TPS
//        MAP = ((int16_t((rxmsg.buf[1]) | (rxmsg.buf[0] << 8))) / 10);
//        rpm = (int16_t((rxmsg.buf[3]) | (rxmsg.buf[2] << 8)));
//        CLT = ((uint16_t((rxmsg.buf[5]) | (rxmsg.buf[4] << 8))) / 10);
//        TPS = (int16_t((rxmsg.buf[7]) | (rxmsg.buf[6] << 8)));
//        readAllCAN++;
//        break;
//      case 1513: // PW1, PW2, MAT, SPK
//        MAT = ((int16_t((rxmsg.buf[5]) | (rxmsg.buf[4] << 8))) / 10);
//        readAllCAN++;
//        break;
//      case 1514: // TGTAFR1, AFR1, EGOcor1, EGT1, PWseq1
//        break;
//      case 1515: // BATT, Sensors1, Sensors2, KnockRetard
//        BATTV = ((int16_t((rxmsg.buf[1]) | (rxmsg.buf[0] << 8))) / 10);
//        GSI1 = ((int16_t((rxmsg.buf[3]) | (rxmsg.buf[2] << 8))) / 10);
//        GSI2 = ((int16_t((rxmsg.buf[5]) | (rxmsg.buf[4] << 8))) / 10);
//        readAllCAN++;
//        break;
//      case 1516: // VSS1, TC_Retard, LaunchTiming
//        break;
//      default: // not a broadcast packet
//    }
