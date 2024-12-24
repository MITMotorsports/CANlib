#include "static.h"

#include <stdint.h>
#include <string.h>
#include "bus.h"
#include "driver.h"
#include "drivers/inc/stm32h5xx/stm32h5xx.h"
#include "stm32h5xx_hal.h"
#include "logger.h"
#include "clock.h"
#include CANLIB_INCLUDE

extern FDCAN_HandleTypeDef hfdcan1; // critical

uint32_t CANlib_Get_DLC_Code(uint8_t num_bytes) {
  switch(num_bytes) {
    case 0:
      return FDCAN_DLC_BYTES_0;
    case 1:
      return FDCAN_DLC_BYTES_1;
    case 2:
      return FDCAN_DLC_BYTES_2;
    case 3:
      return FDCAN_DLC_BYTES_3;
    case 4:
      return FDCAN_DLC_BYTES_4;
    case 5:
      return FDCAN_DLC_BYTES_5;
    case 6:
      return FDCAN_DLC_BYTES_6;
    case 7:
      return FDCAN_DLC_BYTES_7;
    case 8:
      return FDCAN_DLC_BYTES_8;
    case 12:
      return FDCAN_DLC_BYTES_12;
    case 16:
      return FDCAN_DLC_BYTES_16;
    case 20:
      return FDCAN_DLC_BYTES_20;
    case 24:
      return FDCAN_DLC_BYTES_24;
    case 32:
      return FDCAN_DLC_BYTES_32;
    case 48:
      return FDCAN_DLC_BYTES_48;
    case 64:
      return FDCAN_DLC_BYTES_64;
  }
  return FDCAN_DLC_BYTES_0;
}

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
  FDCAN_HandleTypeDef *hcan = CANTypeDef_From_BusT(bus);

  FDCAN_TxHeaderTypeDef pHeader;
  pHeader.Identifier = frame->id;
  pHeader.IdType = frame->extended ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
  pHeader.TxFrameType = FDCAN_DATA_FRAME;
  pHeader.DataLength = CANlib_Get_DLC_Code(frame->dlc);
  pHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  pHeader.BitRateSwitch = 0;
  pHeader.FDFormat = FDCAN_CLASSIC_CAN;
  pHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  pHeader.MessageMarker = 0;      // Don't replace last 2 bytes of data with TX time.
  HAL_StatusTypeDef res = HAL_FDCAN_AddMessageToTxFifoQ(hcan, &pHeader, frame->data);
  return res;
}

void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) {
  UNUSED(bus);
  FDCAN_HandleTypeDef *hcan = CANTypeDef_From_BusT(bus);

  uint8_t data[8] = {};
  FDCAN_RxHeaderTypeDef pHeader;
  uint32_t fifo_address = FDCAN_RX_FIFO0;
  for (int fifo = 0; fifo < 2; fifo++) {  // There are 2 receive FIFOs
    if (HAL_FDCAN_GetRxFifoFillLevel(hcan, fifo_address) != 0) {
      HAL_FDCAN_GetRxMessage(hcan, fifo_address, &pHeader, data);
      frame->id  = pHeader.Identifier;
      frame->dlc = pHeader.DataLength;

      memcpy(frame->data, data, sizeof(data));
      frame->extended = pHeader.IdType == FDCAN_EXTENDED_ID;
      return;
    }
    fifo_address = FDCAN_RX_FIFO1;
  }
}

Time_T CANlib_GetTick(void) {
  return HAL_GetTick();
}