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




int main(void)
{

    mcu_init();
    io_init() ;

    while (1) { }
}
