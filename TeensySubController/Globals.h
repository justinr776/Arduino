#define SIZE_BYTE   8
#define SIZE_INT    16

//Outputs
static const int clutchoutPin = 27;  //CANbus if possible
static const int tractionoutPin = 8;
static const int tachoutPin = 9;
static const int speedout1Pin = 10;
static const int DCCDout = 29;
static const int DBWOut = 0;
static const int powerSteeringPin = 5;
static const int canStandby = 57;

//Inputs
#define oilPressPin A1
#define fuelPressPin A2
#define cltPressPin A3
static const int clutchPin = 24;
static const int brakePin = 25;
static const int handbrakePin = 26;
static const int encoder0PinA = 41;
static const int encoder0PinB = 42;
static const int encoderbutton = 40;
static const int rfPin = 24; //Right Front Wheel
static const int lfPin = 25; //Left Front Wheel
static const int rrPin = 26; //Right Rear Wheel
static const int lrPin = 27; //Left Rear Wheel
static const int knockInPin = 0;

//Variables
byte TC = 0;  //Whether or not traction control is on at all.
byte engine = 0;
double speedavg = 0;
int slip = 0;
int finaldccdpower = 0;
byte mDCCDpower = 0;
byte dccdStatus = 0; // 0 = Auto & 1 = Manual
int powersteeringout = 0;
int SetSpeed = 0;
byte Launch = 0; //Whether or not launch is activated. (Based on Clutch and TPS)
uint16_t RPM = 0;
int16_t MAP = 0;
int16_t MAT = 0;
int16_t CLT = 0;
int16_t TPS = 0;
int16_t BATTV = 0;
int16_t knock = 0;
int16_t fuel_pct = 0;
int oil = 0;
int fuel = 0;
int clt = 0;
byte clutch = 0;
byte brake = 0;
byte handbrake = 0;
byte displayMode = 0;
byte mpg = 0;
int trip = 0;

//Wheel Speed Values
int rfVal = 0;
int lfVal = 0;
int rrVal = 0;
int lrVal = 0;
int favg = 0;
int ravg = 0;
int rr = 0, rf = 0, lf = 0, lr = 0;

//Constants
static const int fansthresh = 50; //Speed for on/off of coolant fans

// I2C
const int MPU_addr = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
