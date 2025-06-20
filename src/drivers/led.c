/*
 * led.c
 *
 *  Created on: Jun 19, 2025
 *      Author: salman
 */

#include "stm32h7xx_hal.h"
#include "common/assert_handler.h"
#include "common/defines.h"
#include "drivers/led.h"
#include "drivers/io.h"
#include <stdbool.h>


static volatile GPIO_TypeDef *const ports[MAXPORTS] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE,
                                                        GPIOF, GPIOG, GPIOH, GPIOI };

static const struct  io_config led_config =
{
		.Mode = IO_MODE_OUTPUT_PP ,
		.resistor = IO_RESISTOR_DISABLED ,
		.speed = IO_SPEED_LOW ,
		.Alternate = IO_SELECT_GPIO
};

static const uint8_t led_pin_map[MAX_LEDS] =
{
[LED_] = ENCODE_IO(PORTC,IO_13)
};

static bool initialized = false ;

void led_init(void)
{

	ASSERT(!initialized) ;
	struct io_config curren_config ;
	io_get_current_config(LED_TEST, &curren_config) ;
	//  this will check if the led is initialized already than it did
	// not assert otherwise assert
	ASSERT(io_config_compare(&curren_config, &led_config)) ;
	initialized = true ;
}


void led_set(led_e led , led_state_e state )
{
    volatile uint8_t enc = led_pin_map[led];
    volatile uint8_t _indx = (((enc) >> (4u)) & (0xf));
	GPIO_TypeDef *port =  ports[_indx];

	enc = led_pin_map[led] ;

	_indx = ((enc) & (0xf));

	uint16_t pin = (1u << _indx);

	HAL_GPIO_WritePin(port, pin, state);

}
