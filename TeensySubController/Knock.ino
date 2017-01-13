//http://playground.arduino.cc/Code/Filters
#include <Filters.h>

float lowFrequency = 7000;
float highFrequency = 6200;

FilterOnePole lowpassFilter( LOWPASS, lowFrequency );
FilterOnePole highpassFilter( HIGHPASS, highFrequency );

void readKnock() {
  lowpassFilter.input(analogRead(knockInPin));
  highpassFilter.input(analogRead(knockInPin));
  float knockOut = lowpassFilter.output() + highpassFilter.output();
#if (DEBUG)
  Serial.print("Knock Output: ");
  Serial.println(knockOut);
#endif
}

