/*
 * uart.c
 *
 *  Created on: Jun 23, 2025
 *      Author: salman
 */

#include "stm32h7xx_hal.h"
#include "uart.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "common/ring_buffer.h"
#include "assert.h"
#include "stdint.h"
#include "stdbool.h"

#define UART_BUFFER_SIZE (16)
static uint8_t buffer[UART_BUFFER_SIZE];

static struct ring_buffer tx_buffer = {
    .buff = buffer, .size = sizeof(buffer), .head = 0, .tail = 0
};

static struct ring_buffer rx_buffer = {
    .buff = buffer, .size = sizeof(buffer), .head = 0, .tail = 0
};

static const struct uart_config init_cnfg[MAX_UARTS] = {
    [TRACE] = { .baud = BAUD_115200,
                .isr_stat = INT_DISABLED,
                .int_type = 0,
                .isr_stat = INT_DISABLED,
                .prio = PRIO_0 },
    [MAX485] = { .baud = BAUD_115200,
                 .isr_stat = INT_DISABLED,
                 .int_type = 0,
                 .isr_stat = INT_DISABLED,
                 .prio = PRIO_0 },
    [ESP01] = { .baud = BAUD_115200,
                .isr_stat = INT_DISABLED,
                .int_type = 0,
                .isr_stat = INT_DISABLED,
                .prio = PRIO_0 },
};

static volatile USART_TypeDef *const uart_instance[MAX_UARTS] = { USART1, USART2, USART3 };

static_assert((sizeof(uart_inst_e) == 1), "Unexpected size of enum");

static_assert(MAX_UARTS <= 3, "Please checks UART instances  ");

void uart_configure(uart_inst_e uart_inst, const struct uart_config *config)
{

    UART_HandleTypeDef huart;
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    if (uart_inst == TRACE) {

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
        PeriphClkInitStruct.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            ASSERT(0);

        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

    } else if (uart_inst == MAX485) {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART2;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            ASSERT(0);

        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

    } else if (uart_inst == ESP01) {
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            ASSERT(0);

        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

    // Configuration
    huart.Instance = (USART_TypeDef *)(uart_instance[uart_inst]);
    huart.Init.BaudRate = (uint32_t)config->baud;
    huart.Init.WordLength = UART_WORDLENGTH_8B;
    huart.Init.StopBits = UART_STOPBITS_1;
    huart.Init.Parity = UART_PARITY_NONE;
    huart.Init.Mode = UART_MODE_TX_RX;
    huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart.Init.OverSampling = UART_OVERSAMPLING_16;
    huart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart) != HAL_OK) {
        ASSERT(0);
    }

    // if interrupt are enabled
    if (config->isr_stat == INT_ENABLED) {
        IRQn_Type irqn = USART1_IRQn;

        irqn += (int)uart_inst;
        // Enable interrupt globally
        HAL_NVIC_SetPriority(irqn, (uint32_t)(config->prio), 0);
        HAL_NVIC_EnableIRQ(irqn);

        switch (config->int_type) {
        case TXE_INT:
            __HAL_UART_ENABLE_IT(&huart, UART_IT_TXE);
            break;
        case TC_INT:
            __HAL_UART_CLEAR_IT(&huart, UART_CLEAR_TCF);
            __HAL_UART_CLEAR_FLAG(&huart, UART_CLEAR_TCF);
            __HAL_UART_ENABLE_IT(&huart, UART_IT_TC);

            break;
        case RXNE_INT:
            __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);
            break;
        }
    }

    ///   HAL_UARTEx_DisableFifoMode(&huart);
}

// TODO : wait for character and get it in ISR
static uint16_t call_count = 0;

static bool initialized = 0;
void uart_init(void)
{

    ASSERT(!initialized);
    for (uart_inst_e u = TRACE; u < MAX_UARTS; ++u) {
        uart_configure(u, &init_cnfg[u]);
    }
    initialized = 1;
}

void disable_interrupt(const uart_inst_e inst, const int_type_e typ)
{
    switch (typ) {
    case TXE_INT:
        (uart_instance[inst])->CR1 &= ~(USART_CR1_TXEIE);
        break;
    case TC_INT:
        (uart_instance[inst])->CR1 &= ~(USART_CR1_TCIE);
        break;
    case RXNE_INT:
        (uart_instance[inst])->CR1 &= ~(USART_CR1_RXNEIE);
        break;
    default:
        (uart_instance[inst])->CR1 &= ~(USART_CR1_TXEIE);
        (uart_instance[inst])->CR1 &= ~(USART_CR1_TCIE);
        (uart_instance[inst])->CR1 &= ~(USART_CR1_RXNEIE);
        break;
    }
}

