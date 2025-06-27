/*
 * assert_handler.h
 *
 *  Created on: Jun 19, 2025
 *      Author: salman
 */

#ifndef SRC_COMMON_ASSERT_HANDLER_H_
#define SRC_COMMON_ASSERT_HANDLER_H_



#define ASSERT(expression) \
do { \
	if(!(expression)) \
		{				\
		assert_handler(__FILE__,__LINE__) ; \
		} \
}while(0) ;

void assert_handler( const char *file , int line ) ;

#endif /* SRC_COMMON_ASSERT_HANDLER_H_ */
