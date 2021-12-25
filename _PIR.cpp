#include<LiquidCrystal.h>
#include "Arduino.h"

#define DETECT_PIN 2
#define IN 4
#define OUT 5

int numPeople = 0;
int detectInOut = 0;
int firstTime = 0;

void init_PIR(){
  pinMode(IN, INPUT_PULLUP);
  pinMode(OUT, INPUT_PULLUP);
  pinMode(DETECT_PIN, OUTPUT);
}

void bidirectional_visitor_counter()
{ 
  if (digitalRead(IN) == 0 && digitalRead(OUT) == 0) {
    firstTime = 0;
  }
  if (digitalRead(IN) == 0 && digitalRead(OUT) == 1) {
    if (detectInOut == 0 && firstTime == 0) {
      if (numPeople > 0)
        numPeople--;
      detectInOut = 1;
      firstTime = 1;
    }
    else
      detectInOut = 0;  
  }
  if (digitalRead(IN) == 1 && digitalRead(OUT) == 0) {
    if (detectInOut == 0 && firstTime == 0) {
      numPeople++;
      detectInOut = 1;
      firstTime = 1;
    }
    else
      detectInOut = 0;  
  }
  if (numPeople > 0)
    digitalWrite(DETECT_PIN, 1);
  else
    digitalWrite(DETECT_PIN, 0);
}
