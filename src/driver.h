#ifndef __DRIVER_H
#define __DRIVER_H

// INCLUDE THIS AFTER YOUR DRIVER

#include "drivers/inc/stm32h7xx.h"
#include "bus.h"

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus);
CAN_Raw_Bus_T CANlib_GetRawBus(CANlib_Bus_T bus);

#endif // __DRIVER_H
