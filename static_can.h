#ifndef _MY18_STATIC_CAN_H
#define _MY18_STATIC_CAN_H

#include "can_raw.h"
#include <stdint.h>
#include <stdbool.h>

Can_MsgID_T Can_MsgType(void);

#define TO_CAN(name) \
  void name ## _ToCan(name ## _T *type_in, Frame *can_out)

#define FROM_CAN(name) \
  void name ## _FromCan(Frame *can_in, name ## _T *type_out)

#define DECLARE(name) \
  Can_ErrorID_T name ##_Read(name ## _T *type); \
  Can_ErrorID_T name ##_Write(name ## _T *type); \
  TO_CAN(name); \
  FROM_CAN(name);

Can_ErrorID_T Can_Unknown_Read(Frame *frame);
Can_ErrorID_T Can_Error_Read(void);

void to_bitstring(uint8_t in[], uint64_t *out);
void from_bitstring(uint64_t *in, uint8_t out[]);

#endif // _MY18_STATIC_CAN_H
