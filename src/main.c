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
#include "drivers/uart.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "common/trace.h"

int main(void)
{

    mcu_init();

   // trace_init();
    //TRACE("Salman %d \r\n", 2025);
    while (1) {

        __delay(10000);
    }
}

void systick_func(void)
{

    HAL_IncTick();
}
