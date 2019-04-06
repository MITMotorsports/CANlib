#include "static.h"

#include "stm32f4xx_hal.h"
#include "bus.h"
#include "driver.h"
#include <stdint.h>
#include <string.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern CAN_HandleTypeDef hcan3;

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
  CAN_Raw_Bus_T raw_bus = CANlib_GetRawBus(bus);
  CAN_HandleTypeDef* hcan;
  switch(raw_bus) {
    case CAN_1:
      hcan = &hcan1;
      break;
    case CAN_2:
      hcan = &hcan2;
      break;
    case CAN_3:
      hcan = &hcan3;
      break;
    default:
      return HAL_ERROR;
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
  CAN_Raw_Bus_T raw_bus = CANlib_GetRawBus(bus);
  CAN_HandleTypeDef *hcan;
  switch(raw_bus) {
    case CAN_1:
      hcan = &hcan1;
      break;
    case CAN_2:
      hcan = &hcan2;
      break;
    case CAN_3:
      hcan = &hcan3;
      break;
    default:
      return;
  }

  uint8_t data[8] = {};
  CAN_RxHeaderTypeDef pHeader;
  for (int fifo = 0; fifo < 2; fifo++) { // There are 2 receive FIFOs
      if (HAL_CAN_GetRxFifoFillLevel(hcan, fifo) > 0) {
        HAL_CAN_GetRxMessage(hcan, fifo, &pHeader, data);
        frame->id = pHeader.IDE == CAN_ID_STD ? pHeader.StdId : pHeader.ExtId;
        frame->dlc = pHeader.DLC;

        memcpy(frame->data, data, sizeof(data));
        frame->extended = pHeader.IDE == CAN_ID_EXT;
        return;
      }
  }
}


