#include "can_raw.h"

#include "stm32f2xx_hal_can.h"
#include <stdint.h>

static CAN_HandleTypeDef CanHandle;

void Can_Init(uint32_t baudrate) {

	// TODO calculate baudrate and set stuff accordingly
    // Right now the baudrate is hard-coded to 500kbaud
    // Can change prescaler to change this
    // 2 Mbaud / prescaler = baudrate (prescaler goes from 1 to 1024)

	CAN_FilterConfTypeDef  sFilterConfig;
    static CanTxMsgTypeDef        TxMessage;
    static CanRxMsgTypeDef        RxMessage;

    CanHandle.Instance = CAN1;
    CanHandle.pTxMsg = &TxMessage;
    CanHandle.pRxMsg = &RxMessage;

    CanHandle.Init.TTCM = DISABLE;
    CanHandle.Init.ABOM = DISABLE;
    CanHandle.Init.AWUM = DISABLE;
    CanHandle.Init.NART = DISABLE;
    CanHandle.Init.RFLM = DISABLE;
    CanHandle.Init.TXFP = DISABLE;
    CanHandle.Init.Mode = CAN_MODE_NORMAL;
    CanHandle.Init.SJW = CAN_SJW_1TQ;
    CanHandle.Init.BS1 = CAN_BS1_6TQ;
    CanHandle.Init.BS2 = CAN_BS2_8TQ;
    CanHandle.Init.Prescaler = 4;

    if(HAL_CAN_Init(&CanHandle) != HAL_OK)
    {
    	/* Initialization Error */
    	// Error_Handler();
    } 


    // TODO: Might want to comment this out
    sFilterConfig.FilterNumber = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.BankNumber = 14;

    if(HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
    {
	    /* Filter configuration Error */
	    // Error_Handler();
    }
}

Can_ErrorID_T Can_RawWrite(Frame *frame) {

	CanHandle.pTxMsg->StdId = frame->id;		// MSG ID
	CanHandle.pTxMsg->IDE = CAN_ID_STD;			// *STANDARD* OR EXTENDED CAN?
	CanHandle.pTxMsg->RTR = CAN_RTR_DATA;		// *DATA* OR REMOTE FRAME?
	CanHandle.pTxMsg->DLC = frame->len;			// LENGTH OF DATA
	// CanHandle.pTxMsg->Data = frame->data;		// DATA ITSELF uint8[8]
	uint8_t i;
	for (i=0; i < 8; i++) {
		CanHandle.pTxMsg->Data[i] = frame->data[i];
	}

	if (HAL_CAN_Transmit(&CanHandle, 10) != HAL_OK) {
		// TODO handle error
		return Can_Error_UNRECOGNIZED_ERROR;
	} else {
		return Can_Error_NONE;
	}
}

Can_ErrorID_T Can_RawRead(Frame *frame) {

	if (HAL_CAN_Receive(&CanHandle, CAN_FIFO0, 10) != HAL_OK) {
		// TODO handle error
		return Can_Error_UNRECOGNIZED_ERROR;
	} 

	uint32_t recvMsgLen = CanHandle.pRxMsg->DLC;
	if (recvMsgLen < 1 || CanHandle.pRxMsg->IDE == CAN_ID_EXT) {
		return Can_Error_UNRECOGNIZED_ERROR;	
	}

	frame->id = CanHandle.pRxMsg->StdId;
	frame->len = CanHandle.pRxMsg->DLC;
	// frame->data = CanHandle.pRxMsg->Data;

	uint8_t i;
	for (i=0; i < 8; i++) {
		frame->data[i] = CanHandle.pRxMsg->Data[i];
	}

	return Can_Error_NONE;
}


