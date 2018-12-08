#ifndef _CAN_LIBRARY_CAN_RAW_H
#define _CAN_LIBRARY_CAN_RAW_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  uint32_t id;
  uint8_t data[8];
  uint8_t dlc;
  bool extended;
} Frame;

#endif // _CAN_LIBRARY_CAN_RAW_H
