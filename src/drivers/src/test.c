#include "../inc/test.h"
#include <stdio.h>

CANlib_Transmit_Error_T CANlib_TransmitFrame(Frame *frame){
  printf("id: %d dlc: %d\n", frame->id, frame->dlc);
  return true;
}

CANlib_Init_Error_T CANlib_Init(uint32_t baudrate) {
  printf("Initialized %d!\n", baudrate);
  return true;
}
