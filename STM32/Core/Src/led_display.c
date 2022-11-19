/*
 * led_display.c
 *
 *  Created on: Nov 18, 2022
 *      Author: acer
 */
#include "led_display.h"

void toggleRED(){
	HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
}

void toggleGREEN(){
	HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
}

void toggleYELLOW(){
	HAL_GPIO_TogglePin(YELLOW_GPIO_Port, YELLOW_Pin);
}

void toggleBLUE(){
	HAL_GPIO_TogglePin(BLUE_GPIO_Port, BLUE_Pin);
}

void togglePINK(){
	HAL_GPIO_TogglePin(PINK_GPIO_Port, PINK_Pin);
}
