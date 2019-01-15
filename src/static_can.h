#ifndef _CAN_LIBRARY_STATIC_CAN_H
#define _CAN_LIBRARY_STATIC_CAN_H

#include "can_raw.h"
#include "driver.h"
#include <stdint.h>
#include <stdbool.h>

#define CAN_PACK(name) \
  void CANlib_Pack_ ## name(CANlib_ ## name ## _T *type_in, Frame *can_out)

#define CAN_UNPACK(name) \
  void CANlib_Unpack_ ## name(Frame *can_in, CANlib_ ## name ## _T *type_out)

#define DECLARE(name) \
  CANlib_Transmit_Error_T CANlib_Transmit_ ## name(CANlib_ ## name ## _T *type); \
  CAN_PACK(name); \
  CAN_UNPACK(name);

#define DEFINE(name) \
  CANlib_Transmit_Error_T CANlib_Transmit_ ## name(CANlib_ ## name ## _T *type) { \
    Frame frame; \
    CANlib_Pack_ ## name(type, &frame); \
    return CANlib_TransmitFrame(&frame); \
  }

#define LIMIT(name)                                         \
  static Time_T last_sent = 0;                              \
  if (HAL_GetTick() - last_sent < name ## _period) return;  \
  last_sent = HAL_GetTick();

void to_bitstring(uint8_t in[], uint64_t *out);
void from_bitstring(uint64_t *in, uint8_t out[]);

#endif // _CAN_LIBRARY_STATIC_CAN_H
