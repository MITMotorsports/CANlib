#include "drivers/inc/stm32f4xx.h"
#include "bus.h"

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus);
CANlib_Init_Error_T CANlib_Init(uint32_t baudrate);
void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus);
CAN_Raw_Bus_T CANlib_get_raw_bus(CANlib_Bus_T bus);
