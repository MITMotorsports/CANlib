#pragma once

// INCLUDE THIS AFTER YOUR DRIVER

#ifdef CANLIB_ARCH_STM32F4xx
#include "drivers/inc/stm32f4xx.h"
#elif defined(CANLIB_ARCH_STM32F2XX)
#include "drivers/inc/stm32f2xx.h"
#error "No valid architecture specified!"
#endif

#include "bus.h"

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus);
CAN_Raw_Bus_T CANlib_GetRawBus(CANlib_Bus_T bus);
