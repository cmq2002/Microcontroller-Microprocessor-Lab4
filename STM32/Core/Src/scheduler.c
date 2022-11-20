/*
 * scheduler.c
 *
 *  Created on: Nov 9, 2022
 *      Author: acer
 */

#include "scheduler.h"

sTasks SCH_tasks_G[SCH_MAX_TASKS];
uint8_t current_index_task = 0;

void SCH_MakePlaceForNewTask(uint8_t pos){
	for (uint8_t i = (current_index_task - 1); i>pos; i--){
		SCH_tasks_G[i] = SCH_tasks_G[i-1];
	}
}

void SCH_Add_Task (void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	sTasks newTask;
	uint8_t newTask_index = 0;
	if(current_index_task < SCH_MAX_TASKS){

		newTask.pTask = pFunction;
		newTask.Delay = DELAY;
		newTask.Period =  PERIOD;
		newTask.RunMe = 0;

		newTask.TaskID = current_index_task;

		if (current_index_task == 0){
			SCH_tasks_G[0] = newTask;
			current_index_task++;
			return;
		}

		while(newTask.Delay > SCH_tasks_G[newTask_index].Delay && newTask_index < current_index_task){
			newTask.Delay = newTask.Delay - SCH_tasks_G[newTask_index].Delay;
			newTask_index++;
		}

		current_index_task++;

		SCH_MakePlaceForNewTask(newTask_index);

		SCH_tasks_G[newTask_index] = newTask;

		SCH_tasks_G[newTask_index+1].Delay = SCH_tasks_G[newTask_index+1].Delay - newTask.Delay;
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
			if (backup.Period > 0) SCH_Add_Task(backup.pTask, backup.Period, backup.Period);
		}
	}
}

void SCH_Update(void){
	if (SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;
	else SCH_tasks_G[0].RunMe++;
}

void SCH_Testing(void){
	  SCH_Add_Task(toggleRED, 0, 0);
//	  SCH_Add_Task(toggleGREEN, 5, 0100);
//	  SCH_Add_Task(toggleYELLOW, 10, 150);
//	  SCH_Add_Task(toggleBLUE, 15, 200);
//	  SCH_Add_Task(togglePINK, 20, 250);
}
