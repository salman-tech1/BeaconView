/*
 * uart.h
 *
 *  Created on: Jun 23, 2025
 *      Author: salman
 */

#ifndef DRIVERS_UART
#define DRIVERS_UART

// TODO : enum for UART instance
typedef enum {
    TRACE = 0,
    MAX485,
    ESP01,
    MAX_UARTS
} uart_inst_e;

typedef enum {
    BAUD_115200 = 115200,
    BAUD_9600 = 9600

} baud_e;

typedef enum {

    INT_DISABLED ,
	INT_ENABLED
} isr_e;

typedef enum {
    PRIO_0,
    PRIO_1,
    PRIO_2,
    PRIO_3,
    PRIO_4,
    PRIO_5,
    PRIO_6,
    PRIO_7,
    PRIO_8,
    PRIO_9,
    PRIO_10,
    PRIO_11,
    PRIO_12,
    PRIO_13,
    PRIO_14,
    PRIO_15,

} uart_priority_e;



typedef enum
{
  TXE_INT = 1   ,
  TC_INT ,
  RXNE_INT ,

} int_type_e;




struct uart_config
{
    isr_e isr_stat ;
    int_type_e int_type ;
    baud_e baud;
    uart_priority_e prio ;
};

// initialize all the UARTS
void uart_init(void);
// init specific UART
void uart_configure( uart_inst_e uart_inst , const struct uart_config *config); // selectively configure UART
void uart_putchar(uart_inst_e inst , char c) ;
void uart_putchar_interrupt(uart_inst_e inst , char c ) ;
char uart_getchar(uart_inst_e inst ) ;
void uart_getchar_interrupt(uart_inst_e inst ) ;
int uart_irq_isreadable() ;
void _putchar(char c ) ;

void uart_assert_init() ;

#endif
