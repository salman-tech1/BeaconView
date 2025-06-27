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
#include "drivers/uart.h"
#include "string.h"

static const struct uart_config cnfg = { .baud = BAUD_115200, .uart_inst = TRACE };

// For interrupt
// static const struct uart_config cnfg = { .baud = BAUD_115200,.isr_stat = INT_ENABLED , .int_type
// = RXNE_INT , .prio = PRIO_0  };
static const struct uart_config cnfg = {
    .baud = BAUD_115200, .isr_stat = INT_ENABLED, .int_type = 0, .prio = PRIO_0
};

SUPPRESS_UNUSED
void test_trace()
{
    mcu_init();

    trace_init();

    TRACE("Salman %d \r\n", 2025);
    while (1) {

        __delay(10000);
    }
}

SUPPRESS_UNUSED
void test_printf()
{

    mcu_init();

    uart_configure(TRACE, &cnfg);
    // _putchar('s')  ;
    while (1) {

        printf("Salman bytes \r\n");
        __delay(10000);
    }
}

SUPPRESS_UNUSED
void test_led()
{

    while (1) {
        int n;
        led_set(LED_, LED_STATE_OFF);
        for (n = 0; n < 1000000; ++n) { }
        led_set(LED_, LED_STATE_ON);
        for (n = 0; n < 1000000; ++n) { }
    }
}

SUPPRESS_UNUSED
void test_mcu_init()
{

    mcu_init();
    ASSERT(0);
}

SUPPRESS_UNUSED
void test_uart_interrupt()
{
    mcu_init();
    uart_configure(TRACE, &cnfg);

    //	uart_putchar_interrupt(TRACE,'c') ;
    //	uart_putchar_interrupt(TRACE,'l') ;
    //	uart_putchar_interrupt(TRACE,'a') ;
    //	uart_putchar_interrupt(TRACE,'s') ;
    //	uart_putchar_interrupt(TRACE,'s') ;
    //	uart_putchar_interrupt(TRACE,'\r') ;
    //	uart_putchar_interrupt(TRACE,'\n') ;
    //  __delay(10000000) ;

    // TODO : it will enable interrupt RXNE
    // TODO : increment count which tracks number of calls

    // number of interrupts increments count
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;
    //   uart_getchar_interrupt(TRACE) ;

    volatile int c = 0;

    while (1) {
        // Blocking test
        // const volatile char c =  uart_getchar(TRACE) ;
        // uart_putchar(TRACE, c) ;

        uart_getchar_interrupt(TRACE);
        // read character when it have data
        if ((c = uart_irq_isreadable()) != -1) {
            uart_putchar_interrupt(TRACE, c); //
            __delay(1000);
        }
    }
}

static volatile int led_toggle = 0;
SUPPRESS_UNUSED
void test_isr()
{
    led_toggle = !led_toggle;
    led_set(LED_, led_toggle);
}

int main(void)
{

    led_init();
    // test_led() ;
    mcu_init();

    led_init();
    const struct io_config but_isr = {
        .Mode = IO_MODE_IT_FALLING,
        .resistor = IO_RESISTOR_PULLUP,
        .speed = IO_SPEED_LOW,
        .Alternate = IO_SELECT_GPIO,

    };

    io_config_interrupt(AUX_PIN_1, &but_isr, PRIO_0, test_isr);
    // io_clear_interrupt(AUX_PIN_1) ;

    ASSERT(0);
    while (1) {

        uart_putchar(TRACE, 'S');
        uart_putchar(TRACE, 'a');
        uart_putchar(TRACE, 'l');
        uart_putchar(TRACE, 'm');
        uart_putchar(TRACE, 'a');
        uart_putchar(TRACE, 'n');

        uart_putchar_interrupt(TRACE, 'c');
        uart_putchar_interrupt(TRACE, 'l');
        uart_putchar_interrupt(TRACE, 'a');
        uart_putchar_interrupt(TRACE, 's');
        uart_putchar_interrupt(TRACE, 's');
        uart_putchar_interrupt(TRACE, '\r');
        uart_putchar_interrupt(TRACE, '\n');
        __delay(10000000);
        __delay(1000);
    }
}
