#include "common.h"
#include "timer.h"
#include "Arduino.h"

#define NUMBER_OF_SOFTWARE_TIMER 5

int timerCounter[NUMBER_OF_SOFTWARE_TIMER];
int timerFlag[NUMBER_OF_SOFTWARE_TIMER];

void setTimer(int index, int duration){
  if(index >= NUMBER_OF_SOFTWARE_TIMER) return;
  timerCounter[index] = duration / TIMER_CYCLE;
  timerFlag[index] = 0;
}

int getTimerFlag(int index){
  if(index >= NUMBER_OF_SOFTWARE_TIMER) return -1;
  return timerFlag[index];
}

void resetTimer(int index){
  if(index >= NUMBER_OF_SOFTWARE_TIMER) return;
  timerCounter[index] = 0;
  timerFlag[index] = 0;
}

void timer_run(){
  for(int i = 0; i < NUMBER_OF_SOFTWARE_TIMER; i++){
    if(timerCounter[i] > 0){
      timerCounter[i]--;
      if(timerCounter[i] == 0) timerFlag[i] = 1;
    }
  }
}

void init_timer_software(){
  for(int i = 0; i < NUMBER_OF_SOFTWARE_TIMER; i++){
    setTimer(i, TIMER_CYCLE);
  }
}

void init_timer_interrupt(){
  cli();//stop interrupts
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  /*
    We have initial frequency is 16 * 10^6 Hz
    We need time = 10ms --> Frequency = 100Hz
    We use prescaler = 256 and counter = 625.
    Since counter begin at 0-based index so OCR1A = 624
    Prove:
      We have Freq_out = Freq_init / (prescaler * (counter + 1)) = (16 * 10^6) / (256 * (624 + 1)) = 100Hz
  */
  OCR1A = 624; // = (16 * 10^6) / (100 * 256) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 bits for 256 prescaler
  TCCR1B |= 1 << CS12;  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();//allow interrupts
}
