#include "can_raw.h"

#include "stm32f2xx_hal_can.h"
#include <stdint.h>

CAN_HandleTypeDef CanHandle;

void Can_Init(uint32_t baudrate) {
  // TODO calculate baudrate and set stuff accordingly
  // Right now the baudrate is hard-coded to 500kbaud
  // Can change prescaler to change this
  // 2 Mbaud / prescaler = baudrate (prescaler goes from 1 to 1024)

  CAN_FilterConfTypeDef  sFilterConfig;
  static CanTxMsgTypeDef TxMessage;
  static CanRxMsgTypeDef RxMessage;

  CanHandle.Instance = CAN1;
  CanHandle.pTxMsg   = &TxMessage;
  CanHandle.pRxMsg   = &RxMessage;

  CanHandle.Init.TTCM      = DISABLE;
  CanHandle.Init.ABOM      = DISABLE;
  CanHandle.Init.AWUM      = DISABLE;
  CanHandle.Init.NART      = DISABLE;
  CanHandle.Init.RFLM      = DISABLE;
  CanHandle.Init.TXFP      = DISABLE;
  CanHandle.Init.Mode      = CAN_MODE_NORMAL;
  CanHandle.Init.SJW       = CAN_SJW_1TQ;
  CanHandle.Init.BS1       = CAN_BS1_6TQ;
  CanHandle.Init.BS2       = CAN_BS2_8TQ;
  CanHandle.Init.Prescaler = 4;

  if (HAL_CAN_Init(&CanHandle) != HAL_OK)
  {
    /* Initialization Error */

    // Error_Handler();
  }


  // TODO: Might want to comment this out
  sFilterConfig.FilterNumber         = 0;
  sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh         = 0x0000;
  sFilterConfig.FilterIdLow          = 0x0000;
  sFilterConfig.FilterMaskIdHigh     = 0x0000;
  sFilterConfig.FilterMaskIdLow      = 0x0000;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation     = ENABLE;
  sFilterConfig.BankNumber           = 14;

  if (HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */

    // Error_Handler();
  }

  if (HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0) != HAL_OK)
  {
    // Error_Handler();
  }
}

Can_ErrorID_T Can_RawWrite(Frame *frame) {
  if (frame->extended) {
    CanHandle.pTxMsg->ExtId = frame->id;
    CanHandle.pTxMsg->IDE   = CAN_ID_EXT;
  } else {
    CanHandle.pTxMsg->StdId = frame->id;
    CanHandle.pTxMsg->IDE   = CAN_ID_STD;
  }

  CanHandle.pTxMsg->DLC = frame->len;
  CanHandle.pTxMsg->RTR = CAN_RTR_DATA;

  memcpy(CanHandle.pTxMsg->Data, frame->data, sizeof(frame->data));

  if (HAL_CAN_Transmit_IT(&CanHandle) != HAL_OK) {
    // TODO handle error
    return Can_Error_UNRECOGNIZED_ERROR;
  }

  return Can_Error_NONE;
}

void lastRxMsgToFrame(Frame *frame) {
  if (CanHandle.pRxMsg->RTR == CAN_RTR_DATA) {
    if (CanHandle.pRxMsg->IDE == CAN_ID_STD) {
      frame->id       = CanHandle.pRxMsg->StdId;
      frame->extended = 0;
    } else if (CanHandle.pRxMsg->IDE == CAN_ID_EXT) {
      frame->id       = CanHandle.pRxMsg->ExtId;
      frame->extended = 1;
    }

    frame->len = CanHandle.pRxMsg->DLC;

    memcpy(frame->data, CanHandle.pRxMsg->Data, sizeof(frame->data));
  }
}

Can_ErrorID_T Can_RawRead(Frame *frame) {
  if (HAL_CAN_Receive(&CanHandle, CAN_FIFO0, 10) != HAL_OK) {
    // TODO handle error
    return Can_Error_UNRECOGNIZED_ERROR;
  }

  lastRxMsgToFrame(frame);

  return Can_Error_NONE;
}
