//#ifdef CANLIB_ARCH_TESTFAMILY
#include "static.hpp"

#include <stdint.h>
#include <string.h>
#include <iostream>
#include "bus.hpp"
#include "driver.hpp"
#include "drivers/testfamily.hpp"

using namespace CANlib;

CAN can1;
CAN can2;
CAN can3;

Clock::time_point Clock::now() noexcept {
  static const auto start_time = std::chrono::system_clock::now();
  const auto time_passed       = std::chrono::system_clock::now() - start_time;
  std::chrono::milliseconds us = std::chrono::duration_cast<std::chrono::milliseconds>(time_passed);
  return time_point(std::chrono::milliseconds(us.count()));
}

Transmit_Error_T CANlib::TransmitFrame(const Frame &frame, Bus_Names_T bus_name) {
  switch (GetRawBus(bus_name)) {
    case Raw_Bus_T::CAN_1:
      can1.receiveFrame(frame);
      break;
    case Raw_Bus_T::CAN_2:
      can2.receiveFrame(frame);
      break;
    case Raw_Bus_T::CAN_3:
      can3.receiveFrame(frame);
      break;
    default:
      return true;
  }

  return false;
}

void CANlib::ReadFrame(Frame &frame, Bus_Names_T bus_name) {
  switch (GetRawBus(bus_name)) {
    case Raw_Bus_T::CAN_1:
      frame = can1.frameToSend();
      break;
    case Raw_Bus_T::CAN_2:
      frame = can2.frameToSend();
      break;
    case Raw_Bus_T::CAN_3:
      frame = can3.frameToSend();
      break;
    default:
      return;
  }
}
//#endif
