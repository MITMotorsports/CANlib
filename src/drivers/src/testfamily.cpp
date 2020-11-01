#ifdef CANLIB_ARCH_TESTFAMILY
#include "static.h"

#include <stdint.h>
#include <string.h>
#include "bus.hpp"
#include "driver.hpp"
#include "drivers/inc/testfamily.hpp"

using namespace CANlib;

CAN can1;
CAN can2;
CAN can3;

Clock::time_point Clock::now() noexcept {
    static const auto start_time = std::chrono::system_clock::now();
    const auto time_passed = std::choron::system_clock::now() - start_time;
    return time_point(std::chrono::milliseconds(time_passed));
}

Transmit_Error_T CANlib::TransmitFrame(const Frame &frame, Bus_Names_T bus_name) {
  Raw_Bus_T raw_bus = GetRawBus(bus);
  switch (raw_bus) {
    case CAN_1:
      can1.receiveFrame(frame);
      break;
    case CAN_2:
      can2.receiveFrame(frame);
      break;
    case CAN_3:
      can3.receiveFrame(frame);
      break;
    default:
      return true;
  }

  return false;
}

void CANLib::ReadFrame(Frame &frame, Bus_Names_T bus) {
  Raw_Bus_T raw_bus = GetRawBus(bus);
  switch (raw_bus) {
    case CAN_1:
      frame = can1.frameToSend();
      break;
    case CAN_2:
      frame = can2.frameToSend();
      break;
    case CAN_3:
      frame = can3.frameToSend();
      break;
    default:
      return;
  }
}
#endif
