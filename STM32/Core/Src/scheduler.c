/*
 * scheduler.c
 *
 *  Created on: Nov 9, 2022
 *      Author: acer
 */

#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_Init(void){
	for (int i=0; i<SCH_MAX_TASKS; i++){
		SCH_Delete_Task(i);
	}
}

void swap (sTasks *a, sTasks *b){
	sTasks temp = *a;
	*a = *b;
	*b = temp;
}

void SCH_Sort_Task(void){
	// Perform bubble sort algorithm
	uint8_t current = 0;
	uint8_t flag = 0;
	while (current < current_index_task && flag == 0){
		uint8_t walker = current_index_task - 1;
		flag = 1;
		while (walker > current){
			if (SCH_tasks_G[walker].Delay < SCH_tasks_G[walker-1].Delay){
				flag = 0;
				swap(&SCH_tasks_G[walker], &SCH_tasks_G[walker-1]);
			}
			walker -= 1;
		}
		current += 1;
	}

	// Reassign the taskID
	for (int i=0; i<current_index_task; i++){
		SCH_tasks_G[i].TaskID = i;
	}
}


void SCH_Add_Task ( void (*pFunction)() , uint32_t DELAY, uint32_t PERIOD){
	if(current_index_task < SCH_MAX_TASKS){

		SCH_tasks_G[current_index_task].pTask = pFunction;
		SCH_tasks_G[current_index_task].Delay = DELAY;
		SCH_tasks_G[current_index_task].Period =  PERIOD;
		SCH_tasks_G[current_index_task].RunMe = 0;
		SCH_tasks_G[current_index_task].TaskID = current_index_task;

		current_index_task++;

		SCH_Sort_Task();
	}
}

void SCH_Delete_Task (uint32_t taskID){
	for (int i=taskID; i<current_index_task-1; i++){
		SCH_tasks_G[i]=SCH_tasks_G[i+1];
	}

	SCH_tasks_G[current_index_task-1].pTask = 0x0000;
	SCH_tasks_G[current_index_task-1].Delay = 0;
	SCH_tasks_G[current_index_task-1].Period = 0;
	SCH_tasks_G[current_index_task-1].RunMe = 0;

	current_index_task -=1;
}

void SCH_Dispatch_Tasks(void){
	while(SCH_tasks_G[0].pTask != 0x0000){
		if(SCH_tasks_G[0].RunMe > 0){
			SCH_tasks_G[0].RunMe--;
			(*SCH_tasks_G[0].pTask)();
		}

		// Delay of task #1 to #n now is the offset of task #0's delay
		for (int i=0; i<current_index_task; i++){
			SCH_tasks_G[i].Delay -= SCH_tasks_G[0].Delay;
		}

		if (SCH_tasks_G[1].Delay <= 0){
			SCH_tasks_G[1].RunMe += 1;
		}

		if (SCH_tasks_G[0].Period > 0){
			SCH_Add_Task(SCH_tasks_G[0].pTask, SCH_tasks_G[0].Period, SCH_tasks_G[0].Period);
		}
		SCH_Delete_Task(0);
	}
}

void SCH_Update(void){
	if (SCH_tasks_G[0].Delay > 0){
		SCH_tasks_G[0].Delay -= 1;
	}else{
//		SCH_tasks_G[0].Delay = SCH_tasks_G[0].Period;
		SCH_tasks_G[0].RunMe += 1;
	}
}
