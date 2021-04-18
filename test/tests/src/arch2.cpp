#include <cassert>
#include "canlib_testcomp2.hpp"
#include "test.hpp"

namespace CANlib {
namespace map1 {
extern A_T A_input;
}  // namespace map1
namespace map2 {
extern F_T F_input;
}  // namespace map2
}  // namespace CANlib

using namespace CANlib;
using namespace map1;
using namespace map2;

extern CAN can1;
extern CAN can2;
extern CAN can3;

void testArch() {
  assert(GetRawBus(Bus_Names_T::map1) == Raw_Bus_T::CAN_3);
  assert(GetRawBus(Bus_Names_T::map2) == Raw_Bus_T::CAN_1);
  assert(GetRawBus(Bus_Names_T::INVALID_NAME) == Raw_Bus_T::INVALID_BUS);
  assert(GetBusName(Raw_Bus_T::CAN_3) == Bus_Names_T::map1);
  assert(GetBusName(Raw_Bus_T::CAN_1) == Bus_Names_T::map2);
  assert(GetBusName(Raw_Bus_T::CAN_2) == Bus_Names_T::INVALID_NAME);
  assert(GetBusName(Raw_Bus_T::INVALID_BUS) == Bus_Names_T::INVALID_NAME);
  can1.clear();
  can2.clear();
  can3.clear();
  send(&A_input);
  send(&F_input);
  assert(1 == can1.framesReceived());
  assert(0 == can2.framesReceived());
  assert(1 == can3.framesReceived());
  can1.clear();
  can2.clear();
  can3.clear();
  Frame f;
  map1::A_T A_copy;
  A_copy.ARG0 = true;
  A_copy.ARG1 = 69;
  A_copy.ARG2 = 69;
  A_copy.ARG3 = 69;
  A_copy.unpack(f);
  can3.setFrameToSend(f);
  map1::A_T A_copy2;
  map1_update_can();
  assert(A_input.ARG0 && A_input.ARG1 == 69 && A_input.ARG2 == 69 && A_input.ARG3 == 69);
  map2::F_T F_copy;
  F_copy.ARG0 = 69;
  F_copy.ARG1 = 69;
  F_copy.ARG2 = 69;
  F_copy.ARG3 = 69;
  F_copy.ARG4 = 69;
  F_copy.ARG5 = 69;
  F_copy.unpack(f);
  can1.setFrameToSend(f);
  map2_update_can();
  assert(F_input.ARG0 == 69 && F_input.ARG1 == 69 && F_input.ARG2 == 69 && F_input.ARG3 == 69 &&
         F_input.ARG4 == 69 && F_input.ARG5 == 69);
}
