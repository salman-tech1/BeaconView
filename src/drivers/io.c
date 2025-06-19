/*
 * io.c
 *
 *  Created on: Jun 16, 2025
 *      Author: salman
 */

#include "stm32h7xx_hal.h"
#include "drivers/io.h"
#include "common/defines.h"

// This define is a helper macro to encode port and pin bits into a variable
#define ENCODE_IO(port, pin) ((port << MAXPINBITS) | (pin & 0x0F))

// it's a helper headers provide variables/defines for accessing registers
static volatile GPIO_TypeDef *const ports[MAXPORTS] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE,
                                                        GPIOF, GPIOG, GPIOH, GPIOI };

// TODO : Encode tempelate
static const uint8_t io_pin_map[MAX_PIN] = {

    [LTDC_VSYNC] = ENCODE_IO(PORTI, IO_9),
    [LTDC_HSYNC] = ENCODE_IO(PORTI, IO_10),
    [LTDC_DE] = ENCODE_IO(PORTF, IO_10),
    [LTDC_G4] = ENCODE_IO(PORTH, IO_15),
    [LTDC_R3] = ENCODE_IO(PORTH, IO_9),
    [LTDC_R4] = ENCODE_IO(PORTH, IO_10),
    [LTDC_R5] = ENCODE_IO(PORTH, IO_11),
    [LTDC_R6] = ENCODE_IO(PORTH, IO_12),
    [LTDC_R7] = ENCODE_IO(PORTG, IO_6),
    [LTDC_CLK] = ENCODE_IO(PORTG, IO_7),
    [LTDC_B3] = ENCODE_IO(PORTA, IO_8),
    [LTDC_G2] = ENCODE_IO(PORTH, IO_13),
    [LTDC_G5] = ENCODE_IO(PORTI, IO_0),
    [LTDC_G6] = ENCODE_IO(PORTI, IO_1),
    [LTDC_G7] = ENCODE_IO(PORTI, IO_2),
    [LTDC_G3] = ENCODE_IO(PORTG, IO_10),
    [LTDC_B4] = ENCODE_IO(PORTG, IO_12),
    [LTDC_B5] = ENCODE_IO(PORTI, IO_5),
    [LTDC_B6] = ENCODE_IO(PORTI, IO_6),
    [LTDC_B7] = ENCODE_IO(PORTI, IO_7),
    [LCD_RST] = ENCODE_IO(PORTE, IO_2),
    [LCD_BL] = ENCODE_IO(PORTH, IO_6),
    [OUT_PE3] = ENCODE_IO(PORTE, IO_3),
    // TOUCH_SENS
    [TOUCH_SDA] = ENCODE_IO(PORTD, IO_13),
    [TOUCH_SCL] = ENCODE_IO(PORTD, IO_12),
    [GT_RESET] = ENCODE_IO(PORTH, IO_7),
    // TEMP_SENS
    [TEMP_SDA] = ENCODE_IO(PORTB, IO_11),
    [TEMP_SCL] = ENCODE_IO(PORTH, IO_4),
    // SDCARD
    [SD_D0] = ENCODE_IO(PORTB, IO_14),
    [SD_CLK] = ENCODE_IO(PORTD, IO_6),
    [SD_CMD] = ENCODE_IO(PORTD, IO_7),
    // RCC_OSC
    [RCC_OSC32_IN] = ENCODE_IO(PORTC, IO_14),
    [RCC_OSC32_OUT] = ENCODE_IO(PORTC, IO_15),
    [RCC_OSC_IN] = ENCODE_IO(PORTH, IO_0),
    [RCC_OSC_OUT] = ENCODE_IO(PORTH, IO_1),
    // QSPI_FLASH
    [QSPI_CLK] = ENCODE_IO(PORTB, IO_2),
    [QSPI_NCS] = ENCODE_IO(PORTB, IO_10),
    [QSPI_IO3] = ENCODE_IO(PORTF, IO_6),
    [QSPI_IO2] = ENCODE_IO(PORTF, IO_7),
    [QSPI_IO0] = ENCODE_IO(PORTF, IO_8),
    [QSPIO_IO1] = ENCODE_IO(PORTF, IO_9),
    // CAN INVERTER
    [CAN_RX] = ENCODE_IO(PORTA, IO_11),
    [CAN_TX] = ENCODE_IO(PORTA, IO_12),
    // SDRAM ,
    [SDRAM_D0] = ENCODE_IO(PORTD, IO_14),
    [SDRAM_D1] = ENCODE_IO(PORTD, IO_15),
    [SDRAM_D2] = ENCODE_IO(PORTD, IO_0),
    [SDRAM_D3] = ENCODE_IO(PORTD, IO_1),
    [SDRAM_D4] = ENCODE_IO(PORTE, IO_7),
    [SDRAM_D5] = ENCODE_IO(PORTE, IO_8),
    [SDRAM_D6] = ENCODE_IO(PORTE, IO_9),
    [SDRAM_D7] = ENCODE_IO(PORTE, IO_10),
    [SDRAM_D8] = ENCODE_IO(PORTE, IO_11),
    [SDRAM_D9] = ENCODE_IO(PORTE, IO_12),
    [SDRAM_D10] = ENCODE_IO(PORTE, IO_13),
    [SDRAM_D11] = ENCODE_IO(PORTE, IO_14),
    [SDRAM_D12] = ENCODE_IO(PORTE, IO_15),
    [SDRAM_D13] = ENCODE_IO(PORTD, IO_8),
    [SDRAM_D14] = ENCODE_IO(PORTD, IO_9),
    [SDRAM_D15] = ENCODE_IO(PORTD, IO_10),

    [SDRAM_A0] = ENCODE_IO(PORTF, IO_0),
    [SDRAM_A1] = ENCODE_IO(PORTF, IO_1),
    [SDRAM_A2] = ENCODE_IO(PORTF, IO_2),
    [SDRAM_A3] = ENCODE_IO(PORTF, IO_3),
    [SDRAM_A4] = ENCODE_IO(PORTF, IO_4),
    [SDRAM_A5] = ENCODE_IO(PORTF, IO_5),
    [SDRAM_A6] = ENCODE_IO(PORTF, IO_12),
    [SDRAM_A7] = ENCODE_IO(PORTF, IO_13),
    [SDRAM_A8] = ENCODE_IO(PORTF, IO_14),
    [SDRAM_A9] = ENCODE_IO(PORTF, IO_15),
    [SDRAM_A10] = ENCODE_IO(PORTG, IO_0),
    [SDRAM_A11] = ENCODE_IO(PORTG, IO_1),
    [SDRAM_A12] = ENCODE_IO(PORTG, IO_2),

    [SDRAM_NBL0] = ENCODE_IO(PORTE, IO_0),
    [SDRAM_NBL1] = ENCODE_IO(PORTE, IO_1),

    [SDRAM_BA0] = ENCODE_IO(PORTG, IO_4),
    [SDRAM_BA1] = ENCODE_IO(PORTG, IO_5),

    [SDRAM_SDCLK] = ENCODE_IO(PORTG, IO_8),
    [SDRAM_SDNCAS] = ENCODE_IO(PORTG, IO_15),
    [SDRAM_SDNRAS] = ENCODE_IO(PORTF, IO_11),
    [SDRAM_SDNCKE0] = ENCODE_IO(PORTH, IO_2),
    [SDRAM_SDNE0] = ENCODE_IO(PORTH, IO_3),
    [SDRAM_SDNWE] = ENCODE_IO(PORTH, IO_5),

    // UART PRINTF
    [TRACE_TX] = ENCODE_IO(PORTA, IO_9),
    [TRACE_RX] = ENCODE_IO(PORTA, IO_10),
    // ESP01
    [ESP_TX] = ENCODE_IO(PORTA, IO_2),
    [ESP_RX] = ENCODE_IO(PORTA, IO_3),
    // MAX485
    [MAX_TX] = ENCODE_IO(PORTC, IO_10),
    [MAX_RX] = ENCODE_IO(PORTC, IO_11),
    // LED TEST
    [LED_TEST] = ENCODE_IO(PORTC, IO_13),
    // AUX PINS
    [AUX_PIN_1] = ENCODE_IO(PORTB, IO_12),
    [AUX_PIN_2] = ENCODE_IO(PORTE, IO_5),
    [AUX_PIN_3] = ENCODE_IO(PORTE, IO_6),
};

