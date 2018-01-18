#include "can_raw.h"

#include <stdint.h>

#include "chip.h"
#include "can.h"
#include "ccand_11xx.h"

void Can_Init(uint32_t baudrate) {
  CAN_Init(baudrate);
}

void Can_SetFilter(uint32_t mask, uint32_t match_id) {
  CAN_SetMask1(mask, match_id);
  CAN_SetMask2(mask, match_id);
}

Can_ErrorID_T Can_RawWrite(Frame *frame) {
  const uint32_t can_out_id = (uint32_t) (frame->id);
  const uint8_t can_out_bytes = frame->len;

  // TODO actually convert this later, for now just hackily cast it
  Can_ErrorID_T err =  (Can_ErrorID_T) CAN_Transmit(can_out_id, frame->data, can_out_bytes);
  return err;
}

Can_ErrorID_T Can_RawRead(Frame *frame) {
  CCAN_MSG_OBJ_T rx_msg;

  // TODO actually convert this later, for now just hackily cast it
  Can_ErrorID_T err = (Can_ErrorID_T) CAN_Receive(&rx_msg);

  if (err == Can_Error_NONE) {
    frame->id = rx_msg.mode_id;
    frame->len = rx_msg.dlc;

    uint8_t i;
    for (i = 0; i < frame->len; i++) {
      frame->data[i] = rx_msg.data[i];
    }
  }
  return err;
}
