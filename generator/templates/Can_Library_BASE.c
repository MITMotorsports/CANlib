#include "evil_macros.h"

static Frame lastMessage;
static Can_ErrorID_T lastError = Can_Error_NO_RX;

#define DEFINE(name) \
  Can_ErrorID_T name ##_Write(name ## _T *type) { \
    Frame frame; \
    name ## _pack(type, &frame); \
    return Can_RawWrite(&frame); \
  }

typedef enum {
  LITTLE,
  BIG,
} ENDIAN_T;

typedef union {
  uint8_t byte[8];
  uint64_t bitstring;
} DATA_T;

void data_transfer(DATA_T *in, DATA_T *out) {
  uint8_t i;
  for (i = 0; i < 8; i++) {
    (*out).byte[7-i] = (*in).byte[i];
  }
}

void to_bitstring(uint8_t in[], uint64_t *out) {
  data_transfer((DATA_T*)in, (DATA_T*)out);
}

void from_bitstring(uint64_t *in, uint8_t out[]) {
  data_transfer((DATA_T*)in, (DATA_T*)out);
}

// Shameless copypasta-ing from Stack Overflow for trivial endian swap.
// https://stackoverflow.com/a/2637138
uint16_t swap_uint16(uint16_t val) {
  return (val << 8) | (val >> 8 );
}

int16_t swap_int16(int16_t val) {
  return (val << 8) | ((val >> 8) & 0xFF);
}

uint32_t swap_uint32(uint32_t val) {
  val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
  return (val << 16) | (val >> 16);
}

int32_t swap_int32(int32_t val) {
  val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF );
  return (val << 16) | ((val >> 16) & 0xFFFF);
}

#ifdef CAN_ARCHITECTURE_ARM
#include "arm_src/arm_can_drivers.c"
#elif CAN_ARCHITECTURE_AVR
#include "avr_src/avr_can_drivers.c"
#elif CAN_ARCHITECTURE_TEST

void Can_Init(uint32_t baudrate) {
  // TODO test harness
}

Can_ErrorID_T Can_RawWrite(Frame *frame) {
  // TODO test harness
  return 0;
}

Can_ErrorID_T Can_RawRead(Frame *frame) {
  // TODO test harness
  return 0;
}

#else
  // Define nothing so that there is a linker error!
#endif

// TODO this is a bit of a hack...unknown reads should follow same as regular reads
// and use of Can_RawRead must be banned.
Can_ErrorID_T Can_Unknown_Read(Frame *frame) {
  if (lastError == Can_Error_NONE) {
    frame->id = lastMessage.id;
    frame->len = lastMessage.len;
    uint8_t i;
    for (i = 0; i < 8; i++) {
      frame->data[i] = lastMessage.data[i];
    }
    lastError = Can_Error_NO_RX;
    return Can_Error_NONE;
  } else {
    return lastError;
  }
}

Can_ErrorID_T Can_Error_Read(void) {
  Can_ErrorID_T cachedError = lastError;
  lastError = Can_Error_NO_RX;
  return cachedError;
}
