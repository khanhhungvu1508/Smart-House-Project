#ifndef __BUTTON.H
#define __BUTTON.H

#define PRESSED LOW
#define RELEASED HIGH

#define NUMBER_OF_BUTTONS 1
#define BUTTON_1_PIN 6
#define TIMER_CYCLE 10

#define DURATION_FOR_AUTO_OPERATION_1 2000
#define DURATION_FOR_INTERVAL_OPERATION_1 2000

void init_button_reading();
void button_reading();
void fsm_button_reading();
int print_0();

#endif
