/*
 * pin_list.hpp
 *
 *  Created on: 11 апр. 2020 г.
 *      Author: moskvin
 *      !!! WARNING !!! need to be include typelist.hpp, pin.hpp
 */

#ifndef INCLUDE_PIN_LIST_HPP_
#define INCLUDE_PIN_LIST_HPP_

//-----------------------------------------------------------------------------
// Обёртка для задания номера позиции в списке
//-----------------------------------------------------------------------------
template <typename T, int I> struct PinWrapper
{
	typedef T Pin;
	enum {INDEX = I};
};

//-----------------------------------------------------------------------------
// Шаблон, генерирующий список обёрток пинов,
// в которых содержится их порядковый номер (Index)
// На выходе получаем список:
// Голова = PinWrapper<T1, 0>
// Хвост = TypeList<PinWrapper<T2, 1>, PinWrapper<T3, 2>,...>
//-----------------------------------------------------------------------------
template<typename TL, typename Is> struct MakePinWrappers;

/**
 * Частичная специализация
 * Применяется паттерн раскрытия по схеме "inner-join"
 * Паттерн раскрытия - это всё, что стоит в порядке c-decl, начиная от элипсиса
 * Раскрытие применяется в аргументах шаблонов
 */
template<typename ... Pins, std::size_t ... Is> struct MakePinWrappers<utils::TypeList<Pins...>, std::index_sequence<Is...>>
{
	typedef utils::TypeList<PinWrapper<typename utils::TypeList<Pins>::Head, Is>...> type;
};

//-----------------------------------------------------------------------------
// Сформировать список портов на основе списка пинов
//-----------------------------------------------------------------------------
template<typename TL> struct MakePorts;

template<typename ... PWs> struct MakePorts<utils::TypeList<PWs...>>
{
	typedef utils::TypeList<typename utils::TypeList<PWs>::Head::Pin::PortType...> type;
};

//-----------------------------------------------------------------------------
// Сформировать список пинов для конкретного порта
//-----------------------------------------------------------------------------
template<typename T, typename TL> struct TakePinsAtPort;

/**
 * Частичная специализация
 * T = Port<PortLetter>
 * TL = TypeList<PinWrapper<pin_c<PortLetter, N>, Index>, PWs...>
 * Если попали сюда, добавляем голову в список
 */
template<GPIO::letter_t PortLetter, int N, int Index, typename ... PWs> struct TakePinsAtPort<
	Port<PortLetter>,
	utils::TypeList<PinWrapper<Pin<PortLetter, N>, Index>, PWs...>
>
{
private:
	typedef utils::TypeList<PinWrapper<Pin<PortLetter, N>, Index>, PWs...> TL;
	typedef Port<PortLetter> PortType;

public:
	typedef typename utils::Append<
		typename TakePinsAtPort<PortType, typename TL::Tail>::type,
		utils::TypeList<typename TL::Head>
	>::type type;
};

/**
 * Частичная специализация
 * (менее специализированная версия)
 * Если попали сюда, значит порт не наш, продолжаем рекурсию
 */
template<typename T, typename ... PWs> struct TakePinsAtPort<T, utils::TypeList<PWs...>>
{
	typedef typename TakePinsAtPort<T, typename utils::TypeList<PWs...>::Tail>::type type;
};

/**
 * Частичная специализация
 * (пустой список)
 */
template<typename T> struct TakePinsAtPort<T, utils::EmptyTypeList>
{
	typedef utils::EmptyTypeList type;
};

//-----------------------------------------------------------------------------
// Сформировать маски портов на основе списка обёрток
//-----------------------------------------------------------------------------
template <typename T> struct MakePortMask;

/**
 * Частичная специализация
 * T = TypeList<PWs...>
 */
template <typename ... PWs> struct MakePortMask<utils::TypeList<PWs...>>
{
private:
	typedef utils::TypeList<PWs...> TL;
	typedef typename TL::Head::Pin Pin;

public:
	enum {VALUE = (1 << Pin::NUMBER) | MakePortMask<typename TL::Tail>::VALUE};
};

/**
 * Полная специализация
 * (пустой список)
 */
template <> struct MakePortMask<utils::EmptyTypeList>
{
	enum {VALUE = 0};
};

//-----------------------------------------------------------------------------
// Истинность факта последовательного расположения пинов в порту
//-----------------------------------------------------------------------------
/**
 * Общий шаблон
 */
template<typename TL> struct IsSerial;

/**
 * Частичная специализация для списка из, минимум, двух PWs
 * TL = TypeList<PWs...>
 */
template<typename PW, typename PWNext, typename ... PWRest> struct IsSerial<utils::TypeList<PW, PWNext, PWRest...>> : std::integral_constant<
	bool,
	(PW::Pin::NUMBER == (PWNext::Pin::NUMBER - 1)) && IsSerial<utils::TypeList<PWNext, PWRest...>>::value
>
{

};

/**
 * Частичная специализация списком из одного элемента
 */
template<typename PW> struct IsSerial<utils::TypeList<PW>> : std::true_type
{

};

