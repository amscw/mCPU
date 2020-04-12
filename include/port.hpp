/*
 * port.hpp
 *
 *  Created on: 12 апр. 2020 г.
 *      Author: moskvin
 */

#ifndef INCLUDE_PORT_HPP_
#define INCLUDE_PORT_HPP_

#include "pin.hpp"
#include "typelist.hpp"
#include "pinlist.hpp"
#include "bithacks.hpp"
#include "common.hpp"

//-----------------------------------------------------------------------------
// Выполняет конфигурацию для отдельно взятого порта
//-----------------------------------------------------------------------------
/**
 * Структура нужна для обхода ограничения частичной специализации шаблонов функций
 */
template<typename TL1, typename TL2> struct PortConfigure;

template<typename ... Pins> struct PortConfigure<utils::EmptyTypeList,  utils::TypeList<Pins...>>
{
	static void everyAsOutput(GPIO::speed_t s) noexcept
	{
	}
};


template<typename ... Ports, typename ... Pins> struct PortConfigure<utils::TypeList<Ports...>,  utils::TypeList<Pins...>>
{
private:
	// откусываем "голову" списка портов
	typedef typename utils::TypeList<Ports...>::Head Port;
	// получаем список пинов для порта "головы"
	typedef typename TakePinsAtPort<Port, utils::TypeList<Pins...>>::type PinList;
	// получаем маски пинов
	enum {	// !!! WARNING !!! это будет неявное расширение до std::uint64_t так что при использовании нужен каст вниз!
		_1BIT_FIELD_MASK = std::uint16_t(MakePortMask<PinList>::VALUE),
		_2BIT_FIELD_MASK = std::uint32_t(utils::ExpandBits(_1BIT_FIELD_MASK)),
		_4BIT_FIELD_MASK = std::uint64_t(utils::ExpandBits(_2BIT_FIELD_MASK)),
		_1BIT_FIELD_MASK_INV = std::uint16_t(~_1BIT_FIELD_MASK),
		_2BIT_FIELD_MASK_INV = std::uint32_t(~_2BIT_FIELD_MASK),
		_4BIT_FIELD_MASK_INV = std::uint64_t(~_4BIT_FIELD_MASK),
	};

public:
	// настроить на выход
	static void everyAsOutput(GPIO::speed_t s) noexcept
	{
		std::uint32_t tmp;
		constexpr const std::uint32_t MODER_FIELDS = BitField<2, GPIO::mode_t, PinList>::Merge(GPIO::mode_t::OUTPUT);
		/* constexpr */ const std::uint32_t OSPEEDR_FIELDS = BitField<2, GPIO::speed_t, PinList>::Merge(s);

		// 1. set output type - push/pull
		tmp = Port::GPIOx()->OTYPER &= _1BIT_FIELD_MASK_INV;

		// 2. set speed
		tmp = Port::GPIOx()->OSPEEDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= OSPEEDR_FIELDS;
		Port::GPIOx()->OSPEEDR = tmp;

		// 3. disable pull-up/pull-down
		Port::GPIOx()->PUPDR &= _2BIT_FIELD_MASK_INV;

		// 0. set MODER
		tmp = Port::GPIOx()->MODER;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= MODER_FIELDS;
		Port::GPIOx()->MODER = tmp;

#if defined(DEBUG_GPIO)
		PRINT("regdump:");
		Port::DumpPortReg(Port::GPIOx()->OTYPER);
		Port::DumpPortReg(Port::GPIOx()->OSPEEDR);
		Port::DumpPortReg(Port::GPIOx()->PUPDR);
		Port::DumpPortReg(Port::GPIOx()->MODER);
#endif // DEBUG_GPIO

		// рекурсивно настраиваем остальные порты из списка
		PortConfigure<typename utils::TypeList<Ports...>::Tail, PinList>::everyAsOutput(s);
	}

	// настроить на выход с открытым стоком
	static void everyAsOutputOD(GPIO::pull_t pupd, GPIO::speed_t s) noexcept
	{
		std::uint32_t tmp;
		constexpr const std::uint32_t MODER_FIELDS = BitField<2, GPIO::mode_t, PinList>::Merge(GPIO::mode_t::OUTPUT);
		/* constexpr */ const std::uint32_t OSPEEDR_FIELDS = BitField<2, GPIO::speed_t, PinList>::Merge(s);
		/* constexpr */ const std::uint32_t PUPDR_FIELDS = BitField<2, GPIO::pull_t, PinList>::Merge(pupd);

		// 1. set output type - open drain
		Port::GPIOx()->OTYPER |= _1BIT_FIELD_MASK;

		// 2. set speed
		tmp = Port::GPIOx()->OSPEEDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= OSPEEDR_FIELDS;
		Port::GPIOx()->OSPEEDR = tmp;

		// 3. set pull-up/pull-down
		tmp = Port::GPIOx()->PUPDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= PUPDR_FIELDS;
		Port::GPIOx()->PUPDR = tmp;

		// 0. set MODER
		tmp = Port::GPIOx()->MODER;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= MODER_FIELDS;
		Port::GPIOx()->MODER = tmp;

#if defined(DEBUG_GPIO)
		PRINT("regdump:");
		Port::DumpPortReg(Port::GPIOx()->OTYPER);
		Port::DumpPortReg(Port::GPIOx()->OSPEEDR);
		Port::DumpPortReg(Port::GPIOx()->PUPDR);
		Port::DumpPortReg(Port::GPIOx()->MODER);
#endif

		// рекурсивно настраиваем остальные порты из списка
		PortConfigure<typename utils::TypeList<Ports...>::Tail, PinList>::everyAsOutputOD(pupd, s);
	}

	// Configure pin as digital input
	static void ConfigAsInput(GPIO::pull_t pupd) noexcept
	{
		std::uint32_t tmp;
		constexpr const std::uint32_t MODER_FIELDS = BitField<2, GPIO::mode_t, PinList>::Merge(GPIO::mode_t::INPUT);
		/* constexpr */ const std::uint32_t PUPDR_FIELDS = BitField<2, GPIO::pull_t, PinList>::Merge(pupd);

		// 3. set pull-up/pull-down
		tmp = Port::GPIOx()->PUPDR;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= PUPDR_FIELDS;
		Port::GPIOx()->PUPDR = tmp;

		// 0. set MODER
		tmp = Port::GPIOx()->MODER;
		tmp &= _2BIT_FIELD_MASK_INV;
		tmp |= MODER_FIELDS;
		Port::GPIOx()->MODER = tmp;

#if defined(DEBUG_GPIO)
		PRINT("regdump:");
		Port::DumpPortReg(Port::GPIOx()->PUPDR);
		Port::DumpPortReg(Port::GPIOx()->MODER);
#endif

		// рекурсивно настраиваем остальные порты из списка
		PortConfigure<typename utils::TypeList<Ports...>::Tail, PinList>::ConfigAsInput(pupd);
	}

};



//-----------------------------------------------------------------------------
// Выполняет конфигурацию произвольного списка пинов
//-----------------------------------------------------------------------------
template<typename TL> struct Configure;

/**
 * Частичная спициализация
 * TL = TypeList<PWs...>
 */
template<typename ... PWs> class Configure<utils::TypeList<PWs...>>
{
	// сохраняем список пинов
	typedef utils::TypeList<PWs...> PinList;
	// сохраняем список их портов, удаляем дубликаты
	typedef typename utils::RemoveDuplicates<typename MakePorts<PinList>::type>::type PortList;

public:
	static inline void AsOutput(GPIO::speed_t s = GPIO::speed_t::HIGH) noexcept
	{
		PortConfigure<PortList, PinList>::everyAsOutput(s);
	}

	static inline void AsOutputOD(GPIO::pull_t pupd, GPIO::speed_t s = GPIO::speed_t::HIGH) noexcept
	{
		PortConfigure<PortList, PinList>::everyAsOutputOD(pupd, s);
	}
};

#endif /* INCLUDE_PORT_HPP_ */