// return pointer to PORTS : GPIOA
static inline GPIO_TypeDef *io_port(io_e io)
{
    // if (io >= MAX_PIN) return NULL ;
    volatile uint8_t enc = io_pin_map[io];
    volatile uint8_t prt_indx = (((enc) >> (4u)) & (0xf));

    return ports[prt_indx];
}

// return pointer to PORTS : GPIOA
static inline uint16_t io_pin(io_e io)
{

    volatile uint8_t enc = io_pin_map[io];

    volatile uint8_t pin_idx = ((enc) & (0xf));

    return (1u << pin_idx);
}

static uint32_t io_mode_to_hal(io_mode_e mode)
{
    switch (mode) {
    case IO_MODE_INPUT:
        return GPIO_MODE_INPUT;
    case IO_MODE_OUTPUT_PP:
        return GPIO_MODE_OUTPUT_PP;
    case IO_MODE_OUTPUT_OD:
        return GPIO_MODE_OUTPUT_OD;
    case IO_MODE_AF_PP:
        return GPIO_MODE_AF_PP;
    case IO_MODE_AF_OD:
        return GPIO_MODE_AF_OD;
    case IO_MODE_ANALOG:
        return GPIO_MODE_ANALOG;
    case IO_MODE_IT_RISING:
        return GPIO_MODE_IT_RISING;
    case IO_MODE_IT_FALLING:
        return GPIO_MODE_IT_FALLING;
    case IO_MODE_IT_RISING_FALLING:
        return GPIO_MODE_IT_RISING_FALLING;
    case IO_MODE_EVT_RISING:
        return GPIO_MODE_EVT_RISING;
    case IO_MODE_EVT_FALLING:
        return GPIO_MODE_EVT_FALLING;
    case IO_MODE_EVT_RISING_FALLING:
        return GPIO_MODE_EVT_RISING_FALLING;
    default:
        return GPIO_MODE_INPUT; // fallback
    }
}

void io_configure(io_e io, const struct io_config *config)
{
    GPIO_InitTypeDef pincnfg = { 0 };


    io_mode_e mod_l = config->Mode;
    io_speed_e speed_l = config->speed;
    io_select_e Alt_l = config->Alternate;
    io_resistor_e res_lr = config->resistor;

    GPIO_TypeDef *port = io_port(io);

    volatile uint16_t pin = io_pin(io);
    volatile uint32_t mod = io_mode_to_hal(mod_l);

    pincnfg.Mode = mod;
    pincnfg.Pin = pin ;
    pincnfg.Pull = res_lr ;
    pincnfg.Speed = speed_l ;
    pincnfg.Alternate = Alt_l ;
    HAL_GPIO_Init(port, &pincnfg) ;
}

io_in_e io_get_input(io_e io)
{
    GPIO_TypeDef *port = io_port(io);
    uint16_t pin = io_pin(io);

    return HAL_GPIO_ReadPin(port, pin) ? IO_IN_HIGH : IO_IN_LOW;
}

void io_set_out(io_e io, io_out_e out)
{
    GPIO_TypeDef *port = io_port(io);
    uint16_t pin = io_pin(io);
    HAL_GPIO_WritePin(port, pin, out);
}
