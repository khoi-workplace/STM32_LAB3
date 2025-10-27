/*
 * display.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include <led_display.h>
#include "input_processing.h"
#include "timer.h"

#define		N0_OF_MODES				4
#define		N0_OF_SEG				4
#define		N0_OF_SEG_PER_DIR		2

int digitSegment[10][7] = { // ACTIVE LOW
	{0,0,0,0,0,0,1}, //0
	{1,0,0,1,1,1,1}, //1
	{0,0,1,0,0,1,0}, //2
	{0,0,0,0,1,1,0}, //3
	{1,0,0,1,1,0,0}, //4
	{0,1,0,0,1,0,0}, //5
	{0,1,0,0,0,0,0}, //6
	{0,0,0,1,1,1,1}, //7
	{0,0,0,0,0,0,0}, //8
	{0,0,0,0,1,0,0} //9
};

LED_STATUS_t led0_status = GREEN;
LED_STATUS_t led1_status = RED;

int led_index = 0;

int red_counter_0 = DEFAULT_RED_COUNTER;
int amber_counter_0 = DEFAULT_AMBER_COUNTER;
int green_counter_0 = DEFAULT_GREEN_COUNTER;
int red_counter_1 = DEFAULT_RED_COUNTER;
int amber_counter_1 = DEFAULT_AMBER_COUNTER;
int green_counter_1 = DEFAULT_GREEN_COUNTER;

void display7SEG_0(int num) {
	HAL_GPIO_WritePin(SEG_A_0_GPIO_Port, SEG_A_0_Pin, digitSegment[num][0]);
	HAL_GPIO_WritePin(SEG_B_0_GPIO_Port, SEG_B_0_Pin, digitSegment[num][1]);
	HAL_GPIO_WritePin(SEG_C_0_GPIO_Port, SEG_C_0_Pin, digitSegment[num][2]);
	HAL_GPIO_WritePin(SEG_D_0_GPIO_Port, SEG_D_0_Pin, digitSegment[num][3]);
	HAL_GPIO_WritePin(SEG_E_0_GPIO_Port, SEG_E_0_Pin, digitSegment[num][4]);
	HAL_GPIO_WritePin(SEG_F_0_GPIO_Port, SEG_F_0_Pin, digitSegment[num][5]);
	HAL_GPIO_WritePin(SEG_G_0_GPIO_Port, SEG_G_0_Pin, digitSegment[num][6]);
}

void display7SEG_1(int num) {
	HAL_GPIO_WritePin(SEG_A_1_GPIO_Port, SEG_A_1_Pin, digitSegment[num][0]);
	HAL_GPIO_WritePin(SEG_B_1_GPIO_Port, SEG_B_1_Pin, digitSegment[num][1]);
	HAL_GPIO_WritePin(SEG_C_1_GPIO_Port, SEG_C_1_Pin, digitSegment[num][2]);
	HAL_GPIO_WritePin(SEG_D_1_GPIO_Port, SEG_D_1_Pin, digitSegment[num][3]);
	HAL_GPIO_WritePin(SEG_E_1_GPIO_Port, SEG_E_1_Pin, digitSegment[num][4]);
	HAL_GPIO_WritePin(SEG_F_1_GPIO_Port, SEG_F_1_Pin, digitSegment[num][5]);
	HAL_GPIO_WritePin(SEG_G_1_GPIO_Port, SEG_G_1_Pin, digitSegment[num][6]);
}

void update7SEG_0(int index) {
	// Turn off all EN0-1
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, OFF);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, OFF);

	display7SEG_0(led_buffer_0[index]);
	switch(index) {
		case 0:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, ON);
			break;
		case 1:
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, ON);
			break;
		default: break;
	}

}

void update7SEG_1(int index) {
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, OFF);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, OFF);

	display7SEG_1(led_buffer_1[index]);
	switch(index) {
		case 0:
			HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, ON);
			break;
		case 1:
			HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, ON);
			break;
		default: break;
	}
}

void clearAllLeds(void) {
	HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, SET);

	HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, SET);
	HAL_GPIO_WritePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin, SET);
	HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, SET);
}

void displayLED_0(int status) {
	HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, OFF);
	HAL_GPIO_WritePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin, OFF);
	HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, OFF);

	switch (status) {
		case RED:
			HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, ON); break;
		case AMBER:
			HAL_GPIO_WritePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin, ON); break;
		case GREEN:
			HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, ON); break;
		default: break;
	}
}

void displayLED_1(int status) {
	HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, OFF);
	HAL_GPIO_WritePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin, OFF);
	HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, OFF);

	switch (status) {
		case RED:
			HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, ON); break;
		case AMBER:
			HAL_GPIO_WritePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin, ON); break;
		case GREEN:
			HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, ON); break;
		default: break;
	}
}

void defaultState(void) {
	mode = NORMAL;

	// RESET: EN0-3
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, OFF);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, OFF);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, OFF);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, OFF);

	// RESET: All attrs of led0
	led0_status = GREEN;
	red_counter_0 = DEFAULT_RED_COUNTER;
	amber_counter_0 = DEFAULT_AMBER_COUNTER;
	green_counter_0 = DEFAULT_GREEN_COUNTER;
	HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, OFF);
	HAL_GPIO_WritePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin, OFF);
	HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, ON);
	updateLedBuffer_0(green_counter_0);

	// RESET: All attrs of led1
	led1_status = RED;
	red_counter_1 = DEFAULT_RED_COUNTER;
	amber_counter_1 = DEFAULT_AMBER_COUNTER;
	green_counter_1 = DEFAULT_GREEN_COUNTER;
	HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, ON);
	HAL_GPIO_WritePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin, OFF);
	HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, OFF);
	updateLedBuffer_1(red_counter_1);

	// Reset counter_buffer
	red_counter_buffer_temp = DEFAULT_RED_COUNTER;
	amber_counter_buffer_temp = DEFAULT_AMBER_COUNTER;
	green_counter_buffer_temp = DEFAULT_GREEN_COUNTER;
	red_counter_buffer = DEFAULT_RED_COUNTER;
	amber_counter_buffer = DEFAULT_AMBER_COUNTER;
	green_counter_buffer = DEFAULT_GREEN_COUNTER;
}

void resetState(void) { // Just in normal mode
	mode = NORMAL;

	// RESET: EN0-3
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, OFF);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, OFF);
	HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, OFF);
	HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, OFF);

	// RESET: All attrs of led0
	led0_status = GREEN;
	red_counter_0 = red_counter_buffer;
	amber_counter_0 = amber_counter_buffer;
	green_counter_0 = green_counter_buffer;
	HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, OFF);
	HAL_GPIO_WritePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin, OFF);
	HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, ON);
	updateLedBuffer_0(green_counter_0);

	// RESET: All attrs of led1
	led1_status = RED;
	red_counter_1 = red_counter_buffer;
	amber_counter_1 = amber_counter_buffer;
	green_counter_1 = green_counter_buffer;
	HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, ON);
	HAL_GPIO_WritePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin, OFF);
	HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, OFF);
	updateLedBuffer_1(red_counter_1);
}

void displayTrafficIdle(void) {
	switch (led0_status) {
		case RED:
			if (red_counter_0 <= 0) {
				red_counter_0 = get_red_counter_buffer(); // Reset for next red state

				// Setup green state
				led0_status = GREEN;

				updateLedBuffer_0(--green_counter_0);
			} else {
				updateLedBuffer_0(--red_counter_0);
			}
			break;
		case AMBER:
			if (amber_counter_0 <= 0) {
				amber_counter_0 = get_amber_counter_buffer();
				led0_status = RED;

				updateLedBuffer_0(--red_counter_0);
			} else {
				updateLedBuffer_0(--amber_counter_0);
			}

			break;
		case GREEN:
			if (green_counter_0 <= 0) {
				green_counter_0 = get_green_counter_buffer();
				led0_status = AMBER;

				updateLedBuffer_0(--amber_counter_0);
			} else {
				updateLedBuffer_0(--green_counter_0);
			}

			break;
		default: break;
	}

	switch (led1_status) {
		case RED:
			if (red_counter_1 <= 0) {
				red_counter_1 = get_red_counter_buffer();
				led1_status = GREEN;

				updateLedBuffer_1(--green_counter_1);
			} else {
				updateLedBuffer_1(--red_counter_1);
			}

			break;
		case AMBER:
			if (amber_counter_1 <= 0) {
				amber_counter_1 = get_amber_counter_buffer();
				led1_status = RED;

				updateLedBuffer_1(--red_counter_1);

			} else {
				updateLedBuffer_1(--amber_counter_1);
			}

			break;
		case GREEN:
			if (green_counter_1 <= 0) {
				green_counter_1 = get_green_counter_buffer();
				led1_status = AMBER;

				updateLedBuffer_1(--amber_counter_1);
			} else {
				updateLedBuffer_1(--green_counter_1);
			}

			break;
		default: break;
	}

	// Display LEDs
	displayLED_0(led0_status);
	displayLED_1(led1_status);
}

void blinkLED(int mode) {
	switch (mode) {
		case MODIFY_RED:
			HAL_GPIO_TogglePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin);
			HAL_GPIO_TogglePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin);
			break;
		case MODIFY_AMBER:
			HAL_GPIO_TogglePin(LED_AMBER_0_GPIO_Port, LED_AMBER_0_Pin);
			HAL_GPIO_TogglePin(LED_AMBER_1_GPIO_Port, LED_AMBER_1_Pin);
			break;
		case MODIFY_GREEN:
			HAL_GPIO_TogglePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin);
			HAL_GPIO_TogglePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin);
			break;
		default: break;
	}
}

void fsm_for_output_processing(void) {
	if (mode == NORMAL) { // Normal mode
	  if (checkTimerFlag(tmr_traffic_clk)) {
		  resetTimer(tmr_traffic_clk);

		  displayTrafficIdle();
	  }
	} else { // Modify mode
	  if (checkTimerFlag(tmr_blink_mod_led)) {
		  resetTimer(tmr_blink_mod_led);

		  blinkLED(mode);
	  }
	}

	// Display 7SEG_LEDs
	if (checkTimerFlag(tmr_seg_scan)) {
		resetTimer(tmr_seg_scan);

		update7SEG_0(led_index);
		update7SEG_1(led_index);

		led_index = (led_index + 1) % N0_OF_SEG_PER_DIR; // Ensure led_index < N0_SEG_PER_DIR
	}
}
