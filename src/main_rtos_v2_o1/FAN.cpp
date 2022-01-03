#include "FAN.h"
#include "Arduino.h"
#include "_LCD.h"
#include "common.h"
#include "scheduler.h"

#define NO_OF_MODE 5

// Save state of FAN module
int mode = 0;
// Save state when turn off or change to auto mode
int prevMode = 1;
// Save speed value of motor
int FANSpeed = 0;
// Save ID of task task_FAN_update to delete it when turn off fan or out of auto mode
int ID_task_FAN_auto;

// use in auto mode to control speed of motor
extern float temperature;

// Initiaization of module
void init_FAN(){
  pinMode(FAN_PIN, OUTPUT);
  // Display to lcd
  task_FAN_display();
  // Update speed of motor
  task_FAN_update();
}

// Turn ON/OFF fan when button power(2) is pressed
void task_FAN_ON_OFF(){
  if(mode > 0) {
    if(mode == 4) SCH_Delete_Task(ID_task_FAN_auto);
    prevMode = mode;
    mode = 0;
  }
  else if(mode == 0) mode = prevMode;
  task_FAN_display();
  task_FAN_update();
}

// Change mode when button mode(3) is pressed
void task_FAN_increaseMode(){
  if(mode != 0 && mode != 4){
    mode++;
    if(mode > NO_OF_MODE - 2){
      mode = 1;
    }
    prevMode = mode;
    task_FAN_display();
    task_FAN_update();
  }
}

// Change to auto mode when button auto(4) is pressed
void task_FAN_auto(){
  if(mode > 0){
    if(mode != 4){
      mode = 4;
      ID_task_FAN_auto = SCH_Add_Task(task_FAN_update, 20, PERIOD_FAN_AUTO_UPDATE);
    }
    else{
      if(prevMode != 4)mode = prevMode;
      else mode = 1;
      SCH_Delete_Task(ID_task_FAN_auto);
    }
    task_FAN_display();
  }
}

// Update speed of motor base on mode value 
void task_FAN_update(){
  switch(mode){
    case 0: // OFF
      FANSpeed = 0;
      break;
    case 1: // MODE 0
      FANSpeed = MODE_0_SPEED;
      break;
    case 2: // MODE 1
      FANSpeed = MODE_1_SPEED;
      break;
    case 3: // MODE 2
      FANSpeed = MODE_2_SPEED;
      break;
    case 4: // MODE AUTO
      if(temperature < MIN_FAN_TEMPERATURE) FANSpeed = 0;
      else if (temperature > MAX_FAN_TEMPERATURE) FANSpeed = 255;
      else FANSpeed = 255 - (215 * (temperature - MIN_FAN_TEMPERATURE)/(MAX_FAN_TEMPERATURE - MIN_FAN_TEMPERATURE));
      break;
    default:
      break;
  }
  analogWrite(FAN_PIN, FANSpeed);
}
