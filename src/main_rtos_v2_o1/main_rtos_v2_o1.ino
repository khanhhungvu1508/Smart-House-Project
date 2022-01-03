#include "FAN.h"
#include "timer.h"
#include "_DHT11.h"
#include "_LCD.h"
#include "_button.h"
#include "_RFID.h"
#include "_LED.h"
#include "gateway.h"
#include "scheduler.h"
#include "common.h"
#include <stdint.h>

// Save ID of task task_gateway_sending in scheduler
uint32_t ID_task_gateway;

void setup() {
  // Call initialization function of other modules
  init_LCD();
  init_button_reading();
  init_timer_interrupt();
  init_timer_software();
  init_LED();
  init_DHT11();
  init_SCH();
  init_FAN();
  init_RFID();
  init_gateway();

  // Scheduling tasks have period
  SCH_Add_Task(task_measure_environment, 0, PERIOD_DHT11_MEASURE);
  SCH_Add_Task(task_DHT11_display, 10, PERIOD_DHT11_DISPLAY);
  ID_task_gateway = SCH_Add_Task(task_gateway_sending, 20, PERIOD_GATEWAY_SENDING);
}

void loop(){
  // Execute task if out of delay
  SCH_Dispatch_Tasks();
  // Call other functions when have button event
  fsm_button_reading();
  // Finite state machine for RFID module
  RFID_run();
  // Wait server requirement and call functions to do it
  gateway_command_received_parse();
}

// Interrupt service routine function
ISR(TIMER1_COMPA_vect){
  // Read signal from buttons
  button_reading();
  // Update time of software timer 
  timer_run();
  // Update time of scheduler
  SCH_Update();
}