static inline void enable_interrupt(const uart_inst_e inst, const int_type_e typ)
{
    switch (typ) {
    case TXE_INT:
        (uart_instance[inst])->CR1 |= (USART_CR1_TXEIE);
        break;
    case TC_INT:
        (uart_instance[inst])->CR1 |= (USART_CR1_TCIE);
        break;
    case RXNE_INT:
        (uart_instance[inst])->CR1 |= (USART_CR1_RXNEIE);
        break;
    default:
        (uart_instance[inst])->CR1 |= (USART_CR1_TXEIE);
        (uart_instance[inst])->CR1 |= (USART_CR1_TCIE);
        (uart_instance[inst])->CR1 |= (USART_CR1_RXNEIE);
        break;
    }
}

// transmission using uart TDR
static void uart_tx_start(uart_inst_e inst)
{
    rng_stat_e stat;

    if ((stat = ring_buffer_isempty(&tx_buffer)) == R_NOT_EMPTY) {
        const uint8_t c = ring_buffer_peek(&tx_buffer);
        uart_instance[inst]->TDR = c;
    }
}

// start recieving character and retrun
static inline char uart_rx_start(uart_inst_e inst)
{

    // disable interrupt : to keep track also count
    if (call_count > 0)
        --call_count;

    if (call_count == 0) {
        disable_interrupt(TRACE, RXNE_INT);
        enable_interrupt(TRACE, TXE_INT);
    }

    return (char)uart_instance[inst]->RDR;
}

void trace_isr()
{
    rng_stat_e stat;

    uint32_t isr = USART1->ISR; // read ISR register once
    uint32_t cr1 = USART1->CR1; // read CR1 register once for enabled ints

    // check both configuration and status flag
    if ((isr & UART_FLAG_RXNE) && (cr1 & USART_CR1_RXNEIE)) {
        rng_stat_e stat;

        if ((stat = ring_buffer_isfull(&rx_buffer)) == R_NOT_FULL) {
            const volatile char c = uart_rx_start(TRACE);
            // write to ring buffer
            ring_buffer_put(&rx_buffer, (uint8_t)c);
        }

        if (stat == R_FULL)
            ASSERT(0);

        return;
    }

    if ((isr & UART_FLAG_TXE) && (cr1 & USART_CR1_TXEIE)) {

        // if ring buffer is empty stuck here
        if ((stat = ring_buffer_isempty(&tx_buffer)) == R_EMPTY) {
            ASSERT(0);
        }

        // remove from ring buffer
        ring_buffer_get(&tx_buffer);

        // if ring buffer is empty stuck here
        if ((stat = ring_buffer_isempty(&tx_buffer)) == R_NOT_EMPTY) {
            uart_tx_start(TRACE);
        } else {
            disable_interrupt(TRACE, TXE_INT); // TXEI interrupt
            enable_interrupt(TRACE, RXNE_INT);
        }
    }
}

void uart_putchar_interrupt(uart_inst_e inst, char c)
{
    rng_stat_e stat;

    // poll if full
    while ((stat = ring_buffer_isfull(&tx_buffer)) == R_FULL)
        ;

    // disable both interrupts
    disable_interrupt(inst, 0); // TXEI interrupt

    // if buffer is  empty first push recent character
    // why we check because when the UART is busy it wont fires
    // and the character are stays there
    stat = ring_buffer_isempty(&tx_buffer);

    // increment head put another stuff
    ring_buffer_put(&tx_buffer, c);

    if (stat == R_EMPTY) {
        uart_tx_start(TRACE);
    }

    // TXEI interrupt
    enable_interrupt(inst, TXE_INT);
}

void uart_getchar_interrupt(uart_inst_e inst)
{
    rng_stat_e stat;

    // poll if full
    while ((stat = ring_buffer_isfull(&rx_buffer)) == R_FULL)
        ;

    disable_interrupt(inst, 0); // RXEI interrupt

    if (call_count < ((rx_buffer.size) - 2u))
        ++call_count;

    enable_interrupt(inst, RXNE_INT); // RXEI interrupt
}

void uart_putchar(uart_inst_e inst, char c)
{
    USART_TypeDef *uart = (USART_TypeDef *)uart_instance[inst];
    while (uart->ISR & UART_FLAG_BUSY || !(uart->ISR & UART_FLAG_TXE)) {
        __NOP();
    }
    uart->TDR = c;
    __delay(1000);
}

char uart_getchar(uart_inst_e inst)
{
    USART_TypeDef *uart = (USART_TypeDef *)uart_instance[inst];
    while (uart->ISR & UART_FLAG_BUSY || !(uart->ISR & UART_FLAG_RXNE)) {
        __NOP();
    }

    return (char)(uart->RDR);
}

int uart_irq_isreadable()
{
    rng_stat_e stat;

    if ((stat = ring_buffer_isempty(&rx_buffer)) == R_NOT_EMPTY) {
        const uint8_t data = ring_buffer_get(&rx_buffer);
        return (uint8_t)data;
    }

    return -1;
}


// For ASSERT functionality
void uart_assert_init()
{
	static const struct uart_config cnfg_l = {
	    .baud = BAUD_115200, .isr_stat = INT_DISABLED, .int_type = 0, .prio = PRIO_0
	};
    uart_configure(TRACE, &cnfg_l);
}


