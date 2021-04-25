#pragma once

// INCLUDE THIS AFTER YOUR DRIVER

#if !(CANLIB_ARCH_STM32F4XX || CANLIB_ARCH_STM32F2XX || CANLIB_ARCH_TESTFAMILY)
#error "No architecture specified!"
#endif

#ifdef CANLIB_ARCH_STM32F4xx
#include "drivers/stm32f4xx.hpp"
#elif defined(CANLIB_ARCH_STM32F2XX)
#include "drivers/stm32f2xx.hpp"
#elif defined(CANLIB_ARCH_TESTFAMILY)
#include "drivers/testfamily.hpp"
#else
#error "No architecture specified!"
#endif

#include "bus.hpp"

namespace CANlib {
TransmitError transmit_frame(const Frame &frame, AbstractBus bus_name);
void read_frame(Frame &frame, RawBus bus);
}  // namespace CANlib
