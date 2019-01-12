#ifndef __STM32F4XX_CAN_H
#define __STM32F4XX_CAN_H

#include "can_raw.h"
#include "stm32f4xx_hal.h"

#include <stdint.h>

typedef HAL_StatusTypeDef CANlib_Transmit_Error_T;
typedef HAL_StatusTypeDef CANlib_Init_Error_T;

HAL_StatusTypeDef CANlib_Init(uint32_t baudrate);
HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame);

#endif // __STM32F4XX_CAN_H
