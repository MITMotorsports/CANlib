#ifndef __STM32F4XX_CAN_H
#define __STM32F4XX_CAN_H

#include "static.h"
#include "stm32f4xx_hal.h"
#include "bus.h"

#include <stdint.h>

typedef uint32_t Time_T; // in ms
typedef HAL_StatusTypeDef CANlib_Transmit_Error_T;
typedef HAL_StatusTypeDef CANlib_Init_Error_T;

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus);

#endif // __STM32F4XX_CAN_H
