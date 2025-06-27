/*
 * trace.h
 *
 *  Created on: Jun 27, 2025
 *      Author: salman
 */

#ifndef TRACE_H
#define TRACE_H


#define TRACE(fmt, ...) trace("%s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#ifndef DISABLE_TRACE
// initialize the uart
void trace_init(void) ;
// this will wrap the printf call
void trace(const char *fmt, ...) ;


#else
#define trace_init(void) ;
#define trace(const char *fmt, ...) ;
#endif
#endif /* SRC_COMMON_TRACE_H_ */
