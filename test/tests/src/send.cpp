#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include "pack_unpack.hpp"
#include "test.hpp"

using namespace CANlib;
using namespace map1;
using namespace map2;
using namespace J;
using namespace CC;
using namespace std::chrono_literals;

extern TestCAN can1;
extern TestCAN can2;
extern TestCAN can3;

#define CREATE_TEST(Name, can, BITMASK)                                        \
  static void testSend##Name() {                                               \
    Name Name##_copy;                                                          \
    Frame f0;                                                                  \
    for (int i = 0; i < 8; ++i) {                                              \
      f0.data[i] = distribution(generator);                                    \
    }                                                                          \
    uint64_t bitstring0;                                                       \
    to_bitstring((uint8_t *)f0.data, &bitstring0);                             \
    bitstring0 &= BITMASK;                                                     \
    Name##_copy.pack(f0);                                                      \
    can.clear();                                                               \
    if (Name##_copy.get_period() != 0ms) {                                     \
      auto starting_time = Clock::now();                                       \
      auto tmp           = starting_time;                                      \
      while (Clock::now() - starting_time <= Name##_copy.get_period() + 5ms) { \
        send_period(tmp, &Name##_copy);                                        \
      }                                                                        \
    } else {                                                                   \
      send(&Name##_copy);                                                      \
    }                                                                          \
    uint64_t bitstring1;                                                       \
    assert(can.framesReceived() == 1);                                         \
    Frame f1 = can.topFrame();                                                 \
    to_bitstring((uint8_t *)f1.data, &bitstring1);                             \
    bitstring1 &= BITMASK;                                                     \
    assert(bitstring0 == bitstring1);                                          \
  }

CREATE_TEST(A, MAP1_CAN, (~ZEROES_MASK(7, 49)))
CREATE_TEST(B, MAP1_CAN, (~ZEROES_MASK(5, 59)))
CREATE_TEST(C, MAP1_CAN, (~ZEROES_MASK(0, 56)))
CREATE_TEST(D,
            MAP1_CAN,
            (~ZEROES_MASK(0, 1)) | (~ZEROES_MASK(2, 1)) | (~ZEROES_MASK(3, 1)) |
                (~ZEROES_MASK(4, 1)) | (~ZEROES_MASK(5, 1)) | (~ZEROES_MASK(6, 1)) |
                (~ZEROES_MASK(7, 1)) | (~ZEROES_MASK(9, 1)) | (~ZEROES_MASK(16, 1)) |
                (~ZEROES_MASK(17, 1)))
CREATE_TEST(E, MAP1_CAN, (~ZEROES_MASK(0, 64)))
CREATE_TEST(F,
            MAP2_CAN,
            (~ZEROES_MASK(0, 10)) | (~ZEROES_MASK(10, 8)) | (~ZEROES_MASK(18, 10)) |
                (~ZEROES_MASK(28, 8)) | (~ZEROES_MASK(36, 10)) | (~ZEROES_MASK(46, 8)) |
                (~ZEROES_MASK(54, 10)))
CREATE_TEST(G, MAP2_CAN, (~ZEROES_MASK(0, 60)))
CREATE_TEST(H, MAP2_CAN, (~ZEROES_MASK(0, 64)))
CREATE_TEST(I, MAP2_CAN, (~ZEROES_MASK(0, 64)))
CREATE_TEST(AA, MAP2_CAN, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(39, 1)))
CREATE_TEST(BB, MAP2_CAN, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(39, 1)))
CREATE_TEST(AAA, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(BBB, MAP2_CAN, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(32, 16)))
CREATE_TEST(CCC, MAP2_CAN, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(32, 16)))
CREATE_TEST(DDD, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(DD, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(EE, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(FF, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(GG, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(HH, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(II, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(JJ, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(KK, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(LL, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(MM, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(39, 1))))
CREATE_TEST(NN, MAP2_CAN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(39, 1))))
CREATE_TEST(K, MAP2_CAN, (~ZEROES_MASK(0, 32)))
CREATE_TEST(L, MAP2_CAN, (~ZEROES_MASK(0, 25)))
CREATE_TEST(M, MAP2_CAN, (~ZEROES_MASK(0, 16)))
CREATE_TEST(N,
            MAP2_CAN,
            ((~ZEROES_MASK(3, 1)) | (~ZEROES_MASK(4, 3)) | (~ZEROES_MASK(8, 1)) |
             (~ZEROES_MASK(9, 1)) | (~ZEROES_MASK(10, 1))))

void testSend() {
  for (int cs = 0; cs < 10; cs++) {
    testSendA();
    testSendB();
    testSendC();
    testSendD();
    testSendE();
    testSendF();
    testSendG();
    testSendH();
    testSendI();
    testSendAA();
    testSendBB();
    testSendAAA();
    testSendBBB();
    testSendCCC();
    testSendDDD();
    testSendDD();
    testSendEE();
    testSendFF();
    testSendGG();
    testSendHH();
    testSendII();
    testSendJJ();
    testSendKK();
    testSendLL();
    testSendMM();
    testSendNN();
    testSendK();
    testSendL();
    testSendM();
    testSendN();
  }
}
