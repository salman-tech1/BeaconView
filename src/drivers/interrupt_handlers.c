/*
 * interrupt_handlers.c
 *
 *  Created on: Jun 18, 2025
 *      Author: salman
 */


/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32h7xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_func(void);
void DebugMon_Handler(void);
void PendSV_func(void);
//void SysTick_func(void);


void NMI_Handler(void)
{

   while (1)
  {
  }

}


void HardFault_Handler(void)
{

	while (1)
  {
  }
}

void MemManage_Handler(void)
{

  while (1)
  {

  }
}


void BusFault_Handler(void)
{

  while (1)
  {

  }
}


void UsageFault_Handler(void)
{

  while (1)
  {

  }
}

void SVC_func(void)
{

}


void DebugMon_Handler(void)
{

}


void PendSV_func(void)
{

}

//void systick_func(void)
//{
//
//  HAL_IncTick();
//
//}

