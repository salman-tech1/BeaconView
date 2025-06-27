/*
 * ring_buffer.c
 *
 *  Created on: Jun 23, 2025
 *      Author: salman
 */

#include "ring_buffer.h"
#include <stdlib.h>
#include <stdbool.h>

void ring_buffer_put(struct ring_buffer *rb, uint8_t data)
{

    rb->buff[(rb->head)] = data;
    rb->head += 1u;

    if (rb->head >= rb->size)
        rb->head = 0;
}

uint8_t ring_buffer_get(struct ring_buffer *rb)
{
    const uint16_t indx = rb->tail;
    const uint8_t data = rb->buff[indx];
    rb->tail += 1u;

    if (rb->tail >= rb->size) // if the size matched than wrap around
    {
        rb->tail = 0;
    }

    return data;
}

uint8_t ring_buffer_peek(const struct ring_buffer *rb)
{
    const uint16_t indx = rb->tail;
    const uint8_t data = rb->buff[indx];

    return data;
}

// TODO : return true if head is equal to tail
rng_stat_e ring_buffer_isempty(struct ring_buffer *rb)
{

    return (rb->head == rb->tail) ? R_EMPTY : R_NOT_EMPTY;
}

rng_stat_e ring_buffer_isfull(struct ring_buffer *rb)
{
    uint16_t next_head = (rb->head + 1);

    if (next_head == rb->size) {
        next_head = 0;
    }
    // return true if full
    return (next_head == rb->tail) ? R_FULL : R_NOT_FULL;
}
