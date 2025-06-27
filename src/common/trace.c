/*
 * trace.c
 *
 *  Created on: Jun 27, 2025
 *      Author: salman
 */

#ifndef DISABLE_TRACE
#include "common/trace.h"
#include "drivers/uart.h"
#include "common/defines.h"
#include "common/assert_handler.h"
#include "../external/printf/printf.h"
#include "assert.h"
#include "stdint.h"
#include "stdbool.h"

// int type 0 to enable all interrupts
static const struct uart_config cnfg = {
    .baud = BAUD_115200, .isr_stat = INT_ENABLED, .int_type = 0, .prio = PRIO_0
};

static bool initialized = false;
void trace_init(void)
{

    ASSERT(!initialized);
    uart_configure(TRACE, &cnfg);

    initialized = true;
}

void trace(const char *fmt, ...)
{

    // always assert
    ASSERT(initialized);

    va_list args;
    va_start(args, fmt); // Initialize the variable arguments
    vprintf(fmt, args); // Format the string
    va_end(args);
}

// needs fpr mpland printf implementation
void _putchar(char c)
{
    if (cnfg.isr_stat == INT_ENABLED)
        uart_putchar_interrupt(TRACE, c);
    else
        uart_putchar(TRACE, c);
}

#endif
