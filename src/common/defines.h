/*
 * defines.h
 *
 *  Created on: Jun 16, 2025
 *      Author: salman
 */

#ifndef DEFINES_H
#define DEFINES_H

#define UNUSED_(x) (void)(x)
#define ARRAYSIZE(x)  ((sizeof(x)) / sizeof(x[0]))
#define ENCODE_IO(port, pin) ((port << MAXPINBITS) | (pin & 0x0F)) // Encode pin and port in 8-bit variable

#endif /* SRC_COMMON_DEFINES_H_ */
