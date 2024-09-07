#pragma once

// INCLUDE THIS AFTER YOUR DRIVER

#ifndef CANLIB_ARCH
#error "No architecture specified!"
#endif

#if CANLIB_ARCH == STM32F4xx
#include "drivers/inc/stm32f4xx/stm32f4xx.h"
#elif CANLIB_ARCH == STM32F2xx
#include "drivers/inc/stm32f2xx/stm32f2xx.h"
#else
#error "No valid architecture specified!"
#endif

#include "bus.h"

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus);
CAN_Raw_Bus_T CANlib_GetRawBus(CANlib_Bus_T bus);
