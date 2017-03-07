void initializeAlternator(){
  pinMode(alternatorPin, OUTPUT);  
}

void SetAlternatorPWM(byte voltage) {
  if ( voltage > 14.9)
    alternatorOut = 0;
  else if (voltage < 14.5)
    alternatorOut = 1;
  digitalWrite(alternatorPin, alternatorOut);
}

