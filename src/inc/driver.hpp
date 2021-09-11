#pragma once

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
RawBus get_raw_bus(AbstractBus bus_name);
AbstractBus get_bus_name(RawBus bus);
}  // namespace CANlib
