#include "can_raw.h"

#include <stdint.h>

#include "chip.h"
#include "can.h"
#include "ccand_11xx.h"

#if SAVAGE_MODE
  #define TRANSMIT_METHOD CAN_Transmit_SAVAGE
#else
  #define TRANSMIT_METHOD CAN_Transmit
#endif

// Note: We're using one mask for actual hardware filtering and another mask for
// configuring extended messages
// This way, we can accept extended messages w/o changing the lpc library itself
void Can_Init(uint32_t baudrate) {
  CAN_Init(baudrate);
  CAN_SetMask2(0, CAN_MSGOBJ_EXT);
}

void Can_SetFilter(uint32_t mask, uint32_t match_id) {
  CAN_SetMask1(mask, match_id);
}

Can_ErrorID_T Can_RawWrite(Frame *frame) {
  uint32_t can_out_id = (uint32_t) (frame->id);
  if (can_out_id > 2047) { // i.e. is an extended message
    can_out_id |= CAN_MSGOBJ_EXT;
  }
  const uint8_t can_out_bytes = frame->len;

  // TODO actually convert this later, for now just hackily cast it
  Can_ErrorID_T err =  (Can_ErrorID_T) TRANSMIT_METHOD(can_out_id, frame->data, can_out_bytes);
  return err;
}

Can_ErrorID_T Can_RawRead(Frame *frame) {
  CCAN_MSG_OBJ_T rx_msg;

  // TODO actually convert this later, for now just hackily cast it
  Can_ErrorID_T err = (Can_ErrorID_T) CAN_Receive(&rx_msg);

  if (err == Can_Error_NONE) {
    // Take first 29 bits -- others are for config, even for extended
    frame->id = rx_msg.mode_id & 0x1fffffff;
    frame->len = rx_msg.dlc;

    uint8_t i;
    for (i = 0; i < frame->len; i++) {
      frame->data[i] = rx_msg.data[i];
    }
  }
  return err;
}
