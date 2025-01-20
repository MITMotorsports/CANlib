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
#ifdef USING_LOGGING_CALLBACK
#include "log.h"
#endif


common::Clock::time_point last_send_time;
uint32_t num_sent;

uint8_t next_can_buffers[4];

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

uint32_t CANlib_TxBufferLocation_From_BufferNumber(uint8_t buffer_nbr) {
  switch(buffer_nbr) {
    case 0:
      return FDCAN_TX_BUFFER0;
    case 1:
      return FDCAN_TX_BUFFER1;
    case 2:
      return FDCAN_TX_BUFFER2;
    // case 3:
    //   return FDCAN_TX_BUFFER3;
    // case 4:
    //   return FDCAN_TX_BUFFER4;
    // case 5:
    //   return FDCAN_TX_BUFFER5;
    // case 6:
    //   return FDCAN_TX_BUFFER6;
    // case 7:
    //   return FDCAN_TX_BUFFER7;
    // case 8:
    //   return FDCAN_TX_BUFFER8;
    // case 9:
    //   return FDCAN_TX_BUFFER9;
    // case 10:
    //   return FDCAN_TX_BUFFER10;
    // case 11:
    //   return FDCAN_TX_BUFFER11;
    // case 12:
    //   return FDCAN_TX_BUFFER12;
    // case 13:
    //   return FDCAN_TX_BUFFER13;
    // case 14:
    //   return FDCAN_TX_BUFFER14;
    // case 15:
    //   return FDCAN_TX_BUFFER15;
    // case 16:
    //   return FDCAN_TX_BUFFER16;
    // case 17:
    //   return FDCAN_TX_BUFFER17;
    // case 18:
    //   return FDCAN_TX_BUFFER18;
    // case 19:
    //   return FDCAN_TX_BUFFER19;
    // case 20:
    //   return FDCAN_TX_BUFFER20;
    // case 21:
    //   return FDCAN_TX_BUFFER21;
    // case 22:
    //   return FDCAN_TX_BUFFER22;
    // case 23:
    //   return FDCAN_TX_BUFFER23;
    // case 24:
    //   return FDCAN_TX_BUFFER24;
    // case 25:
    //   return FDCAN_TX_BUFFER25;
    // case 26:
    //   return FDCAN_TX_BUFFER26;
    // case 27:
    //   return FDCAN_TX_BUFFER27;
    // case 28:
    //   return FDCAN_TX_BUFFER28;
    // case 29:
    //   return FDCAN_TX_BUFFER29;
    // case 30:
    //   return FDCAN_TX_BUFFER30;
    // case 31:
    //   return FDCAN_TX_BUFFER31;
  }
  return 0;
}

void print_can_info(FDCAN_HandleTypeDef* hcan) {
  FDCAN_ProtocolStatusTypeDef protocol_status;
  HAL_StatusTypeDef res2 = HAL_FDCAN_GetProtocolStatus(hcan, &protocol_status);

  LOG_INFO("protocol status res: %d", res2);
  LOG_INFO("LEC: %lu, DLEC: %lu, Activity: %lu, ErrorPassive: %lu, Warning: %lu, BusOff: %lu, ESIflag: %lu, BRSflag: %lu, FDFflag: %lu, ProtocolException: %lu, TDCvalue: %lu", 
    protocol_status.LastErrorCode,
    protocol_status.DataLastErrorCode,
    protocol_status.Activity,
    protocol_status.ErrorPassive,
    protocol_status.Warning,
    protocol_status.BusOff,
    protocol_status.RxESIflag,
    protocol_status.RxBRSflag,
    protocol_status.RxFDFflag, 
    protocol_status.ProtocolException,
    protocol_status.TDCvalue);

  FDCAN_ErrorCountersTypeDef error_counters;
  HAL_StatusTypeDef res3 = HAL_FDCAN_GetErrorCounters(hcan, &error_counters);
  LOG_INFO("error counter res: %d", res3);
  LOG_INFO("Tx error count: %lu, Rx error count: %lu, Rx error passive: %lu, Error logging: %lu", 
    error_counters.TxErrorCnt,
    error_counters.RxErrorCnt,
    error_counters.RxErrorPassive,
    error_counters.ErrorLogging);
}

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame, CANlib_Bus_T bus) {
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
  
  common::Clock::time_point now = common::Clock::now();
  num_sent++;
  if(now - last_send_time > std::chrono::seconds(10)) {
    LOG_INFO("CAN sent %lu messages in %lums", num_sent, (now - last_send_time).count());
    last_send_time = now;
    num_sent = 0;
  }
  // print_can_info(hcan);
  HAL_StatusTypeDef res = HAL_FDCAN_AddMessageToTxFifoQ(hcan, &pHeader, frame->data);
  if(res != HAL_OK) {
    uint32_t free_level = HAL_FDCAN_GetTxFifoFreeLevel(hcan);
    LOG_INFO("CAN TX ERROR %d, Error code %lu, free level %lu", res, hcan->ErrorCode, free_level);
  }
  return res;
}

void CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) {
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