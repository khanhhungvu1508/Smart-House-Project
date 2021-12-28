/*
 * scheduler.c
 *
 *  Created on: Oct 20, 2021
 *      Author: Khanh
 */
#include "scheduler.h"
#include <stdio.h>

#define MAX_NUMBER_OF_TASK 40
#define TIMER_CYCLE 10

// Don't have RunMe
typedef struct{
  void (*pTask)(void);
  uint32_t Delay;
  uint32_t Period;
  uint32_t TaskID;
} stuctTask;

stuctTask SCH_task_array[MAX_NUMBER_OF_TASK];
static int minDelayCounter = -1;
static int minDelay = -1;
//static int Error_code = 0;

void init_SCH(){
  for(int i=0; i<MAX_NUMBER_OF_TASK; i++){
    SCH_task_array[i].pTask = NULL;
    SCH_task_array[i].Delay = 0;
    SCH_task_array[i].Period = 0;
    SCH_task_array[i].TaskID = 0;
  }
//  Error_code = 0;
}

void SCH_Update(){
  if(minDelayCounter > 0) minDelayCounter--;
}

uint32_t SCH_Add_Task(void(*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
  int i = 0;
  // Find empty element
  while((SCH_task_array[i].pTask != 0) && (i < MAX_NUMBER_OF_TASK)){
    i++;
  }
  // Array is full, so I just end function
  if(i >= MAX_NUMBER_OF_TASK){
    return MAX_NUMBER_OF_TASK;
  }
  // Add task
  SCH_task_array[i].pTask = pFunction ;
  SCH_task_array[i].Delay = DELAY / TIMER_CYCLE;
  SCH_task_array[i].Period = PERIOD / TIMER_CYCLE;
  SCH_task_array[i].TaskID = i;
  // minDelay is not initiated
  if(minDelay < 0){
    minDelayCounter = SCH_task_array[i].Delay;
    minDelay = SCH_task_array[i].Delay;
  }
  // minDelay is initiated and new task's delay is smaller than minDelayCounter
  else if(minDelay >= 0 && SCH_task_array[i].Delay < minDelayCounter){
    // delayed is time the old tasks has waited
    int delayed = minDelay - minDelayCounter;
    // This if use for passing case delayed == 0
    if(delayed > 0){
      for(int i = 0; i < MAX_NUMBER_OF_TASK; i++){
        if(SCH_task_array[i].pTask != 0){
          SCH_task_array[i].Delay -= delayed;
        }
      }
    }
    // Update minDelay, minDelayCounter
    minDelay = SCH_task_array[i].Delay;
    minDelayCounter = SCH_task_array[i].Delay;
  }
  return i;
}

uint32_t SCH_Delete_Task(uint32_t taskID){
  uint32_t returnCode = 1;
  if(SCH_task_array[taskID].pTask == 0){
    returnCode = 99999; // Not finished
  }
  else{
    returnCode = 0; // Not finished
  }
  SCH_task_array[taskID].pTask = 0;
  SCH_task_array[taskID].Delay = 0;
  SCH_task_array[taskID].Period = 0;

  return returnCode;
}


void SCH_Dispatch_Tasks(){
  if(minDelayCounter == 0){
    // Save minDelay
    int tempDelay = minDelay;
    // Reset minDelay
    minDelay = -1;
    minDelayCounter = -1;

    for(int i = 0; i < MAX_NUMBER_OF_TASK; i++){
      if(SCH_task_array[i].pTask != 0){
        // Update delay of task
        SCH_task_array[i].Delay = SCH_task_array[i].Delay - tempDelay;
        // Execute task
        if(SCH_task_array[i].Delay == 0){
          (*SCH_task_array[i].pTask)();
          // Task is execute one time
          if(SCH_task_array[i].Period == 0) {
            SCH_Delete_Task(i);
            continue;
          }
          // Task has period
          else {
            SCH_task_array[i].Delay = SCH_task_array[i].Period;
          }
        }

        // Update minDelayTask
        // minDelay is not initiated
        if(minDelay < 0) {
          minDelayCounter = SCH_task_array[i].Delay;
          minDelay = SCH_task_array[i].Delay;
        }
        // minDelay is initiated
        else{
          if(SCH_task_array[i].Delay < minDelay) {
            minDelayCounter = SCH_task_array[i].Delay;
            minDelay = SCH_task_array[i].Delay;
          }
        }
      }
    }
  }
}

//static int last_Error_code = 0;
//static int Error_tick_count = 0;
//
//void SCH_Report_Status(){
//#ifdef SCH_REPORT_ERROR
//  if(Error_code != last_Error_code){
//    Error_port = 255 - Error_code;
//    last_Error_code = Error_code;
//    if(Error_code != 0){
//      Error_tick_count = 6000;
//    }
//    else{
//      Error_tick_count = 0;
//    }
//  }
//  else{
//    if(Error_tick_count != 0){
//      if(--Error_tick_count == 0){
//        Error_code = 0;
//      }
//    }
//  }
//#endif
//}
