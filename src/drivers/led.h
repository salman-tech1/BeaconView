/*
 * led.h
 *
 *  Created on: Jun 19, 2025
 *      Author: salman
 */

#ifndef SRC_DRIVERS_LED_H_
#define SRC_DRIVERS_LED_H_

#include "led.h"


typedef enum
{
	LED_,
	MAX_LEDS
}led_e ;

typedef enum
{
	LED_STATE_OFF ,
	LED_STATE_ON ,
}led_state_e;


void led_init() ;
void led_set(led_e led , led_state_e state ) ;


#endif /* SRC_DRIVERS_LED_H_ */
