#ifndef _MY18_CAN_LIBRARY_CAN_RAW_H
#define _MY18_CAN_LIBRARY_CAN_RAW_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint32_t id;
  uint8_t len;
  uint8_t data[8];
} Frame;

typedef enum {
  Can_Error_NONE,
  Can_Error_NO_RX,
  Can_Error_EPASS,
  Can_Error_WARN,
  Can_Error_BOFF,
  Can_Error_STUF,
  Can_Error_FORM,
  Can_Error_ACK,
  Can_Error_BIT1,
  Can_Error_BIT0,
  Can_Error_CRC,
  Can_Error_UNUSED,
  Can_Error_UNRECOGNIZED_MSGOBJ,
  Can_Error_UNRECOGNIZED_ERROR,
  Can_Error_TX_BUFFER_FULL,
  Can_Error_RX_BUFFER_FULL,
} Can_ErrorID_T;

void Can_Init(uint32_t baudrate);
void Can_SetFilter(uint32_t mask, uint32_t match_id);
Can_ErrorID_T Can_RawWrite(Frame *frame);
Can_ErrorID_T Can_RawRead(Frame *frame);

#endif // _MY18_CAN_LIBRARY_CAN_RAW_H
