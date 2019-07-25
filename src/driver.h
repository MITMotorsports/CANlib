#ifndef __DRIVER_H
#define __DRIVER_H

#include "drivers/inc/stm32f4xx.h"
#include "bus.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
bool HAL_CANlib_ReadFrame(CAN_HandleTypeDef *hcan, Frame* out);
bool HAL_CANlib_ReadFrameFromFIFO(CAN_HandleTypeDef *hcan, uint32_t RxFifo, Frame* out);
CAN_Raw_Bus_T CANlib_GetRawBus(CANlib_Bus_T bus);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // __DRIVER_H
