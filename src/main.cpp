/*
 * main.cpp
 *
 *  Created on: 17 мар. 2020 г.
 *      Author: moskvin
 */


#include "trace.h"
#include "pin_usage.hpp"
#include "io_sn74lv244a.hpp"

typedef utils::TypeList<PB8, PB13, PB14, PB15> PinList;
typedef std::make_index_sequence<static_cast<std::size_t>(utils::Length<PinList>::value)> Is;
typedef typename MakePinWrappers<PinList, Is>::type PinWrappers;

int main(int argc, char *argv[])
{
	trace_printf("hello, fucking world\n");
	Configure<PinWrappers>::AsOutput();

	while(1);
}
