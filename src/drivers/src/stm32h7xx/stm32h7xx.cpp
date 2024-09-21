#include "static.h"

#include <stdint.h>
#include <string.h>
#include "bus.h"
#include "driver.h"
#include "drivers/inc/stm32h7xx/stm32h7xx.h"
#include "stm32h7xx_hal.h"
#ifdef USING_LOGGING_CALLBACK
#include "log.h"
#endif

extern FDCAN_HandleTypeDef hfdcan1;

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
  UNUSED(bus);
  int bus_num;
  FDCAN_HandleTypeDef *hcan;
  hcan    = &hfdcan1;
  bus_num = 1;

  FDCAN_TxHeaderTypeDef pHeader;
  pHeader.Identifier = frame->id;
  pHeader.IdType = frame->extended ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
  pHeader.TxFrameType = FDCAN_DATA_FRAME;
  pHeader.DataLength = frame->dlc;
  pHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  pHeader.BitRateSwitch = 0;
  pHeader.FDFormat = FDCAN_CLASSIC_CAN;
  pHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  pHeader.MessageMarker = 0;      // Don't replace last 2 bytes of data with TX time.
#ifdef USING_LOGGING_CALLBACK
  log_frame(frame, bus_num);
#else
  UNUSED(bus_num);
#endif
  return HAL_FDCAN_AddMessageToTxFifoQ(hcan, &pHeader, frame->data);
}

void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) {
  UNUSED(bus);
  FDCAN_HandleTypeDef *hcan;
  hcan = &hfdcan1;

  uint8_t data[8] = {};
  FDCAN_RxHeaderTypeDef pHeader;
  for (int fifo = 0; fifo < 2; fifo++) {  // There are 2 receive FIFOs
    if (HAL_FDCAN_IsRxBufferMessageAvailable(hcan, fifo)) {
      HAL_FDCAN_GetRxMessage(hcan, fifo, &pHeader, data);
      frame->id  = pHeader.Identifier;
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
