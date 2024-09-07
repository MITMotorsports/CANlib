#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint32_t id;
  uint8_t data[8];
  uint8_t dlc;
  bool extended;
} Frame;
