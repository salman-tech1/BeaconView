/*
 * assert_handler.c
 *
 *  Created on: Jun 19, 2025
 *      Author: salman
 */


#include "common/assert_handler.h"
#include "stm32h7xx_hal.h"
#define BREAKPOINT    asm("BKPT #0");

void assert_handler(void)
{
	// TODO : Turn off LCD , MOD ,
	// TODO : Trace to console
	// TODO : software Breakpoint
	// TODO : Blink LED

	// check if debugger is attached
	   if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk)
	    {
	        BREAKPOINT
	    }

	  __HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitTypeDef ledcnfg = { 0 };

	ledcnfg.Mode = GPIO_MODE_OUTPUT_PP;
	ledcnfg.Pin = GPIO_PIN_13;
	ledcnfg.Pull = GPIO_NOPULL;
	ledcnfg.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOC, &ledcnfg);

	while(1)
	{

		int n ;
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1) ;
		  for(n=0 ; n<1000000 ; ++n) {}
		    	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0) ;
		    	for(n=0 ; n<1000000 ; ++n) {}

	}


}
