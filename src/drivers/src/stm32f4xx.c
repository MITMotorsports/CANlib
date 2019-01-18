#include "static.h"

#include "stm32f4xx_hal.h"
#include "bus.h"
#include "driver.h"
#include <stdint.h>

extern CAN_HandleTypeDef can1;
extern CAN_HandleTypeDef can2;
extern CAN_HandleTypeDef can3;

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
  CAN_Raw_Bus_T raw_bus = CANlib_get_raw_bus(bus);
  CAN_HandleTypeDef *hcan;
  switch(raw_bus) {
    case CAN_1:
      hcan = &can1;
      break;
    case CAN_2:
      hcan = &can2;
      break;
    case CAN_3:
      hcan = &can3;
      break;
  }

  CAN_TxHeaderTypeDef pHeader;
  uint32_t pTxMailbox = 0;

  pHeader.DLC = frame->dlc;
  pHeader.StdId = frame->id;
  pHeader.IDE = frame->extended ? CAN_ID_EXT: CAN_ID_STD;
  pHeader.RTR = CAN_RTR_DATA; // Data frame (as opposed to a remote frame)
  pHeader.TransmitGlobalTime = DISABLE; // Don't replace last 2 bytes of data with TX time.
  return HAL_CAN_AddTxMessage(hcan, &pHeader, frame->data, &pTxMailbox);
}

void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) {
  CAN_Raw_Bus_T raw_bus = CANlib_get_raw_bus(bus);
  CAN_HandleTypeDef *hcan;
  switch(raw_bus) {
    case CAN_1:
      hcan = &can1;
      break;
    case CAN_2:
      hcan = &can2;
      break;
    case CAN_3:
      hcan = &can3;
      break;
  }

  uint8_t data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  CAN_RxHeaderTypeDef pHeader;
  HAL_StatusTypeDef err = HAL_OK;
  for (int fifo = 0; fifo < 2; fifo++) { // There are 2 receive FIFOs
      if (HAL_CAN_GetRxFifoFillLevel(hcan, fifo) > 0) {
        err = HAL_CAN_GetRxMessage(hcan, fifo, &pHeader, data);
        frame->id = pHeader.IDE == CAN_ID_STD ? pHeader.StdId : pHeader.ExtId;
        frame->dlc = pHeader.DLC;
        for (int i = 0; i < 8; i++) {
          frame->data[i] = data[i];
        }
        frame->extended = pHeader.IDE == CAN_ID_EXT;
        return err;
      }
  }
  return err;
}


