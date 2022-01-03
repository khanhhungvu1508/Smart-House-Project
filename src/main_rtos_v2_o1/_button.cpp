#include "Arduino.h"
#include "_button.h"
#include "FAN.h"
#include "common.h"
#include "_RFID.h"
#include "_LED.h"
#include "scheduler.h"

#define PRESSED LOW
#define RELEASED HIGH

#define NUMBER_OF_BUTTONS 5

enum STATE{
  BEING_RELEASED,
  BE_PRESSED,
  BEING_PRESSED,
  BEING_PRESSED_MORE_1,
  BE_PRESSED_MORE_1_INTERVAL,
};

// Array save PIN value of buttons
int buttonPin[NUMBER_OF_BUTTONS] = {BUTTON_1_PIN, BUTTON_2_PIN, BUTTON_3_PIN, BUTTON_4_PIN, BUTTON_5_PIN};

// Array save first debounce of buttons
int buttonDebounce1[NUMBER_OF_BUTTONS];
// Array save second debounce of buttons
int buttonDebounce2[NUMBER_OF_BUTTONS];
// Array save third debounce of buttons
int buttonDebounce3[NUMBER_OF_BUTTONS];
// Array save counter of buttons
int buttonCounter[NUMBER_OF_BUTTONS];
// Array save flag of buttons
STATE buttonFlag[NUMBER_OF_BUTTONS];

// Initiaization of module
void init_button_reading(){
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP);
  pinMode(BUTTON_4_PIN, INPUT_PULLUP);
  pinMode(BUTTON_5_PIN, INPUT_PULLUP);
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    buttonDebounce1[i] = RELEASED;
    buttonDebounce2[i] = RELEASED;
    buttonDebounce3[i] = RELEASED;
    buttonCounter[i] = 0;
    buttonFlag[i] = BEING_RELEASED;
  }
}

// Function get signal from button
void button_reading(){
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    buttonDebounce1[i] = buttonDebounce2[i];
    buttonDebounce2[i] = buttonDebounce3[i];
    buttonDebounce3[i] = digitalRead(buttonPin[i]);
    if(buttonDebounce1[i] == buttonDebounce2[i] && buttonDebounce2[i] == buttonDebounce3[i]){
      if(buttonDebounce1[i] == PRESSED){
        buttonCounter[i]++;
        if(buttonCounter[i] == 1) buttonFlag[i] = BE_PRESSED;
        else buttonFlag[i] = BEING_PRESSED;
        // For button 1
        if(i == 0){
          if((buttonCounter[i] + 1) >= (DURATION_FOR_AUTO_OPERATION_1 / TIMER_CYCLE)){
            buttonFlag[i] = BEING_PRESSED_MORE_1;
            if((((buttonCounter[i] + 1) * TIMER_CYCLE - DURATION_FOR_AUTO_OPERATION_1) % DURATION_FOR_INTERVAL_OPERATION_1) == 0){
              buttonFlag[i] = BE_PRESSED_MORE_1_INTERVAL;
              buttonCounter[i] = (DURATION_FOR_AUTO_OPERATION_1 / TIMER_CYCLE) - 1;
            }
          } 
        }
        // For button 2, 3, 4, 5
        else{
          buttonCounter[i] = 2;
        }
      }
      else{
        buttonCounter[i] = 0;
        buttonFlag[i] = BEING_RELEASED;
      }
    }
  }
}

// function call other functions when have button event
void fsm_button_reading(){
  for(int i = 0; i < NUMBER_OF_BUTTONS; i++){
    switch(buttonFlag[i]){
      case BEING_RELEASED:
        break;
      case BEING_PRESSED:
        break;
      case BE_PRESSED:
        if(i == 0) toggleLED();
        if(i == 1) task_FAN_ON_OFF();
        if(i == 2) task_FAN_increaseMode();
        if(i == 3) task_FAN_auto();
        if(i == 4) changeStateRFID();
        buttonFlag[i] = BEING_PRESSED;
        break;
      case BEING_PRESSED_MORE_1:
        break;
      case BE_PRESSED_MORE_1_INTERVAL:
        toggleLED();
        buttonFlag[i] = BEING_PRESSED_MORE_1;
        break;
      default:
        break;
    }
  }
}
