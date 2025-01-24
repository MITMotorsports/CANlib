#include "static.h"

#include <stdint.h>
#include <string.h>
#include "bus.h"
#include "driver.h"
#include "drivers/inc/stm32h7xx/stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "logger.h"
#include "clock.h"
#include CANLIB_INCLUDE
#ifdef USING_LOGGING_CALLBACK
#include "log.h"
#endif


common::Clock::time_point last_send_time;
uint32_t num_sent;

uint8_t next_can_buffers[4];

uint32_t CANlib_Get_DLC_Code(uint8_t num_bytes) {
  UNUSED(num_bytes);
  // return 0;
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

uint32_t CANlib_TxBufferLocation_From_BufferNumber(uint8_t buffer_nbr) {
  // UNUSED(buffer_nbr);
  // return 0;
  switch(buffer_nbr) {
    case 0:
      return FDCAN_TX_BUFFER0;
    case 1:
      return FDCAN_TX_BUFFER1;
    case 2:
      return FDCAN_TX_BUFFER2;
    case 3:
      return FDCAN_TX_BUFFER3;
    case 4:
      return FDCAN_TX_BUFFER4;
    case 5:
      return FDCAN_TX_BUFFER5;
    case 6:
      return FDCAN_TX_BUFFER6;
    case 7:
      return FDCAN_TX_BUFFER7;
    case 8:
      return FDCAN_TX_BUFFER8;
    case 9:
      return FDCAN_TX_BUFFER9;
    case 10:
      return FDCAN_TX_BUFFER10;
    case 11:
      return FDCAN_TX_BUFFER11;
    case 12:
      return FDCAN_TX_BUFFER12;
    case 13:
      return FDCAN_TX_BUFFER13;
    case 14:
      return FDCAN_TX_BUFFER14;
    case 15:
      return FDCAN_TX_BUFFER15;
    case 16:
      return FDCAN_TX_BUFFER16;
    case 17:
      return FDCAN_TX_BUFFER17;
    case 18:
      return FDCAN_TX_BUFFER18;
    case 19:
      return FDCAN_TX_BUFFER19;
    case 20:
      return FDCAN_TX_BUFFER20;
    case 21:
      return FDCAN_TX_BUFFER21;
    case 22:
      return FDCAN_TX_BUFFER22;
    case 23:
      return FDCAN_TX_BUFFER23;
    case 24:
      return FDCAN_TX_BUFFER24;
    case 25:
      return FDCAN_TX_BUFFER25;
    case 26:
      return FDCAN_TX_BUFFER26;
    case 27:
      return FDCAN_TX_BUFFER27;
    case 28:
      return FDCAN_TX_BUFFER28;
    case 29:
      return FDCAN_TX_BUFFER29;
    case 30:
      return FDCAN_TX_BUFFER30;
    case 31:
      return FDCAN_TX_BUFFER31;
  }
  return 0;
}

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
  // UNUSED(frame);
  // UNUSED(bus);
  FDCAN_HandleTypeDef *hcan = CANTypeDef_From_BusT(bus);

  FDCAN_TxHeaderTypeDef pHeader;
  pHeader.Identifier = frame->id;
  pHeader.IdType = frame->extended ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
  pHeader.TxFrameType = FDCAN_DATA_FRAME;
  pHeader.DataLength = CANlib_Get_DLC_Code(frame->dlc);
  pHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  pHeader.BitRateSwitch = FDCAN_BRS_OFF;
  pHeader.FDFormat = FDCAN_CLASSIC_CAN;
  pHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  pHeader.MessageMarker = 0;      // Don't replace last 2 bytes of data with TX time.

  uint8_t buffer_nbr = next_can_buffers[bus]++;
  if(next_can_buffers[bus] >= hcan->Init.TxBuffersNbr) {
    next_can_buffers[bus] = 0;
  }
  uint32_t buffer_nbr_enum = CANlib_TxBufferLocation_From_BufferNumber(buffer_nbr);
  
  if(HAL_FDCAN_IsTxBufferMessagePending(hcan, buffer_nbr_enum) == 1) {
    LOG_INFO("Trying to send can messages too fast on buffer %u!", buffer_nbr);
    return HAL_BUSY; // busy processing other request
  }

  // if(hcan == &hfdcan3) {
  //   LOG_INFO("Sent on buffer %u -> %lu", buffer_nbr, buffer_nbr_enum);
  // }
  HAL_StatusTypeDef res = HAL_FDCAN_AddMessageToTxBuffer(hcan, &pHeader,
                                                 frame->data, buffer_nbr_enum);
  HAL_StatusTypeDef res2 = HAL_FDCAN_EnableTxBufferRequest(hcan, buffer_nbr_enum);
  // HAL_StatusTypeDef res = HAL_FDCAN_AddMessageToTxFifoQ(hcan, &pHeader, frame->data);
  common::Clock::time_point now = common::Clock::now();
  num_sent++;
  if(now - last_send_time > std::chrono::milliseconds(1000)) {
    LOG_INFO("sent %lu messages", num_sent);
    last_send_time = now;
    num_sent = 0;
  }
  if(res != HAL_OK || res2 != HAL_OK) {
    LOG_INFO("CAN TX ERROR %lu", hcan->ErrorCode);
  }
  return res2;
}

void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) {
  // UNUSED(bus);
  // UNUSED(frame);
  FDCAN_HandleTypeDef *hcan = CANTypeDef_From_BusT(bus);

  uint8_t data[8] = {};
  FDCAN_RxHeaderTypeDef pHeader;
  uint32_t fifo_address = FDCAN_RX_FIFO0;
  for (int fifo = 0; fifo < 2; fifo++) {  // There are 2 receive FIFOs
    if (HAL_FDCAN_IsRxBufferMessageAvailable(hcan, fifo_address)) {
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