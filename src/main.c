/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 *
 */
#include "stm32h7xx_hal.h"
#include "common/assert_handler.h"
#include "drivers/mcu_init.h"
#include "drivers/io.h"
#include "drivers/led.h"

void test_led_driver()
{

	   while (1) {
	    	int n ;
	    	led_set(LED_ , LED_STATE_ON ) ;
	    	for(n=0 ; n<1000000 ; ++n) ;
	    	led_set(LED_ , LED_STATE_OFF) ;
	    	for(n=0 ; n<1000000 ; ++n) ;
	    }
}


int main(void)
{

    mcu_init();

   // io_init() ;
    led_init() ;
 //   test_led_driver() ;

    while (1) {

    }
}
