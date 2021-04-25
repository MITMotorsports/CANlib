#include <cassert>
#include <iostream>
#include "test.hpp"

namespace CANlib {
extern Message_T **messages[2];
}

using namespace CANlib;
using namespace map1;
using namespace map2;
using namespace J;
using namespace CC;

#define CREATE_TEST0(ID, bus_idx, key)                         \
  static void test##ID() {                                     \
    for (int cs = 0; cs < 100; ++cs) {                         \
      Frame f;                                                 \
      f.dlc = distribution(generator);                         \
      for (int i = 0; i < 8; ++i) {                            \
        f.data[i] = distribution(generator);                   \
      }                                                        \
      f.id           = key;                                    \
      int idx        = identify(AbstractBus::map##bus_idx, f); \
      Message_T *msg = messages[bus_idx - 1][idx];             \
      assert((dynamic_cast<ID##_T *>(msg)) != nullptr);        \
    }                                                          \
  }

#define CREATE_TEST1(ID, bus_idx, key)                       \
  static void test##ID() {                                   \
    ID##_T ID##_input;                                       \
    Frame f;                                                 \
    ID##_input.unpack(f);                                    \
    int idx        = identify(AbstractBus::map##bus_idx, f); \
    Message_T *msg = messages[bus_idx - 1][idx];             \
    assert((dynamic_cast<ID##_T *>(msg)) != nullptr);        \
  }

CREATE_TEST0(A, 1, 0X2FF)
CREATE_TEST0(B, 1, 0X305)
CREATE_TEST0(C, 1, 0X306)
CREATE_TEST0(D, 1, 0X307)
CREATE_TEST0(E, 1, 0X308)
CREATE_TEST0(F, 2, 0X016)
CREATE_TEST0(G, 2, 0X017)
CREATE_TEST0(H, 2, 0X0A0)
CREATE_TEST0(I, 2, 0XA2)
CREATE_TEST1(AA, 2, 0)
CREATE_TEST1(BB, 2, 0X14)
CREATE_TEST1(AAA, 2, 0X69)
CREATE_TEST1(BBB, 2, 0X70)
CREATE_TEST1(CCC, 2, 0X71)
CREATE_TEST1(DDD, 2, 0X72)
CREATE_TEST1(DD, 2, 0X65)
CREATE_TEST1(EE, 2, 0X66)
CREATE_TEST1(FF, 2, 0X67)
CREATE_TEST1(GG, 2, 0X68)
CREATE_TEST1(HH, 2, 0X6A)
CREATE_TEST1(II, 2, 0X6B)
CREATE_TEST1(JJ, 2, 0X6C)
CREATE_TEST1(KK, 2, 0X6D)
CREATE_TEST1(LL, 2, 0X6F)
CREATE_TEST1(MM, 2, 0XAB)
CREATE_TEST1(NN, 2, 0X91)
CREATE_TEST0(K, 2, 0X0BB)
CREATE_TEST0(L, 2, 0X0DF)
CREATE_TEST0(M, 2, 0X0F0)
CREATE_TEST0(N, 2, 0X400)

void testIdentify() {
  testA();
  testB();
  testC();
  testD();
  testE();
  testF();
  testG();
  testH();
  testI();
  testAA();
  testBB();
  testAAA();
  testBBB();
  testCCC();
  testDDD();
  testDD();
  testEE();
  testFF();
  testGG();
  testHH();
  testII();
  testJJ();
  testKK();
  testLL();
  testMM();
  testNN();
  testK();
  testL();
  testM();
  testN();
}
