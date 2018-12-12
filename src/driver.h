#include "drivers/inc/stm32f4xx.h"

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame);
CANlib_Init_Error_T CANlib_Init(uint32_t baudrate);
