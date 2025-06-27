/*
 * assert_handler.c
 *
 *  Created on: Jun 19, 2025
 *      Author: salman
 */

#include "stm32h7xx_hal.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "drivers/uart.h"
#include "stdio.h"
#include "string.h"


#define BREAKPOINT asm("BKPT #0");


static void blink_led()
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitTypeDef ledcnfg = { 0 };

    ledcnfg.Mode = GPIO_MODE_OUTPUT_PP;
    ledcnfg.Pin = GPIO_PIN_13;
    ledcnfg.Pull = GPIO_NOPULL;
    ledcnfg.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOC, &ledcnfg);
    while (1) {

           HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 1);
           __delay(20000000) ;
           HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, 0);
           __delay(20000000) ;
       }
}

static void assert_uart(const char *file , int line)
{
	char buffer[256];

	sprintf(buffer, "FILE : %s  LINE : %d \r\n",file,line);

    for (int i = 0; i < strlen(buffer); i++) {
    	 while (USART1->ISR & UART_FLAG_BUSY || !(USART1->ISR & UART_FLAG_TXE)) {
    	        __NOP();
    	    } // Wait until TXE set
          USART1->TDR = buffer[i];  // Transmit each byte
      }


}




void assert_handler( const char *file , int line )
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

    uart_assert_init() ;

    assert_uart(file,line) ;

    blink_led() ;

}



