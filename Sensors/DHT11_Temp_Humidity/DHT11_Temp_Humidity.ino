#include <dht.h>
#define DHT11PIN 2
dht DHT11;

double Fahrenheit(double celsius) {
  return 1.8 * celsius + 32;
}

double Kelvin(double celsius) {
  return celsius + 273.15;
}

double dewPoint(double celsius, double humidity) {
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

  double VP = pow(10, RHS - 3) * humidity;
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}

double dewPointFast(double celsius, double humidity) {
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  return (b * temp) / (a - temp);
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int chk = DHT11.read11(DHT11PIN);
  Serial.println("\n");
  Serial.print("Read sensor: ");
  switch (chk)  {
    case DHTLIB_OK:
      Serial.println("OK");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.println("Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.println("Time out error");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);
  Serial.print("Temperature (°C): ");
  Serial.println((float)DHT11.temperature, 2);
  Serial.print("Temperature (°F): ");
  Serial.println(Fahrenheit(DHT11.temperature), 2);
  Serial.print("Temperature (°K): ");
  Serial.println(Kelvin(DHT11.temperature), 2);
  Serial.print("Dew Point (°C): ");
  Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));
  Serial.print("Dew PointFast (°C): ");
  Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));
  delay(1000);
}
