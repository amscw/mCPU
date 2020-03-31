/*
 * pins_usage.hpp
 *
 *  Created on: 26 мар. 2020 г.
 *      Author: moskw
 */

#ifndef INCLUDE_PINS_USAGE_HPP_
#define INCLUDE_PINS_USAGE_HPP_

#include "pins.hpp"

/**
 * ALl application pins MUST be defined here!
 */
#if defined(CPUk_APP)
/*
 * Keyboard scan-lines
 */
using PA0 = pin_c<port_c::port_t::A, 0>;	// S0
using PA1 = pin_c<port_c::port_t::A, 1>;	// S1
using PA2 = pin_c<port_c::port_t::A, 2>;	// S2
using PA3 = pin_c<port_c::port_t::A, 3>;	// S3
using PA4 = pin_c<port_c::port_t::A, 4>;	// S4
using PA5 = pin_c<port_c::port_t::A, 5>;	// S5
using PA6 = pin_c<port_c::port_t::A, 6>;	// S6
using PA7 = pin_c<port_c::port_t::A, 7>;	// S7
using PE0 = pin_c<port_c::port_t::E, 0>;	// S8
using PE1 = pin_c<port_c::port_t::E, 1>;	// S9
using PE2 = pin_c<port_c::port_t::E, 2>;	// S10
using PE3 = pin_c<port_c::port_t::E, 3>;	// S11
using PE6 = pin_c<port_c::port_t::E, 6>;	// S12
using PE7 = pin_c<port_c::port_t::E, 7>;	// S13
using PB8 = pin_c<port_c::port_t::B, 8>;	// S14

using PD0 = pin_c<port_c::port_t::D, 0>;	// D0
using PD1 = pin_c<port_c::port_t::D, 1>;	// D1
using PD2 = pin_c<port_c::port_t::D, 2>;	// D2
using PD3 = pin_c<port_c::port_t::D, 3>;	// D3
using PD4 = pin_c<port_c::port_t::D, 4>;	// D4
using PD5 = pin_c<port_c::port_t::D, 5>;	// D5
using PD6 = pin_c<port_c::port_t::D, 6>;	// D6
using PD7 = pin_c<port_c::port_t::D, 7>;	// D7

/*
 * LCD-brightness buttons pins
 */
using PB9 = pin_c<port_c::port_t::B, 9>;	// CPUk_LCD_BKLT+
using PB10 = pin_c<port_c::port_t::B, 10>;	// CPUk_LCD_BKLT-

/*
 * LCD-brightness PWM-control pin
 */
using PB7 = pin_c<port_c::port_t::B, 7>;

/*
 * LCD-OFF button pin
 */
using PC2 = pin_c<port_c::port_t::C, 2>;	// LCD_OFF

/*
 * LED-indicator pins
 */
using PF0 = pin_c<port_c::port_t::F, 0>;	// CH_SEL_A0
using PF1 = pin_c<port_c::port_t::F, 1>;	// CH_SEL_A1
using PF2 = pin_c<port_c::port_t::F, 2>;	// CH_SEL_A2
using PF3 = pin_c<port_c::port_t::F, 3>;	// CH_EN
#elif defined(CPUm_APP)

#endif

/*
 * I2C-lines
 */
using PC0 = pin_c<port_c::port_t::C, 0>;	// CPUx_SCL
using PC1 = pin_c<port_c::port_t::C, 1>;	// CPUx_SDA

/*
 * Debug UART pins
 */
using PB5 = pin_c<port_c::port_t::B, 5>;	// CPUx_Tx
using PB6 = pin_c<port_c::port_t::B, 6>;	// CPUx_Rx

#endif /* INCLUDE_PINS_USAGE_HPP_ */
