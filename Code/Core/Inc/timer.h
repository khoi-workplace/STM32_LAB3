/*
 * timer.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define		N0_OF_TIMERS	10

extern int timer_counter[N0_OF_TIMERS];
extern int timer_flag[N0_OF_TIMERS];

void setTimer(int index, int duration);
void timerRun();
int checkTimerFlag(int index);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);


#endif /* INC_TIMER_H_ */
