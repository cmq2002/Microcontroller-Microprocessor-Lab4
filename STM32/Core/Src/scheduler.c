/*
 * scheduler.c
 *
 *  Created on: Nov 9, 2022
 *      Author: acer
 */

#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;
uint32_t task0_Delay = 0;

void SCH_Swap_Task(sTasks *a, sTasks *b){
	sTasks temp = *a;
	*a = *b;
	*b = temp;
}

void SCH_Sort_Task(){
	// Perform bubble sort algorithm
	uint8_t current = 0;
	uint8_t flag = 0;
	while (current < current_index_task && flag == 0){
		uint8_t walker = current_index_task - 1;
		flag = 1;
		while (walker > current){
			if (SCH_tasks_G[walker].Delay < SCH_tasks_G[walker-1].Delay){
				flag = 0;
				SCH_Swap_Task(&SCH_tasks_G[walker], &SCH_tasks_G[walker-1]);
			}
			walker = walker - 1;
		}
		current = current + 1;
	}

	// Reassign the taskID
	for (int i=0; i<current_index_task; i++){
		SCH_tasks_G[i].TaskID = i;
	}
}

void SCH_Add_Task (void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
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
	for (int i=taskID; i<(current_index_task-1); i++){
		SCH_tasks_G[i]=SCH_tasks_G[i+1];
	}

	SCH_tasks_G[current_index_task-1].pTask = NULL;
	SCH_tasks_G[current_index_task-1].Delay = 0;
	SCH_tasks_G[current_index_task-1].Period = 0;
	SCH_tasks_G[current_index_task-1].RunMe = 0;

	current_index_task--;
}

void SCH_Dispatch_Tasks(void){
	while(SCH_tasks_G[0].pTask != NULL){
		if(SCH_tasks_G[0].RunMe > 0){
			SCH_tasks_G[0].RunMe--;
			(*SCH_tasks_G[0].pTask)();


			// Delay of task #1 to #n now is the offset of task #0's delay
			for (int i=1; i<current_index_task; i++){
				SCH_tasks_G[i].Delay -=  task0_Delay;
			}

			// Task #1 ready to be performed
			if (SCH_tasks_G[1].Delay == 0){
				SCH_tasks_G[1].RunMe++;
			}

			// Backup before remove
			sTasks backup = SCH_tasks_G[0];

			// Remove task #0
			SCH_Delete_Task(0);

			// Sequentially add task #0 back to the ready queue
			// -> ensure that group of tasks will be performed as a forever loop
			SCH_Add_Task(backup.pTask, backup.Period, backup.Period);

			task0_Delay = SCH_tasks_G[0].Delay;
		}
	}
}

void SCH_Update(void){
	if (SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;
	else SCH_tasks_G[0].RunMe++;
}

void SCH_Testing(void){
	  SCH_Add_Task(toggleRED, 0, 50);
	  SCH_Add_Task(toggleGREEN, 5, 100);
	  SCH_Add_Task(toggleYELLOW, 10, 150);
	  SCH_Add_Task(toggleBLUE, 15, 200);
	  SCH_Add_Task(togglePINK, 20, 250);
	  task0_Delay = SCH_tasks_G[0].Delay;
}
