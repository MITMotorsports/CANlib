#pragma once

#include <stdbool.h>
#include <stdint.h>

struct Frame {
  uint32_t id;
  uint8_t data[8];
  uint8_t dlc;
  bool extended;
};
