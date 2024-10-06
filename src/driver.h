#pragma once
#define STM32H7xx 0
#define STM32F2xx 1
#define STM32F4xx 2
#define STM32H5xx 3

// INCLUDE THIS AFTER YOUR DRIVER
#ifndef CANLIB_ARCH
#error "No architecture specified!"
#endif

// theo-todo: for some reason we were hitting the first if statement even when canlib_arch has set to h7
#if CANLIB_ARCH == STM32F4xx
#include "drivers/inc/stm32f4xx/stm32f4xx.h"
#elif CANLIB_ARCH == STM32F2xx
#include "drivers/inc/stm32f2xx/stm32f2xx.h"
#elif CANLIB_ARCH == STM32H7xx
#include "drivers/inc/stm32h7xx/stm32h7xx.h"
#elif CANLIB_ARCH == STM32H5xx
#include "drivers/inc/stm32h5xx/stm32h5xx.h"
#else
#error "Architecture not supported!"
#endif

#include "bus.h"

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus);
CAN_Raw_Bus_T CANlib_GetRawBus(CANlib_Bus_T bus);
