#include "drivers/inc/stm32f4xx.h"
#include "bus.h"

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus);
CAN_Raw_Bus_T CANlib_GetRawBus(CANlib_Bus_T bus);
