#include "static.h"

#include "stm32f2xx_hal_can.h"
#include <stdint.h>

CAN_HandleTypeDef CanHandle;

HAL_StatusTypeDef CANlib_Init(uint32_t baudrate) {
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


  HAL_StatusTypeDef status;

  if ((status = HAL_CAN_Init(&CanHandle)) != HAL_OK) {
    /* Initialization Error */

    printf("[CAN INIT] ERROR\r\n");
    return status;
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

  if ((status = HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig)) != HAL_OK) {
    return status;
  }

  if ((status = HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0)) != HAL_OK) {
    return status;
  }

  return HAL_OK;
}

HAL_StatusTypeDef CANlib_TransmitFrame(Frame *frame) {
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

  __disable_irq();
  HAL_StatusTypeDef CAN_TX_STATUS = HAL_CAN_Transmit_IT(&CanHandle);
  __enable_irq();

  if (CAN_TX_STATUS != HAL_OK) {
    // TODO handle error
    printf("[CAN TX] ERROR: HAL_StatusTypeDef is %d\r\n",    (int) CAN_TX_STATUS);
    printf("[CAN TX] ERROR: HAL_CAN_StateTypeDef is %d\r\n", CanHandle.State);
    printf("[CAN TX] ERROR: ErrorCode is %d\r\n",            CanHandle.ErrorCode);

    return CAN_TX_STATUS;
  }

  // ~HACK~
  HAL_CAN_Receive_IT(&CanHandle, CAN_FIFO0);

  return HAL_OK;
}
