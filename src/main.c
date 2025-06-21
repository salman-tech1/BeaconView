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
#include "drivers/mcu_init.h"
#include "drivers/io.h"
#include "drivers/led.h"


static volatile int led_toggle = 0 ;
void test_isr()
{
	led_toggle = !led_toggle ;
	led_set(LED_, led_toggle) ;
}

int main(void)
{

    mcu_init();


    led_init();
     const struct io_config but_isr =  {
    		.Mode = IO_MODE_IT_FALLING ,
    		.resistor = IO_RESISTOR_PULLUP ,
			.speed = IO_SPEED_LOW ,
			.Alternate = IO_SELECT_GPIO ,

    };

    io_config_interrupt(AUX_PIN_1, &but_isr, PRIO_0,test_isr) ;
    // io_clear_interrupt(AUX_PIN_1) ;
    while (1) { }
}