/**
 * Полная специалзация для пустого списка
 */
template<> struct IsSerial<utils::EmptyTypeList> : std::true_type
{

};

//-----------------------------------------------------------------------------
// Маппинг входного значения на пины порта
//-----------------------------------------------------------------------------
template<typename TL> struct PinMapper;

/**
 * Полная специализация
 * TL = пустой список
 */
template<> class PinMapper<utils::EmptyTypeList>
{
public:
	static unsigned int MapToPins(unsigned int value) noexcept
	{
		return 0;
	}
};

/**
 * Частичная специализация
 * TL = TypeList<PWs...>
 *
 * Пример: запись значения 0x5 в порт. Возможны три случая:
 * 1) <PA3, PA4, PA5> = 0x5 << 3
 * 2) <PA0, PA1, PA2> = 0x5
 * 3) <PA0, PA2, PA7> = 1 << 0 | 0 << 2 | 1 << 7
 *
 * Таким образом, мы должны выполнить маппинг входного значения в порт
 * по одному из трёх алгоритмов
 */
template<typename ... PWs> class PinMapper<utils::TypeList<PWs...>>
{
	typedef typename utils::TypeList<PWs...>::Head PW;
	enum {PORT_MASK = MakePortMask<utils::TypeList<PWs...>>::VALUE};

public:
	static unsigned int MapToPins(unsigned int value) noexcept
	{
		unsigned int result = 0;

		// случай 1) - просто сдвиг
		if (IsSerial<utils::TypeList<PWs...>>::value)
		{
			if (PW::INDEX > PW::Pin::NUMBER)
			{
				// старшая часть значения в младшие биты
				return (value >> (PW::INDEX - PW::Pin::NUMBER)) & PORT_MASK;
			} else {
				// младшая часть значения в старшие биты
				return (value << (PW::Pin::NUMBER - PW::INDEX)) & PORT_MASK;
			}
		}

		if (PW::INDEX == PW::Pin::NUMBER)
		{
			// случай 2) - маппинг 1 к 1
			result = value & (1 << PW::INDEX);
		} else {
			// случай 3) - маппинг в произвольные позиции
			if (value & (1 << PW::INDEX))
				result = 1 << PW::Pin::NUMBER;
		}

		// рекурсивно обрабатываем хвост
		return result | PinMapper<typename utils::TypeList<PWs...>::Tail>::MapToPins(value);
	};
};

//-----------------------------------------------------------------------------
// объединение битовых полей
//-----------------------------------------------------------------------------
template<int w, typename T, typename TL> struct BitField;

/**
 * 1-битовое поле и пустой список
 */
template<typename T> struct BitField<1, T, utils::EmptyTypeList>
{
	static constexpr std::uint32_t Merge(T value)
	{
		return 0;
	}
};

/**
 * 1-битовое поле и непустой список
 */
template<typename T, typename ... PWs> struct BitField<1, T, utils::TypeList<PWs...>>
{
	static constexpr std::uint32_t Merge(T value)
	{
		static_assert(utils::TypeList<PWs...>::Head::Pin::NUMBER < 32, "NUMBER is too big");
		return ((static_cast<std::uint32_t>(value) & 0x1) << utils::TypeList<PWs...>::Head::pin::NUMBER)
				| BitField<1, T, typename utils::TypeList<PWs...>::Tail>::Merge(value);
	}
};

/**
 * 2-битовое поле и пустой список
 */
template<typename T> struct BitField<2, T, utils::EmptyTypeList>
{
	static constexpr std::uint32_t Merge(T value)
	{
		return 0;
	}
};

/**
 * 2-битовое поле и непустой список
 */
template<typename T, typename ... PWs> struct BitField<2, T, utils::TypeList<PWs...>>
{
	static constexpr std::uint32_t Merge(T value)
	{
		static_assert(utils::TypeList<PWs...>::Head::Pin::NUMBER < 16, "NUMBER is too big");
		return ((static_cast<std::uint32_t>(value) & 0x3)
				<< (utils::TypeList<PWs...>::Head::Pin::NUMBER << 1))
				| BitField<2, T, typename utils::TypeList<PWs...>::Tail>::Merge(value);
	}
};

/**
 * 4-ёх битовое поле и пустой список
 */
template<typename T> struct BitField<4, T, utils::EmptyTypeList>
{
	static constexpr std::uint32_t Merge(T value)
	{
		return 0;
	}
};

/**
 * 4-ёх битовое поле и непустой список
 */
template<typename T, typename ... PWs> struct BitField<4, T, utils::TypeList<PWs...>>
{
	static constexpr std::uint64_t Merge(T value)
	{
		static_assert(utils::TypeList<PWs...>::Head::Pin::NUMBER < 16, "NUMBER is too big");
		return ((static_cast<std::uint32_t>(value) & 0x3)
				<< (utils::TypeList<PWs...>::Head::Pin::NUMBER << 1))
				| BitField<4, T, typename utils::TypeList<PWs...>::Tail>::Merge(value);
	}
};

#endif /* INCLUDE_PIN_LIST_HPP_ */
