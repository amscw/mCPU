/*
 * io_channel.hpp
 *
 *  Created on: 1 апр. 2020 г.
 *      Author: moskw
 */

#ifndef INCLUDE_IO_CHANNEL_HPP_
#define INCLUDE_IO_CHANNEL_HPP_

#include <cstdint>
#include "common.hpp"

//enum class ch_t : std::uint8_t { CH1, CH2, CH3, CH4, CH5, CH6, CH7, CH8, NOT_DEF };
//
//template<class A0, class A1, class A2>
//class channel_c
//{
//public:
// 	channel_c() noexcept : mask(pins_c::Use<A0, A1, A2>()), current(ch_t::NOT_DEF)
//	{
// 		pins_c::ConfigAsOutput<A0, A1, A2>();
//	}
// 	~channel_c() noexcept { pins_c::Unuse<A0, A1, A2>(); }
//	inline void Select(ch_t ch) noexcept __attribute__((always_inline))
//	{
//		std::uint8_t value = static_cast<std::uint8_t>(ch);
//		// FIXME: need fix-up on PCB connections A0<->A2
//		MIRROR8(value, 3);
//		pins_c::SetValue<A0::PIN_PORT()>(mask,  value << A2::PIN_NUMBER());
//	}
//
//	inline const ch_t &Current() const noexcept  __attribute__((always_inline)) { return current; }
//
//private:
//	std::uint32_t mask;
//	ch_t current;
//};
//


#endif /* INCLUDE_IO_CHANNEL_HPP_ */
