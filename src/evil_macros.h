#ifndef _CAN_LIBRARY_EVIL_MACROS_H
#define _CAN_LIBRARY_EVIL_MACROS_H

#include <stdbool.h>
#include <stdint.h>

#define TOGGLE(input, test, idx)      \
  if (test) {                         \
    ((input) |= (1 << (7 - (idx))));  \
  } else {                            \
    ((input) &= ~(1 << (7 - (idx)))); \
  }

#define CHECK(a, b) (((a) & (1 << (7 - (b)))) != 0)

#define ONES(len) ((1ULL << (len)) - 1)

#define START_IDX(start, len) (64 - (start) - (len))

#define ZEROES_MASK(start, len) (~(ONES(len) << START_IDX(start, len)))

#define INPUT_MASK(input, start, len) (((input)&ONES(len)) << START_IDX(start, len))

#define INSERT(input, output, start, len) \
  (((output) & (ZEROES_MASK(start, len))) | INPUT_MASK(input, start, len))

#define SIGN(input, data_width) \
  (((int64_t)((uint64_t)(input) << (64 - (data_width)))) >> (64 - (data_width)))

#define EXTRACT(input, start, len) (((input) >> START_IDX(start, len)) & ONES(len))

#define BIT_SET(input, bit_value, bit_idx) \
  if ((bit_value)) {                       \
    ((input) |= (1UL << (bit_idx)));       \
  } else {                                 \
    ((input) &= ~(1UL << (bit_idx)));      \
  }

#define BIT_GET(input, bit_idx) (bool)((input) & (1UL << (bit_idx)))

#endif  // _CAN_LIBRARY_EVIL_MACROS_H
