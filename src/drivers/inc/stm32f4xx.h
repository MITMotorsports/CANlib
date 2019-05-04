#ifndef __STM32F4XX_CAN_H
#define __STM32F4XX_CAN_H

#include "static.h"
#include "stm32f4xx_hal.h"
#include "bus.h"

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t Time_T; // in ms
typedef HAL_StatusTypeDef CANlib_Transmit_Error_T;
typedef HAL_StatusTypeDef CANlib_Init_Error_T;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
bool HAL_CANlib_ReadFrame(CAN_HandleTypeDef *hcan, Frame* out);
bool HAL_CANlib_ReadFrameFromFIFO(CAN_HandleTypeDef *hcan, uint32_t RxFifo, Frame* out);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // __STM32F4XX_CAN_H
