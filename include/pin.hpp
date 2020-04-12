/*
 * pin.hpp
 *
 *  Created on: 21 мар. 2020 г.
 *      Author: moskvin
 *      Description: cpu's pin classes
 */

#ifndef INCLUDE_PIN_HPP_
#define INCLUDE_PIN_HPP_

#include <pin_traits.hpp>
#if defined(DEBUG_GPIO)
#include "common.hpp"
#endif
/**
 * Port
 */
template<GPIO::letter_t letter> struct Port
{
	enum {
		INDEX = (static_cast<std::uint32_t>(letter) >> 10) & 0xF,
		INDEX_MASK = 1 << INDEX,
	};

	// Enable GPIOx port clock
	static inline void ClockEnable() noexcept __attribute__((always_inline))
	{
		RCC->AHB1ENR |= INDEX_MASK;
	}

	// Disable GPIOx port clock
	static inline void ClockDisable() noexcept __attribute__((always_inline))
	{
		RCC->AHB1ENR &= ~INDEX_MASK;
	}

	static inline constexpr GPIO_TypeDef volatile* GPIOx()
	{
		return reinterpret_cast<GPIO_TypeDef*>(letter);
	};

#if defined(DEBUG_GPIO)
	static inline constexpr char LetterChar()
	{
		return 'A' + INDEX;
	}

	static /* inline */ void DumpPortReg(const volatile std::uint32_t &reg)  noexcept /* __attribute__((always_inline)) */
	{
		extern const char *portRegNames[];
		PRINT("GPIO%c_%s = 0x%x", LetterChar(), portRegNames[(reinterpret_cast<std::uint32_t>(&reg) >> 2) & 0xF], reg);
	}
#endif
};

/**
 * Pin
 * letter - letter of port: A, B, C, ... etc
 * n - pin number
 */
template<GPIO::letter_t letter, int n> class Pin
{
	enum {
		_1BIT_FIELD_MASK = (1 << n),
		_2BIT_FIELD_MASK_INV = ~(0x3 << (n << 1)),
		/*_4BIT_FIELD_MASK_INV = ~(0xF << (n << 2)),*/
	};

public:
	typedef Port<letter> PortType;
	enum {NUMBER = n};


	// Configure pin as push-pull output
	static void ConfigAsOutput(GPIO::speed_t s = GPIO::speed_t::HIGH) noexcept
	{
		std::uint32_t tmp;

		// 1. set output type - push/pull
		PortType::GPIOx()->OTYPER &= ~_1BIT_FIELD_MASK;

		// 2. set speed
		tmp = PortType::GPIOx()->OSPEEDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(s) << (n << 1));
		PortType::GPIOx()->OSPEEDR = tmp;


		// 3. disable pull-up/pull-down
		PortType::GPIOx()->PUPDR &= _2BIT_FIELD_MASK_INV;

		// 0. set MODER
		tmp = PortType::GPIOx()->MODER;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(GPIO::mode_t::OUTPUT) << (n << 1));
		PortType::GPIOx()->MODER = tmp;
	}

	// Configure pin as open-drain output
	static void ConfigAsOutputOD(GPIO::pull_t pupd, GPIO::speed_t s = GPIO::speed_t::HIGH) noexcept
	{
		std::uint32_t tmp;

		// 1. set output type - open drain
		PortType::GPIOx()->OTYPER |= _1BIT_FIELD_MASK;

		// 2. set speed
		tmp = PortType::GPIOx()->OSPEEDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(s) << (n << 1));
		PortType::GPIOx()->OSPEEDR = tmp;


		// 3. set pull-up/pull-down
		tmp = PortType::GPIOx()->PUPDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(pupd) << (n << 1));
		PortType::GPIOx()->PUPDR = tmp;

		// 0. set MODER
		tmp = PortType::GPIOx()->MODER;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(GPIO::mode_t::OUTPUT) << (n << 1));
		PortType::GPIOx()->MODER = tmp;
	}

	// Configure pin as digital input
	static void ConfigAsInput(GPIO::pull_t pupd = GPIO::pull_t::NO) noexcept
	{
		std::uint32_t tmp;

		// 3. set pull-up/pull-down
		tmp = PortType::GPIOx()->PUPDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(pupd) << (n << 1));
		PortType::GPIOx()->PUPDR = tmp;

		// 0. set MODER
		tmp = PortType::GPIOx()->MODER;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(GPIO::mode_t::INPUT) << (n << 1));
		PortType::GPIOx()->MODER = tmp;
	}

	static void ConfigAsAlternate(GPIO::af_t af, bool bIsOutputOD, GPIO::speed_t s, GPIO::pull_t pupd) noexcept
	{
		std::uint32_t tmp;

		// 1. set output type - open drain
		if (bIsOutputOD) {
			PortType::GPIOx()->OTYPER |= _1BIT_FIELD_MASK;
		} else {
			PortType::GPIOx()->OTYPER &= ~_1BIT_FIELD_MASK;
		}

		// 2. set speed
		tmp = PortType::GPIOx()->OSPEEDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(s) << (n << 1));
		PortType::GPIOx()->OSPEEDR = tmp;


		// 3. set pull-up/pull-down
		tmp = PortType::GPIOx()->PUPDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(pupd) << (n << 1));
		PortType::GPIOx()->PUPDR = tmp;

		// 4. set alternate function number
		// FIXME: не компилится с 4 битовыми полями
		tmp = PortType::GPIOx()->AFR[n < 8 ? 0 : 1];
		//tmp &= _4BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(af) << (n << 2));
		PortType::GPIOx()->AFR[n < 8 ? 0 : 1] = tmp;

		// 0. set MODER
		tmp = PortType::GPIOx()->MODER;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= (static_cast<std::uint32_t>(GPIO::mode_t::ALTERNATE) << (n << 1));
		PortType::GPIOx()->MODER = tmp;
	}

	static inline void Set() noexcept __attribute__((always_inline))
	{
		PortType::GPIOx()->BSRR |= (1 << n);
	}

	static inline void Reset() noexcept __attribute__((always_inline))
	{
		PortType::GPIOx()->BSRR |= ((1 << n) << 16);
	}

	static inline bool Get() noexcept __attribute__((always_inline))
	{
		return ((PortType::GPIOx()->IDR & (1 << n)) > 0);
	}
};

#endif /* INCLUDE_PIN_HPP_ */
