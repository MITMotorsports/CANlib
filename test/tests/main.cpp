#include "test.hpp"

std::default_random_engine generator;
std::uniform_int_distribution<uint16_t> distribution(0, 511);

int main() {
  testIdentify();
  testArch();
  testKeys();
  testPeriod();
  testMessageType();
  testPackUnpack();
  testSend();
  return 0;
}
