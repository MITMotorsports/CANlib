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
Transmit_Error_T TransmitFrame(const Frame &frame, Bus_Names_T bus);
void ReadFrame(Frame &frame, Raw_Bus_T bus);
}  // namespace CANlib
