/*
 * pin_traits.hpp
 *
 *  Created on: 12 апр. 2020 г.
 *      Author: moskvin
 *      Descriptioт: Platform's specific constants
 */

#ifndef INCLUDE_PIN_TRAITS_HPP_
#define INCLUDE_PIN_TRAITS_HPP_


#include <cstdint>
extern "C" {
#include "stm32f7xx.h"	// need only for GPIOx_BASE macro
}

namespace GPIO {

enum letter_t : std::uint32_t {
		A = GPIOA_BASE,
		B = GPIOB_BASE,
		C = GPIOC_BASE,
		D = GPIOD_BASE,
		E = GPIOE_BASE,
		F = GPIOF_BASE,
		G = GPIOG_BASE,
		H = GPIOH_BASE,
		I = GPIOI_BASE,
		J = GPIOJ_BASE,
		K = GPIOK_BASE,
};

// port's register constants
enum class mode_t : std::uint8_t {INPUT, OUTPUT, ALTERNATE, ANALOG};
enum class speed_t : std::uint8_t {LOW, MEDIUM, HIGH, VERY_HIGH};
enum class pull_t : std::uint8_t {NO, UP, DOWN, RESERVED};
enum class af_t : std::uint8_t {
	AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15
};

} // GPIO



#endif /* INCLUDE_PIN_TRAITS_HPP_ */
