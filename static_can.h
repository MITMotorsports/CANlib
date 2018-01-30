#ifndef _CAN_LIBRARY_STATIC_CAN_H
#define _CAN_LIBRARY_STATIC_CAN_H

#include "can_raw.h"
#include <stdint.h>
#include <stdbool.h>

#define CAN_PACK(name) \
  void name ## _pack(name ## _T *type_in, Frame *can_out)

#define CAN_UNPACK(name) \
  void name ## _unpack(Frame *can_in, name ## _T *type_out)

#define DECLARE(name) \
  Can_ErrorID_T name ##_Write(name ## _T *type); \
  CAN_PACK(name); \
  CAN_UNPACK(name);

Can_ErrorID_T Can_Unknown_Read(Frame *frame);
Can_ErrorID_T Can_Error_Read(void);

void to_bitstring(uint8_t in[], uint64_t *out);
void from_bitstring(uint64_t *in, uint8_t out[]);

#endif // _CAN_LIBRARY_STATIC_CAN_H
