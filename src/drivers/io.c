/*
 * io.c
 *
 *  Created on: Jun 16, 2025
 *      Author: salman
 */

#include "stm32h7xx_hal.h"
#include "drivers/io.h"
#include "common/defines.h"
#include <assert.h>

// This define is a helper macro to encode port and pin bits into a variable

// it's a helper headers provide variables/defines for accessing registers
static volatile GPIO_TypeDef *const ports[MAXPORTS] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE,
                                                        GPIOF, GPIOG, GPIOH, GPIOI };

static io_callback io_callbacks[IO_MAX];

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

//
static const struct io_config initial_configs[MAX_PIN] = {

    // LTDC
    [LTDC_VSYNC] = { .Mode = IO_MODE_AF_PP,
                     .speed = IO_SPEED_LOW,
                     .resistor = IO_RESISTOR_DISABLED,
                     .Alternate = IO_SELECT_ALT14 },
    [LTDC_HSYNC] = { .Mode = IO_MODE_AF_PP,
                     .speed = IO_SPEED_LOW,
                     .resistor = IO_RESISTOR_DISABLED,
                     .Alternate = IO_SELECT_ALT14 },
    [LTDC_DE] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_G4] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_R3] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_R4] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_R5] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_R6] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_R7] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_CLK] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT14 },
    [LTDC_B3] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT13 },
    [LTDC_G2] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_G5] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_G6] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_G7] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_G3] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT9 },
    [LTDC_B4] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_V_HIGH,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT9 },
    [LTDC_B5] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_B6] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LTDC_B7] = { .Mode = IO_MODE_AF_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_DISABLED,
                  .Alternate = IO_SELECT_ALT14 },
    [LCD_RST] = { .Mode = IO_MODE_OUTPUT_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_PULLUP,
                  .Alternate = IO_SELECT_GPIO },
    [LCD_BL] = { .Mode = IO_MODE_OUTPUT_PP,
                 .speed = IO_SPEED_LOW,
                 .resistor = IO_RESISTOR_PULLUP,
                 .Alternate = IO_SELECT_GPIO },
    [OUT_PE3] = { .Mode = IO_MODE_OUTPUT_PP,
                  .speed = IO_SPEED_LOW,
                  .resistor = IO_RESISTOR_PULLUP,
                  .Alternate = IO_SELECT_GPIO },
    // TOUCH_SENS
    [TOUCH_SDA] = { .Mode = IO_MODE_AF_OD,
                    .speed = IO_SPEED_LOW,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT4 },
    [TOUCH_SCL] = { .Mode = IO_MODE_AF_OD,
                    .speed = IO_SPEED_LOW,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT4 },
    [GT_RESET] = { .Mode = IO_MODE_OUTPUT_PP,
                   .speed = IO_SPEED_LOW,
                   .resistor = IO_RESISTOR_PULLUP,
                   .Alternate = IO_SELECT_GPIO },
    // TEMP_SENS
    [TEMP_SDA] = { .Mode = IO_MODE_AF_OD,
                   .speed = IO_SPEED_LOW,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT4 },
    [TEMP_SCL] = { .Mode = IO_MODE_AF_OD,
                   .speed = IO_SPEED_LOW,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT4 },
    // SDCARD
    [SD_D0] = { .Mode = IO_MODE_AF_PP,
                .speed = IO_SPEED_V_HIGH,
                .resistor = IO_RESISTOR_DISABLED,
                .Alternate = IO_SELECT_ALT9 },
    [SD_CLK] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_V_HIGH,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT11 },
    [SD_CMD] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_V_HIGH,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT11 },
    // RCC_OSC
    [RCC_OSC32_IN] = { .Mode = IO_UNUSED,
                       .speed = IO_UNUSED,
                       .resistor = IO_UNUSED,
                       .Alternate = IO_UNUSED },
    [RCC_OSC32_OUT] = { .Mode = IO_UNUSED,
                        .speed = IO_UNUSED,
                        .resistor = IO_UNUSED,
                        .Alternate = IO_UNUSED },
    [RCC_OSC_IN] = { .Mode = IO_UNUSED,
                     .speed = IO_UNUSED,
                     .resistor = IO_UNUSED,
                     .Alternate = IO_UNUSED },
    [RCC_OSC_OUT] = { .Mode = IO_UNUSED,
                      .speed = IO_UNUSED,
                      .resistor = IO_UNUSED,
                      .Alternate = IO_UNUSED },
    // QSPI_FLASH
    [QSPI_CLK] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT9 },
    [QSPI_NCS] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_LOW,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT9 },
    [QSPI_IO3] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT9 },
    [QSPI_IO2] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT9 },
    [QSPI_IO0] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT10 },
    [QSPIO_IO1] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT10 },
    // CAN INVERTER
    [CAN_RX] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_V_HIGH,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT9 },
    [CAN_TX] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_V_HIGH,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT9 },
    // SDRAM ,
    [SDRAM_D0] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D1] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D2] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D3] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D4] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D5] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D6] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D7] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D8] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D9] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D10] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D11] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D12] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D13] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D14] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_D15] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },

    [SDRAM_A0] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A1] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A2] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A3] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A4] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A5] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A6] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A7] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A8] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A9] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_V_HIGH,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A10] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A11] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_A12] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    .resistor = IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },

    [SDRAM_NBL0] = { .Mode = IO_MODE_AF_PP,
                     .speed = IO_SPEED_V_HIGH,
                     .resistor = IO_RESISTOR_DISABLED,
                     .Alternate = IO_SELECT_ALT12 },
    [SDRAM_NBL1] = { .Mode = IO_MODE_AF_PP,
                     .speed = IO_SPEED_V_HIGH,
                     .resistor = IO_RESISTOR_DISABLED,
                     .Alternate = IO_SELECT_ALT12 },

    [SDRAM_BA0] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },
    [SDRAM_BA1] = { .Mode = IO_MODE_AF_PP,
                    .speed = IO_SPEED_V_HIGH,
                    IO_RESISTOR_DISABLED,
                    .Alternate = IO_SELECT_ALT12 },

    [SDRAM_SDCLK] = { .Mode = IO_MODE_AF_PP,
                      .speed = IO_SPEED_V_HIGH,
                      .resistor = IO_RESISTOR_DISABLED,
                      .Alternate = IO_SELECT_ALT12 },
    [SDRAM_SDNCAS] = { .Mode = IO_MODE_AF_PP,
                       .speed = IO_SPEED_V_HIGH,
                       .resistor = IO_RESISTOR_DISABLED,
                       .Alternate = IO_SELECT_ALT12 },
    [SDRAM_SDNRAS] = { .Mode = IO_MODE_AF_PP,
                       .speed = IO_SPEED_V_HIGH,
                       .resistor = IO_RESISTOR_DISABLED,
                       .Alternate = IO_SELECT_ALT12 },
    [SDRAM_SDNCKE0] = { .Mode = IO_MODE_AF_PP,
                        .speed = IO_SPEED_V_HIGH,
                        .resistor = IO_RESISTOR_DISABLED,
                        .Alternate = IO_SELECT_ALT12 },
    [SDRAM_SDNE0] = { .Mode = IO_MODE_AF_PP,
                      .speed = IO_SPEED_V_HIGH,
                      .resistor = IO_RESISTOR_DISABLED,
                      .Alternate = IO_SELECT_ALT12 },
    [SDRAM_SDNWE] = { .Mode = IO_MODE_AF_PP,
                      .speed = IO_SPEED_V_HIGH,
                      .resistor = IO_RESISTOR_DISABLED,
                      .Alternate = IO_SELECT_ALT12 },

    // UART PRINTF
    [TRACE_TX] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_LOW,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT7 },
    [TRACE_RX] = { .Mode = IO_MODE_AF_PP,
                   .speed = IO_SPEED_LOW,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_ALT7 },
    // ESP01
    [ESP_TX] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_LOW,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT7 },
    [ESP_RX] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_LOW,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT7 },
    // MAX485
    [MAX_TX] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_LOW,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT7 },
    [MAX_RX] = { .Mode = IO_MODE_AF_PP,
                 .speed = IO_SPEED_LOW,
                 .resistor = IO_RESISTOR_DISABLED,
                 .Alternate = IO_SELECT_ALT7 },
    // LED TEST
    [LED_TEST] = { .Mode = IO_MODE_OUTPUT_PP,
                   .speed = IO_SPEED_LOW,
                   .resistor = IO_RESISTOR_DISABLED,
                   .Alternate = IO_SELECT_GPIO },
    // AUX PINS
    [AUX_PIN_1] = { .Mode = IO_UNUSED,
                    .speed = IO_UNUSED,
                    .resistor = IO_UNUSED,
                    .Alternate = IO_UNUSED },
    [AUX_PIN_2] = { .Mode = IO_UNUSED,
                    .speed = IO_UNUSED,
                    .resistor = IO_UNUSED,
                    .Alternate = IO_UNUSED },
    [AUX_PIN_3] = { .Mode = IO_UNUSED,
                    .speed = IO_UNUSED,
                    .resistor = IO_UNUSED,
                    .Alternate = IO_UNUSED },

};

