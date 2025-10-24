/*
 * display.h
 *
 *  Created on: Oct 2, 2025
 *      Author: ADMIN
 */

#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

extern int red_counter_0;
extern int amber_counter_0;
extern int green_counter_0;
extern int red_counter_1;
extern int amber_counter_1;
extern int green_counter_1;

typedef enum LED_STATUS {
	RED,
	AMBER,
	GREEN
} LED_STATUS_t;

extern LED_STATUS_t led0_status;
extern LED_STATUS_t led1_status;

// Functions for 7SEGs on direction 0
void display7SEG_0(int num);
void update7SEG_0(int index);

// Functions for 7SEGs on direction 1
void display7SEG_1(int num);
void update7SEG_1(int index);

// Functions for LEDs on direction 0
void displayLED_0(int index);

// Functions for LEDs on direction 1
void displayLED_1(int index);

void trafficCounter(void);
void blinkLED(int mode);
void clearAllLeds(void);
void initState(void);
void resetState(void);
void outputProcessing(void);

#endif /* INC_LED_DISPLAY_H_ */
