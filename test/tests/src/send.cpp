#include <cassert>
#include <random>
#include <chrono>
#include "pack_unpack.hpp"
#include "test.hpp"

using namespace CANlib;
using namespace map1;
using namespace map2;
using namespace J;
using namespace CC;
using namespace std::chrono_literals;

extern CAN can1;
extern CAN can2;
extern CAN can3;

#define CREATE_TEST(ID, can_idx, BITMASK) \
    static void testSend##ID() { \
         ID##_T ID##_input; \
         can##can_idx.clear(); \
         auto starting_time = Clock::now(); \
         Frame f0; \
         for (int i = 0;i < 8;++i) { \
             f0.data[i] = distribution(generator); \
         } \
         uint64_t bitstring0; \
         to_bitstring((uint8_t*)f0.data, &bitstring0); \
         bitstring0 &= BITMASK; \
         ID##_input.unpack(f0); \
         while (Clock::now() - starting_time <= ID##_input.period_ + 5ms) { \
            send(starting_time, &ID##_input); \
         } \
         uint64_t bitstring1; \
         assert(can##can_idx.framesReceived() == 1); \
         Frame f1 = can##can_idx.topFrame(); \
         to_bitstring((uint8_t*)f1.data, &bitstring1); \
         bitstring1 &= BITMASK;\
         assert(bitstring0 == bitstring1); \
    } \

CREATE_TEST(A, 2, get_bitmask(7,56))
CREATE_TEST(B, 2, get_bitmask(5,64))
CREATE_TEST(C, 2, get_bitmask(0,56))
CREATE_TEST(D, 2, get_bitmask(0,18))
CREATE_TEST(E, 2, get_bitmask(0,64))
CREATE_TEST(F, 1, get_bitmask(0,10) | get_bitmask(18, 28) | get_bitmask(36, 46) | get_bitmask(54, 64))
CREATE_TEST(G, 1, get_bitmask(0,60))
CREATE_TEST(H, 1, get_bitmask(0,64))
CREATE_TEST(I, 1, get_bitmask(0,64))
CREATE_TEST(AA, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(BB, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(AAA, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(BBB, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(CCC, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DDD, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DD, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(EE, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(FF, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(GG, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(HH, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(II, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(JJ, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(KK, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(LL, 1, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(MM, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(NN, 1, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(K, 1, get_bitmask(0, 32))
CREATE_TEST(L, 1, get_bitmask(0, 25))
CREATE_TEST(M, 1, get_bitmask(0, 16))
CREATE_TEST(N, 1, get_bitmask(3, 11))

void testSend() {
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
