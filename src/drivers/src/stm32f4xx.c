#include "can_raw.h"

#include "stm32f4xx_hal.h"
#include "bus.h"
#include <stdint.h>

static CAN_HandleTypeDef can1;
static CAN_HandleTypeDef can2;
static CAN_HandleTypeDef can3;

static void config_handle_parameters(CAN_HandleTypeDef *hcan) {
  // Universal configuration parameters
  hcan->Init.Prescaler = 2;
  hcan->Init.Mode = CAN_MODE_NORMAL;
  hcan->Init.SyncJumpWidth = CAN_SJW_3TQ;
  hcan->Init.TimeSeg1 = CAN_BS1_16TQ;
  hcan->Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan->Init.TimeTriggeredMode = DISABLE;
  hcan->Init.AutoBusOff = DISABLE;
  hcan->Init.AutoWakeUp = DISABLE;
  hcan->Init.AutoRetransmission = DISABLE;
  hcan->Init.ReceiveFifoLocked = DISABLE;
  hcan->Init.TransmitFifoPriority = DISABLE;
}

HAL_StatusTypeDef CANlib_Init(uint32_t baudrate) {
  // TODO the baudrate argument should probably be removed...
  // ... this is hardcoded to 1Mbaud
  // TODO this right now just supports using one bus
  CAN_FilterTypeDef filter = {0};
  filter.FilterIdHigh = 0x1fff;
  filter.FilterIdLow = 0xffff;
  filter.FilterMaskIdHigh = 0;
  filter.FilterMaskIdLow = 0;
  filter.FilterFIFOAssignment = CAN_RX_FIFO0;
  filter.FilterBank = 0;
  filter.FilterMode = CAN_FILTERMODE_IDMASK; // Standard CAN filtering
  filter.FilterScale = CAN_FILTERSCALE_16BIT;
  filter.FilterActivation = ENABLE;
  filter.SlaveStartFilterBank = 0;

  #if defined(USING_CAN_1) || defined(USING_CAN_2)
  // We need to initialize CAN1 regardless of wheter we are using CAN1 or CAN2
  can1.Instance = CAN1;
  config_handle_parameters(&can1);
  HAL_CAN_Init(&can1);
  HAL_CAN_ConfigFilter(&can1, &filter);
  HAL_CAN_Start(&can1);
  #endif
  #ifdef USING_CAN_2
  can2.Instance = CAN2;
  config_handle_parameters(&can2);
  HAL_CAN_Init(&can2);
  HAL_CAN_ConfigFilter(&can2, &filter);
  HAL_CAN_Start(&can2);
  #elif defined(USING_CAN_3)
  can3.Instance = CAN3;
  config_handle_parameters(&can3);
  HAL_CAN_Init(&can3);
  HAL_CAN_ConfigFilter(&can3, &filter);
  HAL_CAN_Start(&can3);
  #endif
}

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

HAL_StatusTypeDef CANlib_ReadFrame(Frame *frame, CANlib_Bus_T bus) {
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


