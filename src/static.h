#ifndef _CAN_LIB_STATIC_H
#define _CAN_LIB_STATIC_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint32_t id;
  uint8_t data[8];
  uint8_t dlc;
  bool extended;
} Frame;

#define LIMIT(name)                                         \
  static Time_T last_sent = 0;                              \
  if (HAL_GetTick() - last_sent < name ## _period) return;  \
  last_sent = HAL_GetTick();

#endif // _CAN_LIB_STATIC_H
