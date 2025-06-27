/*
 * ring_buffer.h
 *
 *  Created on: Jun 23, 2025
 *      Author: salman
 */

#ifndef RING_BUFFER
#define RING_BUFFER

#include <stdint.h>
#include <stdbool.h>


typedef enum
{
    R_EMPTY ,
    R_NOT_EMPTY ,
    R_FULL ,
    R_NOT_FULL,

}rng_stat_e ;

struct ring_buffer
{
    uint8_t *buff ;
    uint16_t size ;
    uint16_t head ;
    uint16_t tail ;
} ;


void ring_buffer_put(struct ring_buffer *rb , uint8_t data) ; // putting data into buffer and moving head
uint8_t ring_buffer_get(struct ring_buffer *rb ) ; // get byte from ringbuffer
uint8_t ring_buffer_peek(const struct ring_buffer *rb); // get the peek element but don't remove it

rng_stat_e ring_buffer_isempty(struct ring_buffer *rb ) ;  // check if ring buffer is empty
rng_stat_e ring_buffer_isfull(struct ring_buffer *rb ) ; // check if it's full


#endif /* RING_BUFFER */
