#include "static.h"

#include "drivers/inc/stm32h7xx.h"
#include "stm32h7xx_hal.h"
#include "bus.h"
#include "driver.h"
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#ifdef USING_LOGGING_CALLBACK
  #include "log.h"
#endif

extern FDCAN_HandleTypeDef hfdcan1;
// TODO: do some macro crap to automatically deal with these if not used
extern FDCAN_HandleTypeDef hfdcan2;
extern FDCAN_HandleTypeDef hfdcan3;

// these are needed because STM decided that making FDCAN_DLC_BYTES_3 actually be 3 made too much sense
static bool size_to_FDCAN_def(uint8_t size, uint32_t *def);
static bool FDCAN_def_to_size(uint32_t def, uint8_t *size);

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
  pHeader.Identifier= frame->id;
  pHeader.IdType = frame->extended ? FDCAN_EXTENDED_ID: FDCAN_STANDARD_ID;
  pHeader.FDFormat =  FDCAN_CLASSIC_CAN;
  pHeader.TxFrameType = FDCAN_DATA_FRAME;
  // TODO: figure out if we need to set the MessageMarker field (no idea what that does)
  if (!size_to_FDCAN_def(frame->dlc, &pHeader.DataLength)) {
    return HAL_ERROR;
  }

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

  // There are 2 receive FIFOs
  // note that the macros are not equal to 0 and 1
  uint8_t data[8] = {};
  FDCAN_RxHeaderTypeDef pHeader;
  uint32_t fifos[] = {FDCAN_RX_FIFO0, FDCAN_RX_FIFO1};
  for (unsigned int i = 0; i < sizeof(fifos) / sizeof(fifos[0]); i++) {
    uint32_t fifo = fifos[i];
    if (HAL_FDCAN_GetRxFifoFillLevel(hfdcan, fifo) > 0) {
      HAL_FDCAN_GetRxMessage(hfdcan, fifo, &pHeader, data);
      frame->id = pHeader.IdType == FDCAN_STANDARD_ID ? pHeader.Identifier: pHeader.Identifier;
      FDCAN_def_to_size(pHeader.DataLength, &(frame->dlc));

      memcpy(frame->data, data, sizeof(data));
      frame->extended = pHeader.IdType == FDCAN_EXTENDED_ID;
      return;
    }
  }
}

Time_T CANlib_GetTick(void) {
  return HAL_GetTick();
}

static bool size_to_FDCAN_def(uint8_t size, uint32_t *def) {
  switch (size) {
    case 0:
      *def = FDCAN_DLC_BYTES_0;
      break;
    case 1:
      *def = FDCAN_DLC_BYTES_1;
      break;
    case 2:
      *def = FDCAN_DLC_BYTES_2;
      break;
    case 3:
      *def = FDCAN_DLC_BYTES_3;
      break;
    case 4:
      *def = FDCAN_DLC_BYTES_4;
      break;
    case 5:
      *def = FDCAN_DLC_BYTES_5;
      break;
    case 6:
      *def = FDCAN_DLC_BYTES_6;
      break;
    case 7:
      *def = FDCAN_DLC_BYTES_7;
      break;
    case 8:
      *def = FDCAN_DLC_BYTES_8;
      break;
    case 12:
      *def = FDCAN_DLC_BYTES_12;
      break;
    case 16:
      *def = FDCAN_DLC_BYTES_16;
      break;
    case 20:
      *def = FDCAN_DLC_BYTES_20;
      break;
    case 24:
      *def = FDCAN_DLC_BYTES_24;
      break;
    case 32:
      *def = FDCAN_DLC_BYTES_32;
      break;
    case 48:
      *def = FDCAN_DLC_BYTES_48;
      break;
    case 64:
      *def = FDCAN_DLC_BYTES_64;
      break;
    default:
      return false;
      break;
  }
  return true;
}

static bool FDCAN_def_to_size(uint32_t def, uint8_t *size) {
  switch (def) {
    case FDCAN_DLC_BYTES_0:
      *size = 0;
      break;
    case FDCAN_DLC_BYTES_1:
      *size = 1;
      break;
    case FDCAN_DLC_BYTES_2:
      *size = 2;
      break;
    case FDCAN_DLC_BYTES_3:
      *size = 3;
      break;
    case FDCAN_DLC_BYTES_4:
      *size = 4;
      break;
    case FDCAN_DLC_BYTES_5:
      *size = 5;
      break;
    case FDCAN_DLC_BYTES_6:
      *size = 6;
      break;
    case FDCAN_DLC_BYTES_7:
      *size = 7;
      break;
    case FDCAN_DLC_BYTES_8:
      *size = 8;
      break;
    case FDCAN_DLC_BYTES_12:
      *size = 12;
      break;
    case FDCAN_DLC_BYTES_16:
      *size = 16;
      break;
    case FDCAN_DLC_BYTES_20:
      *size = 20;
      break;
    case FDCAN_DLC_BYTES_24:
      *size = 24;
      break;
    case FDCAN_DLC_BYTES_32:
      *size = 32;
      break;
    case FDCAN_DLC_BYTES_48:
      *size = 48;
      break;
    case FDCAN_DLC_BYTES_64:
      *size = 64;
      break;
    default:
      return false;
      break;
  }
  return true;
}
