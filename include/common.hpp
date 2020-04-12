/*
 * common.hpp
 *
 *  Created on: 1 апр. 2020 г.
 *      Author: moskvin
 *      Description: common useful functions
 */

#ifndef INCLUDE_COMMON_HPP_
#define INCLUDE_COMMON_HPP_

extern "C" {
#include "trace.h"
}

extern "C" {
#define PRINT(fmt,args...) trace_printf("%s(%s)-" fmt "\n", __FILE__, __FUNCTION__, ##args)
}

#endif /* INCLUDE_COMMON_HPP_ */
