/*
 * test.c
 *
 *  Created on: Jun 20, 2025
 *      Author: salman
 */


#include "stm32h7xx_hal.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "drivers/mcu_init.h"
#include "drivers/io.h"
#include "drivers/led.h"
#include "test.h"

SUPPRESS_UNUSED
void test_led()
{

	while(1)
	{
		int n;
		led_set(LED_,LED_STATE_OFF) ;
		 for(n=0 ; n<1000000 ; ++n) {}
		 led_set(LED_,LED_STATE_ON) ;
		 for(n=0 ; n<1000000 ; ++n) {}
	}
}

SUPPRESS_UNUSED
void test_mcu_init()
{

	  mcu_init();
	  ASSERT(0) ;
}




int main(void)
{



    led_init();
   // test_led() ;

    ASSERT(0) ;
    while (1) { }
}




