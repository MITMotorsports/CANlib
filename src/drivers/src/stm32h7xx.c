#include "static.h"

#include "drivers/inc/stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "bus.h"
#include "driver.h"
#include <stdint.h>
#include <string.h>
#ifdef USING_LOGGING_CALLBACK
  #include "log.h"
#endif

extern FDCAN_HandleTypeDef hfdcan1;
// TODO: do some macro crap to automatically deal with these if not used
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
  CAN_Raw_Bus_T raw_bus = CANlib_GetRawBus(bus);
  int bus_num;
  FDCAN_HandleTypeDef* hfdcan;
  switch(raw_bus) {
    case CAN_1:
      hfdcan = &hfdcan1;
      bus_num = 1;
      break;
    case CAN_2:
      hfdcan = &hfdcan2;
      bus_num = 2;
      break;
    case CAN_3:
      hfdcan = &hfdcan3;
      bus_num = 3;
      break;
    default:
      return HAL_ERROR;
  }

  FDCAN_TxHeaderTypeDef pHeader;

  pHeader.ErrorStateIndicator = 0;
  pHeader.BitRateSwitch = 0;
  pHeader.DataLength = FDCAN_DLC_BYTES_8;
  //pHeader.DataLength = frame->dlc;
  pHeader.Identifier= frame->id;
  pHeader.IdType = frame->extended ? FDCAN_EXTENDED_ID: FDCAN_STANDARD_ID;
  pHeader.FDFormat =  FDCAN_CLASSIC_CAN;
  pHeader.TxFrameType = FDCAN_DATA_FRAME;
  //pHeader.TransmitGlobalTime = DISABLE; // Don't replace last 2 bytes of data with TX time.
#ifdef USING_LOGGING_CALLBACK
  log_frame(frame, bus_num);
#else
  UNUSED(bus_num);
#endif
  return HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &pHeader, frame->data);
}

void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) { CAN_Raw_Bus_T raw_bus = CANlib_GetRawBus(bus);
  FDCAN_HandleTypeDef *hfdcan;
  switch(raw_bus) {
    case CAN_1:
      hfdcan = &hfdcan1;
      break;
    case CAN_2:
      hfdcan = &hfdcan2;
      break;
    case CAN_3:
      hfdcan = &hfdcan3;
      break;
    default:
      return;
  }

  uint8_t data[8] = {};
  FDCAN_RxHeaderTypeDef pHeader;
  for (int fifo = 0; fifo < 2; fifo++) { // There are 2 receive FIFOs
      if (HAL_FDCAN_GetRxFifoFillLevel(hfdcan, fifo) > 0) {
        HAL_FDCAN_GetRxMessage(hfdcan, fifo, &pHeader, data);
        frame->id = pHeader.IdType == FDCAN_STANDARD_ID ? pHeader.Identifier: pHeader.Identifier;
        frame->dlc = pHeader.DataLength;

        memcpy(frame->data, data, sizeof(data));
        frame->extended = pHeader.IdType == FDCAN_EXTENDED_ID;
        return;
      }
  }
}

Time_T CANlib_GetTick(void) {
  return HAL_GetTick();
}
