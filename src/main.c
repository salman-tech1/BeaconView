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


static void test_blink_led(void)
{
    const struct io_config led_config = {
        .Mode = IO_MODE_OUTPUT_PP,
        .resistor = IO_RESISTOR_DISABLED,
        .Alternate = IO_SELECT_GPIO,
        .speed = IO_SPEED_LOW,

    };


    // configure pin
    io_configure(LED_TEST, &led_config);
    io_out_e out = IO_OUT_LOW;

    while (1) {
        int i;
        out = IO_OUT_LOW;
        for (i = 0; i < 1000000; ++i) { }
        io_set_out(LED_TEST, out);
        for (i = 0; i < 1000000; ++i) { }
        out = IO_OUT_HIGH;
        io_set_out(LED_TEST, out);
    }
}


static void test_H7_io_pins_output(void)
{
	const struct io_config output_config =
	{
			 .Mode = IO_MODE_OUTPUT_PP,
			 .resistor = IO_RESISTOR_DISABLED,
			 .Alternate = IO_SELECT_GPIO,
			 .speed = IO_SPEED_LOW,

	};

	// configure pins as output
	for(io_e io = LTDC_VSYNC; io < MAX_PIN ; ++io )
	{
		if(io == RCC_OSC32_IN ||io == RCC_OSC32_OUT || io == RCC_OSC_IN || io == RCC_OSC_OUT )
			continue ;
	    // configure pin
	    io_configure(io, &output_config);
	}

	  while (1) {
	         volatile  int i;

	      	for(io_e io = LTDC_VSYNC; io < MAX_PIN ; ++io )
	      	{

	      		io_set_out(io, IO_OUT_HIGH);
	      		for (i = 0; i < 10000; ++i) { }
		        if(io == RCC_OSC32_IN ||io == RCC_OSC32_OUT || io == RCC_OSC_IN || io == RCC_OSC_OUT )
		        continue ;
		        io_set_out(io, IO_OUT_LOW);
	      	}


	      }
}


static void test_H7_io_pins_input(void)
{
	const struct io_config input_config =
	{
			 .Mode = IO_MODE_INPUT,
			 .resistor = IO_RESISTOR_PULLUP,
			 .Alternate = IO_SELECT_GPIO,
			 .speed = IO_SPEED_LOW,

	};

	const struct io_config output_config =
	{
			 .Mode = IO_MODE_OUTPUT_PP,
			 .resistor = IO_RESISTOR_DISABLED,
			 .Alternate = IO_SELECT_GPIO,
			 .speed = IO_SPEED_LOW,

	};


	 	io_configure(LED_TEST, &output_config);

	    io_configure(AUX_PIN_1, &input_config);


	  while (1) {
	         volatile  int i;



		       if( io_get_input(AUX_PIN_1) == IO_IN_HIGH)
		       {
		    	   io_set_out(LED_TEST, IO_OUT_HIGH) ;
		       }
		       else io_set_out(LED_TEST, IO_OUT_LOW) ;



	      }
}

int main(void)
{

	mcu_init() ;

   // test_blink_led();
	// test_H7_io_pins_output() ;
	test_H7_io_pins_input() ;
    while (1) { }
}

