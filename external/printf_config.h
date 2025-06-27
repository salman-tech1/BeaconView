/*
 * printf_config.h
 *
 *  Created on: Jun 27, 2025
 *      Author: salman
 */

#ifndef PRINTF_CONFIG
#define PRINTF_CONFIG

/* Configuration file for the mpaland printf implementation (see external/printf).
 * PRINTF_INCLUDE_CONFIG_H must be defined as a compiler switch for this to be picked up */

// Disable everything not used to reduce flash usage
#define PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#define PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_DISABLE_SUPPORT_FLOAT


#endif /* EXTERNAL_PRINTF_CONFIG_H_ */
