/*
 * input_processing.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#define	DEFAULT_RED_COUNTER		5
#define	DEFAULT_AMBER_COUNTER	2
#define	DEFAULT_GREEN_COUNTER	3

typedef enum MODE{
	AUTOMATIC,
	MANUAL,
	MODIFY_RED,
	MODIFY_AMBER,
	MODIFY_GREEN
} MODE_t;

extern int mode;
extern int free_dir;
extern int red_counter_buffer;
extern int amber_counter_buffer;
extern int green_counter_buffer;
extern int red_counter_buffer_temp;
extern int amber_counter_buffer_temp;
extern int green_counter_buffer_temp;

extern int led_buffer_0[];
extern int led_buffer_1[];

int get_red_counter_buffer(void);
int get_amber_counter_buffer(void);
int get_green_counter_buffer(void);

void updateLedBuffer_0(int num);
void updateLedBuffer_1(int num);

void balanceCounterInMode(int mode);

void fsm_for_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */
