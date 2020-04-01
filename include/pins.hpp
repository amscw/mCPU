/*
 * pin.hpp
 *
 *  Created on: 21 мар. 2020 г.
 *      Author: moskvin
 *      Description: generic pin/port-control functions
 *      !!! Do NOT include this file directly !!!
 *      Use pins_usage.hpp instead for application specific definitions
 */

#ifndef INCLUDE_PINS_HPP_
#define INCLUDE_PINS_HPP_

#include <cstdint>
extern "C" {
#include "stm32f7xx.h"	// need only for GPIOx_BASE macro
}

/*
 * Generic GPIO definitions, constants and methods
 */
class port_c
{
public:
	using port_t = enum {
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

		// GPIOA = 0, GPIOB = 1, GPIOC = 2, etc...
	static constexpr std::uint32_t PORT_INDEX(port_t port) noexcept
	{
		return (static_cast<std::uint32_t>(port) >> 10) & 0xF;
	}

	// GPIOA = 0x01, GPIOB = 0x02, GPIOC = 0x04, etc...
	static constexpr std::uint32_t PORT_BIT(port_t port) noexcept
	{
		return 1 << PORT_INDEX(port);
	}

	// Enable GPIOx port clock
	static inline void ClockEnable(port_t port) noexcept __attribute__((always_inline))
	{
		RCC->AHB1ENR |= PORT_BIT(port);
	}

	// Disable GPIOx port clock
	static inline void ClockDisable(port_t port) noexcept __attribute__((always_inline))
	{
		RCC->AHB1ENR &= ~PORT_BIT(port);
	}

protected:
	static std::uint32_t pinsInUse[];

	// Get pin usage mask
	static inline std::uint32_t &getUsageMask(port_t port) noexcept __attribute__((always_inline))
	{
		return pinsInUse[PORT_INDEX(port)];
	}

	// merge pins: pins = mergePins(pins, 1, 2, 3);
	template<std::uint32_t lastPin>
	constexpr static std::uint32_t mergePins(std::uint32_t mask) noexcept
	{
		return mask |= 1 << lastPin;
	}

	template<std::uint32_t firstPin, std::uint32_t nextPin, std::uint32_t... restPins>
	constexpr static std::uint32_t mergePins(std::uint32_t mask) noexcept
	{
		mask |= 1 << firstPin;
		return mergePins<nextPin, restPins...>(mask);
	}
};

/**
 * One pin only manage functions
 */
template<port_c::port_t port, std::uint32_t pin> class pin_c : port_c
{
public:

	// Enable GPIOx port clock (override)
	static inline void ClockEnable() noexcept __attribute__((always_inline))
	{
		port_c::ClockEnable(port);
	}

	// Disable GPIOx port clock (override)
	static inline void ClockDisable() noexcept __attribute__((always_inline))
	{
		port_c::ClockDisable(port);
	}

	// Configure pin as push-pull output
	static void ConfigAsOutput(speed_t s = speed_t::HIGH) noexcept
	{
		const std::uint32_t _1bitFieldMask = (1 << pin);
		const std::uint32_t _2bitFieldMask = ~(0x3 << (pin << 1));
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		std::uint32_t tmp;

		// 1. set output type - push/pull
		GPIOx->OTYPER &= ~_1bitFieldMask;

		// 2. set speed
		tmp = GPIOx->OSPEEDR;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(s) << (pin << 1));
		GPIOx->OSPEEDR = tmp;


		// 3. disable pull-up/pull-down
		GPIOx->PUPDR &= _2bitFieldMask;

		// 0. set MODER
		tmp = GPIOx->MODER;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(mode_t::OUTPUT) << (pin << 1));
		GPIOx->MODER = tmp;
	}

	// Configure pin as open-drain output
	static void ConfigAsOutputOD(pull_t pupd, speed_t s = speed_t::HIGH) noexcept
	{
		const std::uint32_t _1bitFieldMask = (1 << pin);
		const std::uint32_t _2bitFieldMask = ~(0x3 << (pin << 1));
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		std::uint32_t tmp;

		// 1. set output type - open drain
		GPIOx->OTYPER |= _1bitFieldMask;

		// 2. set speed
		tmp = GPIOx->OSPEEDR;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(s) << (pin << 1));
		GPIOx->OSPEEDR = tmp;


		// 3. set pull-up/pull-down
		tmp = GPIOx->PUPDR;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(pupd) << (pin << 1));
		GPIOx->PUPDR = tmp;

		// 0. set MODER
		tmp = GPIOx->MODER;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(mode_t::OUTPUT) << (pin << 1));
		GPIOx->MODER = tmp;
	}

	// Configure pin as digital input
	static void ConfigAsInput(pull_t pupd = pull_t::NO) noexcept
	{
		const std::uint32_t _1bitFieldMask = (1 << pin);
		const std::uint32_t _2bitFieldMask = ~(0x3 << (pin << 1));
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		std::uint32_t tmp;

		// 3. set pull-up/pull-down
		tmp = GPIOx->PUPDR;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(pupd) << (pin << 1));
		GPIOx->PUPDR = tmp;

		// 0. set MODER
		tmp = GPIOx->MODER;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(mode_t::INPUT) << (pin << 1));
		GPIOx->MODER = tmp;
	}

	static void ConfigAsAlternate(af_t af, bool bIsOutputOD, speed_t s, pull_t pupd) noexcept
	{
		const std::uint32_t _1bitFieldMask = (1 << pin);
		const std::uint32_t _2bitFieldMask = ~(0x3 << (pin << 1));
		const std::uint32_t _4bitFieldMask = ~(0xF << (pin << 2));
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		std::uint32_t tmp;

		// 1. set output type - open drain
		if (bIsOutputOD) {
			GPIOx->OTYPER |= _1bitFieldMask;
		} else {
			GPIOx->OTYPER &= ~_1bitFieldMask;
		}

		// 2. set speed
		tmp = GPIOx->OSPEEDR;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(s) << (pin << 1));
		GPIOx->OSPEEDR = tmp;


		// 3. set pull-up/pull-down
		tmp = GPIOx->PUPDR;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(pupd) << (pin << 1));
		GPIOx->PUPDR = tmp;

		// 4. set alternate function number
		tmp = GPIOx->AFR[pin_c::PIN_NUMBER() < 8 ? 0 : 1];
		tmp &= _4bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(af) << (pin << 2));
		GPIOx->AFR[pin_c::PIN_NUMBER() < 8 ? 0 : 1] = tmp;

		// 0. set MODER
		tmp = GPIOx->MODER;
		tmp &= _2bitFieldMask;
		tmp |= (static_cast<std::uint32_t>(mode_t::ALTERNATE) << (pin << 1));
		GPIOx->MODER = tmp;
	}

	static inline void Set() noexcept __attribute__((always_inline))
	{
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		GPIOx->BSRR |= (1 << pin);
	}

	static inline void Reset() noexcept __attribute__((always_inline))
	{
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		GPIOx->BSRR |= ((1 << pin) << 16);
	}

	static inline bool Get() noexcept __attribute__((always_inline))
	{
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		return ((GPIOx->IDR & (1 << pin)) > 0);
	}

	constexpr static std::uint32_t PIN_NUMBER() noexcept
	{
		return pin;
	}

	constexpr static GPIO_TypeDef* PIN_PORT_STRUCT() noexcept
	{
		return reinterpret_cast<GPIO_TypeDef*>(port);
	}

	constexpr static port_c::port_t PIN_PORT() noexcept
	{
		return port;
	}
};

/**
 * Multiple pins manage functions
 */
class pins_c : port_c
{
public:

	/*
	 * Configure multiple-pin as push-pull outputs
	 */
	template <class lastPin_c>
	static void ConfigAsOutput(speed_t s = speed_t::HIGH) noexcept
	{
		// last recursive call
		lastPin_c::ConfigAsOutput(s);
	}

	template <class firstPin_c, class nextPin_c, class... restPins_c>
	static void ConfigAsOutput(speed_t s = speed_t::HIGH) noexcept
	{
		firstPin_c::ConfigAsOutput(s);
		pins_c::ConfigAsOutput<nextPin_c, restPins_c...>(s);
	}

	// Configure multiple-pin as open-drain outputs
	template <class lastPin_c>
	static void ConfigAsOutputOD(pull_t pupd, speed_t s = speed_t::HIGH) noexcept
	{
		// last recursive call
		lastPin_c::ConfigAsOutputOD(pupd, s);
	}

	template <class firstPin_c, class nextPin_c, class... restPins_c>
	static void ConfigAsOutputOD(pull_t pupd, speed_t s = speed_t::HIGH) noexcept
	{
		firstPin_c::ConfigAsOutputOD(pupd, s);
		pins_c::ConfigAsOutputOD<nextPin_c, restPins_c...>(pupd, s);
	}

