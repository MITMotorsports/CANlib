#include <cassert>
#include <iostream>
#include "pack_unpack.hpp"
#include "test.hpp"

using namespace CANlib;
using namespace map1;
using namespace map2;
using namespace J;
using namespace CC;

#define CREATE_TEST(Name, BITMASK)                   \
  void test##Name() {                                \
    for (int cs = 0; cs < 100000; ++cs) {            \
      Name Name##_copy;                              \
      Frame f0;                                      \
      for (int i = 0; i < 8; ++i) {                  \
        f0.data[i] = distribution(generator);        \
      }                                              \
      uint64_t bitstring0;                           \
      to_bitstring((uint8_t *)f0.data, &bitstring0); \
      bitstring0 &= BITMASK;                         \
      Name##_copy.pack(f0);                          \
      Frame f1;                                      \
      memset(f1.data, 0, sizeof(f1.data));           \
      Name##_copy.unpack(f1);                        \
      uint64_t bitstring1;                           \
      to_bitstring((uint8_t *)f1.data, &bitstring1); \
      bitstring1 &= BITMASK;                         \
      assert(bitstring0 == bitstring1);              \
    }                                                \
  }

CREATE_TEST(A, (~ZEROES_MASK(7, 49)))
CREATE_TEST(B, (~ZEROES_MASK(5, 59)))
CREATE_TEST(C, (~ZEROES_MASK(0, 56)))
CREATE_TEST(D,
            (~ZEROES_MASK(0, 1)) | (~ZEROES_MASK(2, 1)) | (~ZEROES_MASK(3, 1)) |
                (~ZEROES_MASK(4, 1)) | (~ZEROES_MASK(5, 1)) | (~ZEROES_MASK(6, 1)) |
                (~ZEROES_MASK(7, 1)) | (~ZEROES_MASK(9, 1)) | (~ZEROES_MASK(16, 1)) |
                (~ZEROES_MASK(17, 1)))
CREATE_TEST(E, (~ZEROES_MASK(0, 64)))
CREATE_TEST(F,
            (~ZEROES_MASK(0, 10)) | (~ZEROES_MASK(10, 8)) | (~ZEROES_MASK(18, 10)) |
                (~ZEROES_MASK(28, 8)) | (~ZEROES_MASK(36, 10)) | (~ZEROES_MASK(46, 8)) |
                (~ZEROES_MASK(54, 10)))
CREATE_TEST(G, (~ZEROES_MASK(0, 60)))
CREATE_TEST(H, (~ZEROES_MASK(0, 64)))
CREATE_TEST(I, (~ZEROES_MASK(0, 64)))
CREATE_TEST(AA, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(39, 1)))
CREATE_TEST(BB, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(39, 1)))
CREATE_TEST(AAA, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(BBB, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(32, 16)))
CREATE_TEST(CCC, (~ZEROES_MASK(23, 1)) | (~ZEROES_MASK(32, 16)))
CREATE_TEST(DDD, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(DD, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(EE, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(FF, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(GG, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(HH, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(II, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(JJ, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(KK, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(LL, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(32, 16))))
CREATE_TEST(MM, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(39, 1))))
CREATE_TEST(NN, (~ZEROES_MASK(23, 1) | (~ZEROES_MASK(39, 1))))
CREATE_TEST(K, (~ZEROES_MASK(0, 32)))
CREATE_TEST(L, (~ZEROES_MASK(0, 25)))
CREATE_TEST(M, (~ZEROES_MASK(0, 16)))
CREATE_TEST(N,
            ((~ZEROES_MASK(3, 1)) | (~ZEROES_MASK(4, 3)) | (~ZEROES_MASK(8, 1)) |
             (~ZEROES_MASK(9, 1)) | (~ZEROES_MASK(10, 1))))

void testPackUnpack() {
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
