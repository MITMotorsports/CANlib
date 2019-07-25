#include "static.h"

#include "stm32f4xx_hal.h"
#include "bus.h"
#include "driver.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern CAN_HandleTypeDef hcan3;

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
  CAN_Raw_Bus_T raw_bus = CANlib_GetRawBus(bus);
  int bus_num;
  CAN_HandleTypeDef* hcan;
  switch(raw_bus) {
    case CAN_1:
      hcan = &hcan1;
      bus_num = 1;
      break;
    case CAN_2:
      hcan = &hcan2;
      bus_num = 2;
      break;
    case CAN_3:
      hcan = &hcan3;
      bus_num = 3;
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
#ifdef USING_LOGGING_CALLBACK
  log_frame(frame, bus_num);
#else
  UNUSED(bus_num);
#endif
  return HAL_CAN_AddTxMessage(hcan, &pHeader, frame->data, &pTxMailbox);
}

static void HAL_CANlib_ConvertFrame(CAN_RxHeaderTypeDef* pHeader, uint8_t data[8], Frame *out) {
  out->id = pHeader->IDE == CAN_ID_STD ? pHeader->StdId : pHeader->ExtId;
  out->dlc = pHeader->DLC;
  memcpy(out->data, data, 8);
  out->extended = pHeader->IDE == CAN_ID_EXT;
}

bool HAL_CANlib_ReadFrameFromFIFO(CAN_HandleTypeDef *hcan, uint32_t RxFifo, Frame* out) {
  uint8_t data[8] = {};
  CAN_RxHeaderTypeDef pHeader;
  if (HAL_CAN_GetRxFifoFillLevel(hcan, RxFifo) > 0) {
    if (HAL_CAN_GetRxMessage(hcan, RxFifo, &pHeader, data) == HAL_OK) {
        HAL_CANlib_ConvertFrame(&pHeader, data, out);
        return true;
    }
  }
  return false;
}

bool HAL_CANlib_ReadFrame(CAN_HandleTypeDef *hcan, Frame* out) {
  for (uint8_t fifo = 0; fifo < 2; fifo++) {
    if (HAL_CANlib_ReadFrameFromFIFO(hcan, fifo, out)) {
      return true;
    }
  }
  return false;
}
