#include "test.hpp"
#include "pack_unpack.hpp"
#include <cassert>

using namespace CANlib;
using namespace map1;
using namespace map2;
using namespace J;
using namespace CC;

#define CREATE_TEST(ID, BITMASK) \
    void test##ID() { \
        ID##_T ID##_input;\
        Frame f0; \
        for (int i = 0;i < 8;++i) { \
            f0.data[i] = distribution(generator); \
        } \
        uint64_t bitstring0; \
        to_bitstring((uint8_t*)f0.data, &bitstring0); \
        bitstring0 &= BITMASK; \
        ID##_input.pack(f0); \
        Frame f1; \
        ID##_input.unpack(f1); \
        uint64_t bitstring1;\
        to_bitstring((uint8_t*)f1.data, &bitstring1); \
        assert(bitstring0 == bitstring1); \
    }


CREATE_TEST(A, get_bitmask(7,56))
CREATE_TEST(B, get_bitmask(5,64))
CREATE_TEST(C, get_bitmask(0,56))
CREATE_TEST(D, get_bitmask(0,18))
CREATE_TEST(E, get_bitmask(0,64))
CREATE_TEST(F, get_bitmask(0,10) | get_bitmask(18, 28) | get_bitmask(36, 46) | get_bitmask(54, 64))
CREATE_TEST(G, get_bitmask(0,60))
CREATE_TEST(H, get_bitmask(0,64))
CREATE_TEST(I, get_bitmask(0,64))
CREATE_TEST(AA, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(BB, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(AAA, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(BBB, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(CCC, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DDD, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(DD, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(EE, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(FF, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(GG, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(HH, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(II, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(JJ, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(KK, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(LL, get_bitmask(23, 24) | get_bitmask(32, 48))
CREATE_TEST(MM, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(NN, get_bitmask(23, 24) | get_bitmask(39, 40))
CREATE_TEST(K, get_bitmask(0, 32))
CREATE_TEST(L, get_bitmask(0, 25))
CREATE_TEST(M, get_bitmask(0, 16))
CREATE_TEST(N, get_bitmask(3, 11))


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
