/*
 * main.cpp
 *
 *  Created on: 17 мар. 2020 г.
 *      Author: moskvin
 */

#include "trace.h"
#include "io.hpp"

int main(int argc, char *argv[])
{
	CHANNEL_c channel;
	SN74LV244A::Init();
	SN74LV244A::Driver1Enable();

	channel.Select(ch_t::CH5);

	trace_printf("hello, fucking world\n");
	while(1);
}
