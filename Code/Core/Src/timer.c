/*
 * timer.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "timer.h"
#include "input_reading.h"

/*
 * Timer 0 : for clock counter 0
 * Timer 1 : for clock counter 1
 * Timer 2 : for button read
 * Timer 3 : for 7seg scanning
 * Timer 4 : for increase counter_buffer when pressed_1s button2
 * Timer 5 : for blinky led in modify_mode
 * Timer 6 : for blinky DEBUG_LED
 */

int timer_counter[N0_OF_TIMERS];
int timer_flag[N0_OF_TIMERS];
int TIMER_CYCLE = 10;

void setTimer(int index, int duration) {
	if (index >= N0_OF_TIMERS) return;

	timer_counter[index] = duration / TIMER_CYCLE;
	timer_flag[index] = 0;
}

int checkTimerFlag(int index) {
	if (index >= N0_OF_TIMERS) return 0;
	return (timer_flag[index] == 1);
}

void timerRun() {
	for (int i = 0; i < N0_OF_TIMERS; ++i) {
		if (timer_counter[i] > 0) {
			--timer_counter[i];
			if (timer_counter[i] == 0) timer_flag[i] = 1;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		button_reading();
		timerRun();
	}
}