static_assert((sizeof(io_e) == 1), "Unexpected size of enum");

// return pointer to PORTS : GPIOA
static inline GPIO_TypeDef *io_port(io_e io)
{
    // if (io >= MAX_PIN) return NULL ;
    volatile uint8_t enc = io_pin_map[io];
    volatile uint8_t prt_indx = (((enc) >> (4u)) & (0xf));

    return (GPIO_TypeDef *)ports[prt_indx];
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
    pincnfg.Pin = pin;
    pincnfg.Pull = res_lr;
    pincnfg.Speed = speed_l;
    pincnfg.Alternate = Alt_l;

    HAL_GPIO_Init(port, &pincnfg);
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

void io_init(void)
{

    // TODO : intializing all pins group each Peripheral in a seperated BLocks
    for (io_e io = LTDC_VSYNC; io < MAX_PIN; ++io) {
        if (io == RCC_OSC32_IN || io == RCC_OSC32_OUT || io == RCC_OSC_IN || io == RCC_OSC_OUT)
            continue;
        for (int i = 0; i < 100; ++i)
            ; // delay
        io_configure(io, &(initial_configs[io]));
    }
}

// Get current configuration
void io_get_current_config(io_e io, struct io_config *current_config)
{
    GPIO_TypeDef *port = io_port(io);

    volatile uint8_t enc = io_pin_map[io];
    volatile uint8_t pin_idx = ((enc) & (0xf));

    // extract the config from PORT registers
    volatile uint32_t moder = ((port->MODER & (0x3 << (pin_idx * 2u))) >> (pin_idx * 2u));
    volatile uint32_t otype = ((port->OTYPER & (0x1 << pin_idx)) >> pin_idx);

    io_mode_e mode;
    switch (moder) {
    case 0x0:
        mode = IO_MODE_INPUT;
        break;
    case 0x1:
        mode = (otype == 0) ? IO_MODE_OUTPUT_PP : IO_MODE_OUTPUT_OD;
        break;
    case 0x2:
        mode = (otype == 0) ? IO_MODE_AF_PP : IO_MODE_AF_OD;
        break;
    case 0x3:
        mode = IO_MODE_ANALOG;
        break;
    default:
        mode = IO_MODE_INPUT;
        break; // Fallback
    }

    // extract configuration from the pin
    current_config->Mode = mode;
    current_config->resistor = ((port->PUPDR & (0x3 << (pin_idx * 2u))) >> (pin_idx * 2u));
    current_config->speed = ((port->OSPEEDR & (0x3 << (pin_idx * 2u))) >> (pin_idx * 2u));

    // Alternate function is valid only for AF mode
    if (moder == 0x2) {
        if (pin_idx < 8)
            current_config->Alternate =
                ((port->AFR[0] & (0xfu << (pin_idx * 4u))) >> (pin_idx * 4u));
        else
            current_config->Alternate =
                ((port->AFR[1] & (0xfu << ((pin_idx - 8u) * 4u))) >> ((pin_idx - 8u) * 4u));
    } else {
        current_config->Alternate = IO_SELECT_GPIO; // Not using AF
    }
}

bool io_config_compare(const struct io_config *cfg1, const struct io_config *cfg2)
{

    return (cfg1->Alternate == cfg2->Alternate) && (cfg1->Mode == cfg2->Mode)
        && (cfg1->resistor == cfg2->resistor) && (cfg1->speed == cfg2->speed);
}

// interrupt configuration
void io_config_interrupt(io_e io, const struct io_config *cfg, io_priority_e prio,
                         io_callback callbacks)
{

    io_configure(io, cfg); // configure pin

    volatile uint8_t enc = io_pin_map[io];
    volatile uint8_t pin_idx = ((enc) & (0xf)); // pin index from 0 - 15

    if (cfg->Mode == IO_MODE_IT_RISING || cfg->Mode == IO_MODE_IT_FALLING
        || cfg->Mode == IO_MODE_IT_RISING_FALLING) {

        IRQn_Type irqn = EXTI0_IRQn;
        if (pin_idx <= 4 && pin_idx >= 0) {
            irqn = (IRQn_Type)(EXTI0_IRQn + pin_idx);
        } else if (pin_idx <= 9 && pin_idx > 4) {
            irqn = EXTI9_5_IRQn;
        } else if (pin_idx <= 15 && pin_idx > 9) {
            irqn = EXTI15_10_IRQn;
        }
        HAL_NVIC_SetPriority((IRQn_Type)irqn, prio, 0);
        HAL_NVIC_EnableIRQ((IRQn_Type)irqn);

        io_callbacks[pin_idx] = callbacks;
    }
}
// clearing configured interrupt
void io_clear_interrupt(io_e io)
{

    volatile uint8_t enc = io_pin_map[io];
    volatile uint8_t pin_idx = ((enc) & (0xf));

    IRQn_Type irqn = EXTI0_IRQn;
    if (pin_idx <= 4 && pin_idx >= 0) {
        irqn = (IRQn_Type)(EXTI0_IRQn + pin_idx);
    } else if (pin_idx <= 9 && pin_idx > 4) {
        irqn = EXTI9_5_IRQn;
    } else if (pin_idx <= 15 && pin_idx > 9) {
        irqn = EXTI15_10_IRQn;
    }

    HAL_NVIC_ClearPendingIRQ((IRQn_Type)irqn);
    HAL_NVIC_DisableIRQ((IRQn_Type)irqn);
}

void io0_irqhandler()
{
    io_callbacks[0]();
}
void io1_irqhandler()
{
    io_callbacks[1]();
}
void io2_irqhandler()
{
    io_callbacks[2]();
}
void io3_irqhandler()
{
    io_callbacks[3]();
}
void io4_irqhandler()
{
    io_callbacks[4]();
}

void io5_9irqhandler(void)
{
    uint32_t pending = EXTI->PR1 & 0x000003E0; // bits 5-9 // read pending register
    EXTI->PR1 = pending; // clear all pending interrupts
    for (uint8_t pin = 5; pin <= 9; ++pin) {
        if (pending & (1U << pin)) {
            if (io_callbacks[pin])
                io_callbacks[pin]();
        }
    }
}

void io10_15irqhandler()
{
    uint32_t pending = EXTI->PR1 & 0x0000FC00; // bits 10-15
    EXTI->PR1 = pending; // clear all flags at once

    for (uint8_t pin = 10; pin <= 15; ++pin) {
        if (pending & (1U << pin)) {
            if (io_callbacks[pin]) {
                io_callbacks[pin]();
            }
        }
    }
}
