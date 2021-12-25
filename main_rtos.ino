#include "AC.h"
#include "timer.h"
#include "_DHT11.h"
#include "_DHT11_nonblocking.h"
#include "_LCD.h"
#include "_button.h"
#include "common.h"
#include "scheduler.h"
#include "_PIR.h"

float temperature = 0;
float humidity = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  
  Serial.begin(9600);
  
  init_button_reading();
  init_LCD();
  init_timer_interrupt();
  init_timer_software();
  init_DHT11();
  init_AC();
  init_SCH();
  init_PIR();

  SCH_Add_Task(task_measure_environment, 0, 2000);
  SCH_Add_Task(task_DHT11_display, 0, 10000);
  SCH_Add_Task(task_AC_display, 0, 0);
  SCH_Add_Task(task_AC_update, 0, 0);
  SCH_Add_Task(task_PIR_display, 0, 1000);
}

void loop() {
  bidirectional_visitor_counter();
  fsm_button_reading();
  SCH_Dispatch_Tasks();
  
  /*
  button_reading();
  timer_run();
  delay(10);
  */
}
///*
ISR(TIMER1_COMPA_vect){
  button_reading();
  SCH_Update();
  timer_run();
}
//*/
