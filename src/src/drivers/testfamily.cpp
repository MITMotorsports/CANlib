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

TransmitError CANlib::transmit_frame(const Frame &frame, AbstractBus bus_name) {
  switch (get_raw_bus(bus_name)) {
    case RawBus::CAN_1:
      can1.receiveFrame(frame);
      break;
    case RawBus::CAN_2:
      can2.receiveFrame(frame);
      break;
    case RawBus::CAN_3:
      can3.receiveFrame(frame);
      break;
    default:
      return true;
  }

  return false;
}

void CANlib::read_frame(Frame &frame, AbstractBus bus_name) {
  switch (get_raw_bus(bus_name)) {
    case RawBus::CAN_1:
      frame = can1.frameToSend();
      break;
    case RawBus::CAN_2:
      frame = can2.frameToSend();
      break;
    case RawBus::CAN_3:
      frame = can3.frameToSend();
      break;
    default:
      return;
  }
}
