#pragma once

#include <stdint.h>
#include <chrono>

namespace CANlib {

// a generic clock interface that when implemented should statisfy TrivialClock requirements
// implementation should be provided in a source file
// https://en.cppreference.com/w/cpp/named_req/TrivialClock
struct Clock {
  typedef int32_t rep;
  typedef std::milli period;
  typedef std::chrono::duration<rep, period> duration;
  typedef std::chrono::time_point<Clock> time_point;

  static time_point now() noexcept;

  static constexpr bool is_steady = true;
};
 
}  // namespace CANlib
