/*
 * input_processing.c
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#include "main.h"
#include "input_reading.h"
#include "timer.h"
#include "input_processing.h"
#include "led_display.h"

int button1_toggle_flag = 0;
int button2_toggle_flag = 0;
int button3_toggle_flag = 0;
int button4_toggle_flag = 0;

ButtonState_t button1State = BUTTON_RELEASED;
ButtonState_t button2State = BUTTON_RELEASED;
ButtonState_t button3State = BUTTON_RELEASED;
ButtonState_t button4State = BUTTON_RELEASED;

int led_buffer_0[2] = {0, 0};
int led_buffer_1[2] = {0, 0};

int N0_OF_MODES = 4;

int mode = NORMAL;

int red_counter_buffer = DEFAULT_RED_COUNTER;
int amber_counter_buffer = DEFAULT_AMBER_COUNTER;
int green_counter_buffer = DEFAULT_GREEN_COUNTER;

int red_counter_buffer_temp = DEFAULT_RED_COUNTER;
int amber_counter_buffer_temp = DEFAULT_AMBER_COUNTER;
int green_counter_buffer_temp = DEFAULT_GREEN_COUNTER;

void updateLedBuffer_0(int num) {
	led_buffer_0[0] = num / 10;
	led_buffer_0[1] = num % 10;
}

void updateLedBuffer_1(int num) {
	led_buffer_1[0] = num / 10;
	led_buffer_1[1] = num % 10;
}

int get_red_counter_buffer(void) {
	return red_counter_buffer;
}

int get_amber_counter_buffer(void) {
	return amber_counter_buffer;
}

int get_green_counter_buffer(void) {
	return green_counter_buffer;
}

void balanceCounterInMode(int mode) {
	// Priority: RED -> GREEN -> AMBER & Ensure not change the lastest modified_color counter
	switch (mode) {
		case MODIFY_RED:
			green_counter_buffer = red_counter_buffer - amber_counter_buffer;
			break;
		case MODIFY_AMBER:
			green_counter_buffer = red_counter_buffer - amber_counter_buffer;
			break;
		case MODIFY_GREEN:
			if (green_counter_buffer >= red_counter_buffer) {
				red_counter_buffer = green_counter_buffer + amber_counter_buffer;
			} else {
				amber_counter_buffer = red_counter_buffer - green_counter_buffer;
			}
			break;
		default:
			break;
	}

	if (red_counter_buffer < 0 || amber_counter_buffer < 0 || green_counter_buffer < 0) {
		red_counter_buffer = DEFAULT_RED_COUNTER;
		amber_counter_buffer = DEFAULT_AMBER_COUNTER;
		green_counter_buffer = DEFAULT_GREEN_COUNTER;
	}

	// Update temp buffer
	red_counter_buffer_temp = red_counter_buffer;
	amber_counter_buffer_temp = amber_counter_buffer;
	green_counter_buffer_temp = green_counter_buffer;
}

void fsm_for_input_processing(void) {
	// Input_processing for button 1
	switch (button1State) {
		case BUTTON_RELEASED:
			if (is_button_pressed(BUTTON_FOR_MODE)) {
				button1State = BUTTON_PRESSED;
			}
			else button1_toggle_flag = 0;
			break;
		case BUTTON_PRESSED:
			if (checkTimerFlag(2) && !button1_toggle_flag) {
				setTimer(2, 50);
				button1_toggle_flag = 1;

				if (mode == N0_OF_MODES - 1) {
					// Prepare for next NORMAL mode
					led0_status = GREEN;
					led1_status = RED;
					red_counter_0 = red_counter_buffer;
					red_counter_1 = red_counter_buffer;
					amber_counter_0 = amber_counter_buffer;
					amber_counter_1 = amber_counter_buffer;
					green_counter_0 = green_counter_buffer;
					green_counter_1 = green_counter_buffer;
					setTimer(0, 1000);
					setTimer(1, 1000);
				}

				mode = (mode + 1) % N0_OF_MODES;
				if (mode != 0) {
					updateLedBuffer_0(mode);
					switch (mode) {
						case MODIFY_RED:
							updateLedBuffer_1(red_counter_buffer_temp = red_counter_buffer); break;
						case MODIFY_AMBER:
							updateLedBuffer_1(amber_counter_buffer_temp = amber_counter_buffer); break;
						case MODIFY_GREEN:
							updateLedBuffer_1(green_counter_buffer_temp = green_counter_buffer); break;
						default: break;
					}
				}
				clearAllLeds();
			}

			// Change state
			if (!is_button_pressed_1s(BUTTON_FOR_MODE)) {
				button1State = BUTTON_RELEASED;
			} else {
				if (is_button_pressed_1s(BUTTON_FOR_MODE)) {
					button1State = BUTTON_PRESSED_MORE_THAN_1S;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1S:
			if (!is_button_pressed(BUTTON_FOR_MODE)) {
				button1State = BUTTON_RELEASED;
			}
			break;
		default: break;
	}

	// Input_processing for button 2
	switch (button2State) {
		case BUTTON_RELEASED:
			if (is_button_pressed(BUTTON_FOR_MODIFY)) {
				button2State = BUTTON_PRESSED;
				if (checkTimerFlag(2) && !button2_toggle_flag) {
					setTimer(2, 50);
					button2_toggle_flag = 1; // Ensure increase only one time

					switch (mode) {
						case MODIFY_RED: // Change time duration of red LEDs
							red_counter_buffer_temp = (red_counter_buffer_temp + 1) % 100;
							updateLedBuffer_1(red_counter_buffer_temp);
							break;
						case MODIFY_AMBER: // Change time duration of amber LEDs
							amber_counter_buffer_temp = (amber_counter_buffer_temp + 1) % 100;
							updateLedBuffer_1(amber_counter_buffer_temp);
							break;
						case MODIFY_GREEN: // Change time duration of green LEDs
							green_counter_buffer_temp = (green_counter_buffer_temp + 1) % 100;
							updateLedBuffer_1(green_counter_buffer_temp);
							break;
						default: break;
					}
				}
			}
			else button2_toggle_flag = 0;
			break;
		case BUTTON_PRESSED:
			// Change state
			if (!is_button_pressed_1s(BUTTON_FOR_MODIFY)) {
				button2State = BUTTON_RELEASED;
			} else {
				if (is_button_pressed_1s(BUTTON_FOR_MODIFY)) {
					button2State = BUTTON_PRESSED_MORE_THAN_1S;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1S:
			// Change state
			if (!is_button_pressed(BUTTON_FOR_MODIFY)) {
				button2State = BUTTON_RELEASED;
			}

			if (checkTimerFlag(4)) {
				setTimer(4, 500);
				switch (mode) {
					case MODIFY_RED: // Change time duration of red LEDs
						red_counter_buffer_temp = (red_counter_buffer_temp + 1) % 100;
						updateLedBuffer_1(red_counter_buffer_temp);
						break;
					case MODIFY_AMBER: // Change time duration of amber LEDs
						amber_counter_buffer_temp = (amber_counter_buffer_temp + 1) % 100;
						updateLedBuffer_1(amber_counter_buffer_temp);
						break;
					case MODIFY_GREEN: // Change time duration of green LEDs
						green_counter_buffer_temp = (green_counter_buffer_temp + 1) % 100;
						updateLedBuffer_1(green_counter_buffer_temp);
						break;
					default: break;
				}
			}
			break;
		default: break;
	}

	// Input_processing for button 3
	switch (button3State) {
		case BUTTON_RELEASED:
			if (is_button_pressed(BUTTON_FOR_CONFIRM)) {
				button3State = BUTTON_PRESSED;
			}
			else button3_toggle_flag = 0;
			break;
		case BUTTON_PRESSED:
			if (checkTimerFlag(2) && !button3_toggle_flag) {
				setTimer(2, 50);

				button3_toggle_flag = 1;

				switch (mode) {
					case MODIFY_RED:
						red_counter_buffer = red_counter_buffer_temp;
						balanceCounterInMode(1);
						break;
					case MODIFY_AMBER:
						amber_counter_buffer = amber_counter_buffer_temp;
						balanceCounterInMode(2);
						break;
					case MODIFY_GREEN:
						green_counter_buffer = green_counter_buffer_temp;
						balanceCounterInMode(3);
						break;
					default: break;
				}
			}

			// Change state
			if (!is_button_pressed_1s(BUTTON_FOR_CONFIRM)) {
				button3State = BUTTON_RELEASED;
			} else {
				if (is_button_pressed_1s(BUTTON_FOR_CONFIRM)) {
					button3State = BUTTON_PRESSED_MORE_THAN_1S;
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1S:
			if (!is_button_pressed(BUTTON_FOR_CONFIRM)) {
				button3State = BUTTON_RELEASED;
			}
			break;
		default: break;
	}

	// Input_processing for button 4
	/*
	 * Button 4 function:
	 * - If pressed: in reset mode
	 * */

	switch (button4State) {
		case BUTTON_RELEASED:
			if (is_button_pressed(BUTTON_FOR_RESET)) {
				button4State = BUTTON_PRESSED;
				switch (mode) {
					case NORMAL:
						resetState();
					case MODIFY_RED:
						red_counter_buffer_temp = red_counter_buffer;
						updateLedBuffer_1(red_counter_buffer_temp);
						break;
					case MODIFY_AMBER:
						amber_counter_buffer_temp = amber_counter_buffer;
						updateLedBuffer_1(amber_counter_buffer_temp);
						break;
					case MODIFY_GREEN:
						green_counter_buffer_temp = green_counter_buffer;
						updateLedBuffer_1(green_counter_buffer_temp);
						break;
					default: break;
				}
			}
			break;
		case BUTTON_PRESSED:
			if (!is_button_pressed_1s(BUTTON_FOR_RESET)) {
				button4State = BUTTON_RELEASED;
			} else {
				if (is_button_pressed_1s(BUTTON_FOR_RESET)) {
					button4State = BUTTON_PRESSED_MORE_THAN_1S;
					initState();
				}
			}
			break;
		case BUTTON_PRESSED_MORE_THAN_1S:
			if (!is_button_pressed(BUTTON_FOR_RESET)) {
				button4State = BUTTON_RELEASED;
			}
			break;
		default: break;
	}
}
