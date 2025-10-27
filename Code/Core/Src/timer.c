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

typedef struct {
	int base_counter;
	int counter;
	int flag;
} TIMER_t;

TIMER_t timerList[N0_OF_TIMERS];
int TIMER_CYCLE = 10;

void updateTimerCycle(int prescaler, int period, int clk) {
	TIMER_CYCLE = ((prescaler + 1) * (period + 1) * 1000) / clk;
}

void setTimer(int id, int duration) {
	if (id >= N0_OF_TIMERS) return;

	timerList[id].base_counter = duration / TIMER_CYCLE;
	resetTimer(id);
}

void resetTimer(int id) {
	if (id >= N0_OF_TIMERS) return;

	timerList[id].counter = timerList[id].base_counter;
	timerList[id].flag = 0;
}

int checkTimerFlag(int id) {
	if (id >= N0_OF_TIMERS) return 0;

	return (timerList[id].flag == 1);
}

void timerRun() {
	for (int i = 0; i < N0_OF_TIMERS; ++i) {
		if (timerList[i].counter > 0) {
			--timerList[i].counter;
			if (timerList[i].counter == 0) timerList[i].flag = 1;
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		button_reading();
		timerRun();
	}
}
