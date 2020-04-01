/*
 * io.hpp
 *
 *  Created on: 1 апр. 2020 г.
 *      Author: moskvin
 *      Description: io peripheral pin map
 *      All used peripherals MUST be defined here!
 *
 *      NB: all non-static classes MUST have the suffix '_c'
 */

#ifndef INCLUDE_IO_HPP_
#define INCLUDE_IO_HPP_

#include "pins_usage.hpp"		// need to be first included
#include "io_sn74lv244a.hpp"
#include "io_channel.hpp"

using SN74LV244A = sn74lv244a_c<mCPU_INBUF_nEN, mCPU_OUTBUF_nEN>;
using CHANNEL_c = channel_c<mCPU_CH_SEL_A0, mCPU_CH_SEL_A1, mCPU_CH_SEL_A2>;

#endif /* INCLUDE_IO_HPP_ */
