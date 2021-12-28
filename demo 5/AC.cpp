#include "AC.h"
#include "scheduler.h"
#include "Arduino.h"
#include "_LCD.h"

#define AC_PIN 3

#define NO_OF_MODE 3
#define MAX_AC_TEMPERATURE 32
#define MIN_AC_TEMPERATURE 16

int mode = 0;
int ACTemperature = MAX_AC_TEMPERATURE;
int ACSpeed = 0;
int autoTemperature = 20;

extern float temperature;

void task_increaseMode(){
  if(mode == -1) mode = 2;
  else{
    mode++;
    if(mode >= NO_OF_MODE - 1){
      mode = 0;
    }
  }
 SCH_Add_Task(task_AC_display, 0, 0);
}

void task_changeAutoMode(){
  mode = -1;
  SCH_Add_Task(task_AC_display, 0, 0);
  SCH_Add_Task(task_AC_update, 0, 0);
}

void task_decreaseACTemperature(){
  ACTemperature--;
  if(ACTemperature < MIN_AC_TEMPERATURE){
    ACTemperature = MAX_AC_TEMPERATURE;
  }
  SCH_Add_Task(task_AC_display, 0, 0);
  SCH_Add_Task(task_AC_update, 0, 0);
}

void task_increaseACTemperature(){
  ACTemperature++;
  if(ACTemperature > MAX_AC_TEMPERATURE){
    ACTemperature = MIN_AC_TEMPERATURE;
  }
  SCH_Add_Task(task_AC_display, 0, 0);
  SCH_Add_Task(task_AC_update, 0, 0);
}

void task_decreaseACAuto(){
  autoTemperature--;
  if(autoTemperature < MIN_AC_TEMPERATURE){
    autoTemperature = MAX_AC_TEMPERATURE;
  }
  SCH_Add_Task(task_AC_display, 0, 0);
  SCH_Add_Task(task_AC_update, 0, 0);
}

void task_increaseACAuto(){
  autoTemperature++;
  if(autoTemperature > MAX_AC_TEMPERATURE){
    autoTemperature = MIN_AC_TEMPERATURE;
  }
  SCH_Add_Task(task_AC_display, 0, 0);
  SCH_Add_Task(task_AC_update, 0, 0);
}

void init_AC(){
  SCH_Add_Task(task_AC_display, 0, 0);
  SCH_Add_Task(task_AC_update, 0, 0);
}

void task_AC_update(){
  switch(mode){
    case -1:
      ACSpeed = 0;
      break;
    case 0: // OFF
      ACSpeed = 0;
      break;
    case 1: // MANUAL
      ACSpeed = 255 - (205 * (ACTemperature - MIN_AC_TEMPERATURE) / (MAX_AC_TEMPERATURE - MIN_AC_TEMPERATURE));
      break;
    case 2: // AUTO
      if(temperature <= autoTemperature) ACTemperature = autoTemperature;
      else{
         ACTemperature = MIN_AC_TEMPERATURE;
      }
      ACSpeed = 255 - (205 * (ACTemperature - MIN_AC_TEMPERATURE) / (MAX_AC_TEMPERATURE - MIN_AC_TEMPERATURE));
      break;
    default:
      break;
  }
  analogWrite(AC_PIN, ACSpeed);
}
