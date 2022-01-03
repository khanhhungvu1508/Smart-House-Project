#include "_LED.h"
#include "Arduino.h"
#include "common.h"

// Initiaization of module
void init_LED(){
  pinMode(LED_PIN, OUTPUT);
}

// Toggle state of LED
void toggleLED(){
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}
