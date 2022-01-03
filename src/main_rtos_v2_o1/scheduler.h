#ifndef INC_SCHEDULER_H_
#define INC_SCHEDULER_H_

#include <stdint.h>

void init_SCH(void);
void SCH_Update(void);
uint32_t SCH_Add_Task(void(*)(), uint32_t, uint32_t);
uint32_t SCH_Delete_Task(uint32_t);
void SCH_Dispatch_Tasks(void);

#endif
