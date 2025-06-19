/*
 * io.h
 *
 *  Created on: Jun 16, 2025
 *      Author: salman
 */

#ifndef IO_H
#define IO_H

#include "stdbool.h"
#include "stdint.h"

/* IO pins handling including pinmapping, initialization, and configuration.
 * This wraps the more crude register defines provided in the headers from
 * ST MicroElectronics */

#define MAXPORTBITS 4 // 2^4 = 16 // 9 Ports
#define MAXPINBITS 4 // 2^4 = 16 //15 pins
#define IO_UNUSED 0

// TODO : Improve multiple HW targets handling
#define STM32H7BOARD

// TODO : enums : define all pins
typedef enum {
#if defined(STM32H7BOARD)
    // LCD
    LTDC_VSYNC,
    LTDC_HSYNC,
    LTDC_DE,
    LTDC_G4,
    LTDC_R3,
    LTDC_R4,
    LTDC_R5,
    LTDC_R6,
    LTDC_R7,
    LTDC_CLK,
    LTDC_B3,
    LTDC_G2,
    LTDC_G5,
    LTDC_G6,
    LTDC_G7,
    LTDC_G3,
    LTDC_B4,
    LTDC_B5,
    LTDC_B6,
    LTDC_B7,
    LCD_RST,
    LCD_BL,
    OUT_PE3,
    // TOUCH_SENS
    TOUCH_SDA,
    TOUCH_SCL,
    // TEMP_SENS
    TEMP_SDA,
    TEMP_SCL,
    GT_RESET,
    // SDCARD
    SD_D0,
    SD_CLK,
    SD_CMD,
    // RCC_OSC
    RCC_OSC32_IN,
    RCC_OSC32_OUT,
    RCC_OSC_IN,
    RCC_OSC_OUT,
    // QSPI_FLASH
    QSPI_CLK,
    QSPI_NCS,
    QSPI_IO3,
    QSPI_IO2,
    QSPI_IO0,
    QSPIO_IO1,
    // CAN INVERTER
    CAN_RX,
    CAN_TX,
    // SDRAM ,
    SDRAM_D0,
    SDRAM_D1,
    SDRAM_D2,
    SDRAM_D3,
    SDRAM_D4,
    SDRAM_D5,
    SDRAM_D6,
    SDRAM_D7,
    SDRAM_D8,
    SDRAM_D9,
    SDRAM_D10,
    SDRAM_D11,
    SDRAM_D12,
    SDRAM_D13,
    SDRAM_D14,
    SDRAM_D15,

    SDRAM_A0,
    SDRAM_A1,
    SDRAM_A2,
    SDRAM_A3,
    SDRAM_A4,
    SDRAM_A5,
    SDRAM_A6,
    SDRAM_A7,
    SDRAM_A8,
    SDRAM_A9,
    SDRAM_A10,
    SDRAM_A11,
    SDRAM_A12,

    SDRAM_NBL0,
    SDRAM_NBL1,

    SDRAM_BA0,
    SDRAM_BA1,

    SDRAM_SDCLK,
    SDRAM_SDNCAS,
    SDRAM_SDNRAS,
    SDRAM_SDNCKE0,
    SDRAM_SDNE0,
    SDRAM_SDNWE,

    // UART PRINTF
    TRACE_TX,
    TRACE_RX,
    // ESP01
    ESP_TX,
    ESP_RX,
    // MAX485
    MAX_TX,
    MAX_RX,
    // LED TEST
    LED_TEST,
    // AUX PINS
    AUX_PIN_1,
    AUX_PIN_2,
    AUX_PIN_3,
    // END of PINS
    MAX_PIN,

#elif defined(BEACONVIEW)
    // LCD
    LTDC_VSYNC,
    LTDC_HSYNC,
    LTDC_DE,
    LTDC_G4,
    LTDC_R3,
    LTDC_R4,
    LTDC_R5,
    LTDC_R6,
    LTDC_R7,
    LTDC_CLK,
    LTDC_B3,
    LTDC_G2,
    LTDC_G5,
    LTDC_G6,
    LTDC_G7,
    LTDC_G3,
    LTDC_B4,
    LTDC_B5,
    LTDC_B6,
    LTDC_B7,
    LCD_RST,
    LCD_BL,
    OUT_PE3,
    // TOUCH_SENS
    TOUCH_SDA,
    TOUCH_SCL,
    // TEMP_SENS
    TEMP_SDA,
    TEMP_SCL,
    // SDCARD
    SD_D0,
    SD_CLK,
    SD_CMD,
    // RCC_OSC
    RCC_OSC32_IN,
    RCC_OSC32_OUT,
    RCC_OSC_IN,
    RCC_OSC_OUT,
    // QSPI_FLASH
    QSPI_CLK,
    QSPI_NCS,
    QSPI_IO3,
    QSPI_IO2,
    QSPI_IO0,
    QSPIO_IO1,
    // CAN INVERTER
    CAN_RX,
    CAN_TX,
    // SDRAM ,
    SDRAM_D0,
    SDRAM_D1,
    SDRAM_D2,
    SDRAM_D3,
    SDRAM_D4,
    SDRAM_D5,
    SDRAM_D6,
    SDRAM_D7,
    SDRAM_D8,
    SDRAM_D9,
    SDRAM_D10,
    SDRAM_D11,
    SDRAM_D12,
    SDRAM_D13,
    SDRAM_D14,
    SDRAM_D15,

    SDRAM_A0,
    SDRAM_A1,
    SDRAM_A2,
    SDRAM_A3,
    SDRAM_A4,
    SDRAM_A5,
    SDRAM_A6,
    SDRAM_A7,
    SDRAM_A8,
    SDRAM_A9,
    SDRAM_A10,
    SDRAM_A11,
    SDRAM_A12,

    SDRAM_NBL0,
    SDRAM_NBL1,

    SDRAM_BA0,
    SDRAM_BA1,

    SDRAM_SDCLK,
    SDRAM_SDNCAS,
    SDRAM_SDNRAS,
    SDRAM_SDNCKE0,
    SDRAM_SDNE0,
    SDRAM_SDNWE,

    // UART PRINTF
    TRACE_TX,
    TRACE_RX,
    // ESP01
    ESP_TX,
    ESP_RX,
    // MAX485
    MAX_TX,
    MAX_RX,
    // LED TEST
    LED_TEST,
    // AUX PINS
    AUX_PIN_1,
    AUX_PIN_2,
    AUX_PIN_3,
#endif

} io_e;

