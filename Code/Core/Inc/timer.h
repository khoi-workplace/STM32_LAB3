/*
 * timer.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"
/*
 * Timer 0 : for clock counter 0
 * Timer 1 : for clock counter 1
 * Timer 2 : for button read
 * Timer 3 : for 7seg scanning
 * Timer 4 : for increase counter_buffer when pressed_1s button2
 * Timer 5 : for blinky led in modify_mode
 * Timer 6 : for blinky DEBUG_LED
 */
#define		N0_OF_TIMERS	6

typedef enum {
	tmr_traffic_clk, 	// 0
	tmr_btn_press,		// 1
	tmr_btn_hold,	// 3
	tmr_seg_scan,		// 2
	tmr_blink_mod_led,	// 4
	tmr_blink_red_led 	// 5
} TIMER_ID_t;

void updateTimerCycle(int prescaler, int period, int clk);
void setTimer(int id, int duration);
void resetTimer(int id);
void timerRun();
int checkTimerFlag(int id);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* INC_TIMER_H_ */
