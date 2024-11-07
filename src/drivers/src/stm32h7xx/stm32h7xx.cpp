#include "static.h"

#include <stdint.h>
#include <string.h>
#include "bus.h"
#include "driver.h"
#include "drivers/inc/stm32h7xx/stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "logger.h"
#include "clock.h"
#ifdef USING_LOGGING_CALLBACK
#include "log.h"
#endif

extern FDCAN_HandleTypeDef hfdcan1; // control
extern FDCAN_HandleTypeDef hfdcan2; // sensor
extern FDCAN_HandleTypeDef hfdcan3; // critical

common::Clock::time_point last_send_time;
uint32_t num_sent;

FDCAN_HandleTypeDef* CANTypeDef_From_BusT(CANlib_Bus_T bus) {
  switch(bus) {
    case charger:
      return nullptr;
    case control:
      return &hfdcan1;
    case critical:
      return &hfdcan3;
    case sensor:
      return &hfdcan2;
  }
  return nullptr;
}

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
// #ifdef USING_LOGGING_CALLBACK
//   log_frame(frame, bus_num);
// #else
//   UNUSED(bus_num);
// #endif
  // SLO_LOG_INFO("sending");
  // frame->data[0] = 0x1;
  // frame->data[1] = 0x2;
  // frame->data[2] = 0x3;
  // frame->data[3] = 0x4;
  // frame->data[4] = 0x5;
  // frame->data[5] = 0x6;
  // frame->data[6] = 0x7;
  // frame->data[7] = 0x8;
  // SLO_LOG_DEBUG("%d %d %d %d %d %d %d %d", frame->data[0], frame->data[1], frame->data[2], frame->data[3], frame->data[4], frame->data[5], frame->data[6], frame->data[7]);
  HAL_StatusTypeDef res = HAL_FDCAN_AddMessageToTxFifoQ(hcan, &pHeader, frame->data);
  common::Clock::time_point now = common::Clock::now();
  num_sent++;
  if(now - last_send_time > std::chrono::milliseconds(1000)) {
    LOG_INFO("sent %lu messages", num_sent);
    last_send_time = now;
    num_sent = 0;
  }
  if(res != HAL_OK) {
    LOG_INFO("%d", hcan == &hfdcan1);
    LOG_INFO("%d", res);
    LOG_INFO(" err %lu", hcan->ErrorCode);
  }
  return res;
}

void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) {
  UNUSED(bus);
  FDCAN_HandleTypeDef *hcan = CANTypeDef_From_BusT(bus);

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