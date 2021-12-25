#ifndef __SCHEDULER.H
#define __SCHEDULER.H

#include <stdint.h>

void init_SCH();
void SCH_Update();
uint32_t SCH_Add_Task(void(*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
uint32_t SCH_Delete_Task(uint32_t taskID);
void SCH_Dispatch_Tasks();

#endif