	// Configure multiple-pin as inputs
	template <class lastPin_c>
	static void ConfigAsInput(pull_t pupd = pull_t::NO) noexcept
	{
		// last recursive call
		lastPin_c::ConfigAsInput(pupd);
	}

	template <class firstPin_c, class nextPin_c, class... restPins_c>
	static void ConfigAsInput(pull_t pupd = pull_t::NO) noexcept
	{
		firstPin_c::ConfigAsOutput(pupd);
		pins_c::ConfigAsInput<nextPin_c, restPins_c...>(pupd);
	}

	// Configure multiple-pins to alternate function
	template <class lastPin_c>
	static void ConfigAsAlternate(af_t af, bool bIsOutputOD, speed_t s, pull_t pupd) noexcept
	{
		// last recursive call
		lastPin_c::ConfigAsAlternate(af, bIsOutputOD, s, pupd);
	}

	template <class firstPin_c, class nextPin_c, class... restPins_c>
	static void ConfigAsAlternate(af_t af, bool bIsOutputOD, speed_t s, pull_t pupd) noexcept
	{
		firstPin_c::ConfigAsAlternate(af, bIsOutputOD, s, pupd);
		pins_c::ConfigAsAlternate<nextPin_c, restPins_c...>(af, bIsOutputOD, s, pupd);
	}

	// Set multiple-pin to "1"
	template <class firstPin_c, class... restPins_c>
	static inline void Set() noexcept
	{
		GPIO_TypeDef *GPIOx = firstPin_c::PIN_PORT_STRUCT();
		std::uint32_t mask = 0;
		mask = mergePins<firstPin_c::PIN_NUMBER(), (restPins_c::PIN_NUMBER())...>(mask);
		GPIOx->BSRR |= (mask & 0xFFFF);
	}

	template<port_t port>
	static inline void SetValue(std::uint32_t mask, std::uint32_t value) noexcept
	{
		GPIO_TypeDef volatile* GPIOx = reinterpret_cast<GPIO_TypeDef*>(port);
		std::uint32_t tmp = GPIOx->ODR;
		tmp &= ~mask;
		tmp |= (value & mask);
		GPIOx->ODR = tmp;
	}

	// Reset multiple-pin to "0"
	template <class firstPin_c, class... restPins_c>
	static inline void Reset() noexcept
	{
		GPIO_TypeDef* GPIOx = firstPin_c::PIN_PORT_STRUCT();
		std::uint32_t mask = 0;
		mask = mergePins<firstPin_c::PIN_NUMBER(), (restPins_c::PIN_NUMBER())...>(mask);
		GPIOx->BSRR |= (mask << 16);
	}

	// Mark multiple-pin for use
	template<class firstPin_c, class... restPins_c>
	static std::uint32_t Use()
	{
		std::uint32_t mask = 0;
		const port_t port = static_cast<port_t>(reinterpret_cast<std::uint32_t>(firstPin_c::PIN_PORT_STRUCT()));

		// mark pins for usage
		mask = mergePins<firstPin_c::PIN_NUMBER(), (restPins_c::PIN_NUMBER())...>(mask);
		port_c::getUsageMask(port) |= mask;

		// enable clocks
		ClockEnable(port);
		return mask;
	}

	// Mark-off multiple-pin out of use
	template<class firstPin_c, class... restPins_c>
	static std::uint32_t Unuse()
	{
		std::uint32_t mask = 0;
		const port_t port = static_cast<port_t>(reinterpret_cast<std::uint32_t>(firstPin_c::PIN_PORT_STRUCT()));

		// mark pins for usage
		mask = mergePins<firstPin_c::PIN_NUMBER(), (restPins_c::PIN_NUMBER())...>(mask);
		port_c::getUsageMask(port) &= ~mask;

		// disable clocks
		if (port_c::getUsageMask(port) == 0)
			ClockDisable(port);
		return mask;
	}

};

/**
 * ALl CPU pins MUST be defined here!
 */
using PB15 = pin_c<port_c::port_t::B, 15>;
using PB14 = pin_c<port_c::port_t::B, 14>;
using PB13 = pin_c<port_c::port_t::B, 13>;
using PB8 = pin_c<port_c::port_t::B, 8>;
using PB9 = pin_c<port_c::port_t::B, 9>;


#endif /* INCLUDE_PINS_HPP_ */