// TODO : IO_PINS
typedef enum {
    IO_0,
    IO_1,
    IO_2,
    IO_3,
    IO_4,
    IO_5,
    IO_6,
    IO_7,
    IO_8,
    IO_9,
    IO_10,
    IO_11,
    IO_12,
    IO_13,
    IO_14,
    IO_15,

} io_pin_e;

// TODO : IO_PORTS
typedef enum {
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF,
    PORTG,
    PORTH,
    PORTI,
    MAXPORTS,
} io_port_e;

typedef enum {
    IO_SELECT_GPIO,
    IO_SELECT_ALT1,
    IO_SELECT_ALT2,
    IO_SELECT_ALT3,
    IO_SELECT_ALT4,
    IO_SELECT_ALT5,
    IO_SELECT_ALT6,
    IO_SELECT_ALT7,
    IO_SELECT_ALT8,
    IO_SELECT_ALT9,
    IO_SELECT_ALT10,
    IO_SELECT_ALT11,
    IO_SELECT_ALT12,
    IO_SELECT_ALT13,
    IO_SELECT_ALT14,
    IO_SELECT_ALT15,

} io_select_e;

typedef enum {
    IO_MODE_INPUT = 0,
    IO_MODE_OUTPUT_PP,
    IO_MODE_OUTPUT_OD,
    IO_MODE_AF_PP,
    IO_MODE_AF_OD,
    IO_MODE_ANALOG,
    IO_MODE_IT_RISING,
    IO_MODE_IT_FALLING,
    IO_MODE_IT_RISING_FALLING,
    IO_MODE_EVT_RISING,
    IO_MODE_EVT_FALLING,
    IO_MODE_EVT_RISING_FALLING,
} io_mode_e;

typedef enum {
    IO_DIR_INPUT,
    IO_DIR_OUTPUT,
} io_dir_e;

typedef enum {
    IO_RESISTOR_DISABLED,
    IO_RESISTOR_PULLUP,
    IO_RESISTOR_PULLDOWN,
} io_resistor_e;

typedef enum {
    IO_SPEED_LOW,
    IO_SPEED_MEDIUM,
    IO_SPEED_HIGH,
    IO_SPEED_V_HIGH,
} io_speed_e;

typedef enum {
    IO_OUT_LOW, // (pull-down)
    IO_OUT_HIGH, // (pull-up)
} io_out_e;

typedef enum {
    IO_IN_LOW,
    IO_IN_HIGH,
} io_in_e;

typedef enum {
    IO_TRIGGER_RISING,
    IO_TRIGGER_FALLING
} io_trigger_e;

// TODO : struct

struct io_config
{
    io_mode_e Mode;
    io_speed_e speed;
    io_select_e Alternate;
    io_resistor_e resistor;
};

// TODO : functions
void io_configure(io_e io, const struct io_config *config);
void io_set_out(io_e io, io_out_e out);
io_in_e io_get_input(io_e io);
void io_init(void) ;

#endif /* SRC_DRIVERS_IO_H_ */
