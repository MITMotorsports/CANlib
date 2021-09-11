#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <cstring>

struct Frame {
  uint32_t id;
  uint8_t data[8];
  uint8_t dlc;
  bool extended;
  Frame &operator=(const Frame &f) {
    id = f.id;
    for (int i = 0; i < 8; ++i) {
      data[i] = f.data[i];
    }
    dlc      = f.dlc;
    extended = f.extended;
    return *this;
  }
  Frame() {
    id = 0;
    memset(data, 0, sizeof(data));
    dlc      = 0;
    extended = false;
  }
};
