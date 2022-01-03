#include "scheduler.h"
#include "common.h"
#include <stdint.h>
#include "Arduino.h"

#define MAX_NUMBER_OF_TASK 15

typedef struct{
  void (*pTask)(void);
  uint32_t Delay;
  uint32_t Period;
  uint32_t TaskID;
} structTask;

// Array save task in scheduler
structTask SCH_task_array[MAX_NUMBER_OF_TASK];
static int minDelayCounter = -1;
static int minDelay = -1;

// Initialization of module
void init_SCH(){
  unsigned char i = 0;
  for(; i<MAX_NUMBER_OF_TASK; i++){
    SCH_task_array[i].pTask = 0;
    SCH_task_array[i].Delay = 0;
    SCH_task_array[i].Period = 0;
    SCH_task_array[i].TaskID = 0;
  }
}

// Update time of scheduler
void SCH_Update(){
  if(minDelayCounter > 0) minDelayCounter--;
}

// Add task to scheduler
uint32_t SCH_Add_Task(void(*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
  // Save minDelayCounter 
  int tempMinDelayCounter = minDelayCounter;
  // Reset minDelayCounter to prevent SCH_Update update it's value
  minDelayCounter = -1;
  // Find empty element
  unsigned char i = 0;
  for(; i < MAX_NUMBER_OF_TASK; i++){
    if(SCH_task_array[i].pTask == 0) break;
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
    minDelay = SCH_task_array[i].Delay;
  }
  // minDelay is initiated and new task's delay is smaller than minDelayCounter
  else if(minDelay >= 0 && SCH_task_array[i].Delay < tempMinDelayCounter){
    minDelay = SCH_task_array[i].Delay;
    // delayed is time the old tasks have waited
    int delayed = minDelay - tempMinDelayCounter;
    // This if use for passing case delayed == 0
    if(delayed > 0){
      for(int j = 0; j < MAX_NUMBER_OF_TASK; j++){
        if(j != i && SCH_task_array[i].pTask != 0){
          SCH_task_array[i].Delay -= delayed;
        }
      }
    }
  }
  else if(minDelay >= 0 && SCH_task_array[i].Delay > tempMinDelayCounter){
    SCH_task_array[i].Delay += (minDelay - tempMinDelayCounter);
  }
  // Update minDelay, minDelayCounter
  minDelayCounter = minDelay;
  return i;
}

// Delete task from scheduler
uint32_t SCH_Delete_Task(uint32_t taskID){
  if(taskID < 0 || taskID >= MAX_NUMBER_OF_TASK || SCH_task_array[taskID].pTask == 0){
    return 99999;
  }
  SCH_task_array[taskID].pTask = 0;
  SCH_task_array[taskID].Delay = 0;
  SCH_task_array[taskID].Period = 0;
  SCH_task_array[taskID].TaskID = 0;
  return taskID;
}

// Execute task 
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
          minDelay = SCH_task_array[i].Delay;
        }
        // minDelay is initiated
        else{
          if(SCH_task_array[i].Delay < minDelay) {
            minDelay = SCH_task_array[i].Delay;
          }
        }
      }
    }
    minDelayCounter = minDelay;
  }
}
