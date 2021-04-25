#include <cassert>
#include <chrono>
#include "test.hpp"

namespace CANlib {
namespace map1 {
extern MessageType message_types[6];
}
namespace map2 {
extern MessageType message_types[28];
}
}  // namespace CANlib

using namespace CANlib;
using namespace std::chrono_literals;
using map1::A;
using map1::B;
using map1::C;
using map1::D;
using map1::E;
using map2::F;
using map2::G;
using map2::H;
using map2::I;
using map2::K;
using map2::L;
using map2::M;
using map2::N;
using map2::J::AA;
using map2::J::BB;
using map2::J::DD;
using map2::J::EE;
using map2::J::FF;
using map2::J::GG;
using map2::J::HH;
using map2::J::II;
using map2::J::JJ;
using map2::J::KK;
using map2::J::LL;
using map2::J::MM;
using map2::J::NN;
using map2::J::CC::AAA;
using map2::J::CC::BBB;
using map2::J::CC::CCC;
using map2::J::CC::DDD;

void testKeys() {
  A A_copy;
  B B_copy;
  C C_copy;
  D D_copy;
  E E_copy;
  F F_copy;
  G G_copy;
  H H_copy;
  I I_copy;
  AA AA_copy;
  BB BB_copy;
  AAA AAA_copy;
  BBB BBB_copy;
  CCC CCC_copy;
  DDD DDD_copy;
  DD DD_copy;
  EE EE_copy;
  FF FF_copy;
  GG GG_copy;
  HH HH_copy;
  II II_copy;
  JJ JJ_copy;
  KK KK_copy;
  LL LL_copy;
  MM MM_copy;
  NN NN_copy;
  K K_copy;
  L L_copy;
  M M_copy;
  N N_copy;
  assert(A_copy.get_key() == 0x2FF);
  assert(map1::A::key == 0x2FF);
  assert(B_copy.get_key() == 0x305);
  assert(map1::B::key == 0x305);
  assert(C_copy.get_key() == 0x306);
  assert(map1::C::key == 0x306);
  assert(D_copy.get_key() == 0x307);
  assert(map1::D::key == 0x307);
  assert(E_copy.get_key() == 0x308);
  assert(map1::E::key == 0x308);
  assert(F_copy.get_key() == 0x16);
  assert(map2::F::key == 0x16);
  assert(G_copy.get_key() == 0x17);
  assert(map2::G::key == 0x17);
  assert(H_copy.get_key() == 0xA0);
  assert(map2::H::key == 0xA0);
  assert(I_copy.get_key() == 0xA2);
  assert(map2::I::key == 0xA2);
  assert(map2::J::key == 0xC2);
  assert(AA_copy.get_key() == 0x0);
  assert(map2::J::AA::key == 0x0);
  assert(BB_copy.get_key() == 0x14);
  assert(map2::J::BB::key == 0x14);
  assert(map2::J::CC::key == 0x64);
  assert(AAA_copy.get_key() == 0x69);
  assert(map2::J::CC::AAA::key == 0x69);
  assert(BBB_copy.get_key() == 0x70);
  assert(map2::J::CC::BBB::key == 0x70);
  assert(CCC_copy.get_key() == 0x71);
  assert(map2::J::CC::CCC::key == 0x71);
  assert(DDD_copy.get_key() == 0x72);
  assert(map2::J::CC::DDD::key == 0x72);
  assert(DD_copy.get_key() == 0x65);
  assert(map2::J::DD::key == 0x65);
  assert(EE_copy.get_key() == 0x66);
  assert(map2::J::EE::key == 0x66);
  assert(FF_copy.get_key() == 0x67);
  assert(map2::J::FF::key == 0x67);
  assert(GG_copy.get_key() == 0x68);
  assert(map2::J::GG::key == 0x68);
  assert(HH_copy.get_key() == 0x6A);
  assert(map2::J::HH::key == 0x6A);
  assert(II_copy.get_key() == 0x6B);
  assert(map2::J::II::key == 0x6B);
  assert(JJ_copy.get_key() == 0x6C);
  assert(map2::J::JJ::key == 0x6C);
  assert(KK_copy.get_key() == 0x6D);
  assert(map2::J::KK::key == 0x6D);
  assert(LL_copy.get_key() == 0x6F);
  assert(map2::J::LL::key == 0x6F);
  assert(MM_copy.get_key() == 0xAB);
  assert(map2::J::MM::key == 0xAB);
  assert(NN_copy.get_key() == 0x91);
  assert(map2::J::NN::key == 0x91);
  assert(K_copy.get_key() == 0xBB);
  assert(map2::K::key == 0xBB);
  assert(L_copy.get_key() == 0xDF);
  assert(map2::L::key == 0xDF);
  assert(M_copy.get_key() == 0xF0);
  assert(map2::M::key == 0xF0);
  assert(N_copy.get_key() == 0x400);
  assert(map2::N::key == 0x400);
}

void testPeriod() {
  map1::A A_copy;
  map1::B B_copy;
  map1::C C_copy;
  map1::D D_copy;
  map1::E E_copy;
  map2::F F_copy;
  map2::G G_copy;
  map2::H H_copy;
  map2::I I_copy;
  map2::J::AA AA_copy;
  map2::J::BB BB_copy;
  map2::J::CC::AAA AAA_copy;
  map2::J::CC::BBB BBB_copy;
  map2::J::CC::CCC CCC_copy;
  map2::J::CC::DDD DDD_copy;
  map2::J::DD DD_copy;
  map2::J::EE EE_copy;
  map2::J::FF FF_copy;
  map2::J::GG GG_copy;
  map2::J::HH HH_copy;
  map2::J::II II_copy;
  map2::J::JJ JJ_copy;
  map2::J::KK KK_copy;
  map2::J::LL LL_copy;
  map2::J::MM MM_copy;
  map2::J::NN NN_copy;
  map2::K K_copy;
  map2::L L_copy;
  map2::M M_copy;
  map2::N N_copy;
  assert(A_copy.get_period() == 50ms);
  assert(B_copy.get_period() == 200ms);
  assert(C_copy.get_period() == 200ms);
  assert(D_copy.get_period() == 200ms);
  assert(E_copy.get_period() == 1000ms);
  assert(F_copy.get_period() == 101ms);
  assert(G_copy.get_period() == 31ms);
  assert(H_copy.get_period() == 0ms);
  assert(I_copy.get_period() == 0ms);
  assert(AA_copy.get_period() == 0ms);
  assert(BB_copy.get_period() == 0ms);
  assert(AAA_copy.get_period() == 0ms);
  assert(BBB_copy.get_period() == 0ms);
  assert(CCC_copy.get_period() == 0ms);
  assert(DDD_copy.get_period() == 0ms);
  assert(DD_copy.get_period() == 0ms);
  assert(EE_copy.get_period() == 0ms);
  assert(FF_copy.get_period() == 0ms);
  assert(GG_copy.get_period() == 0ms);
  assert(HH_copy.get_period() == 0ms);
  assert(II_copy.get_period() == 0ms);
  assert(JJ_copy.get_period() == 0ms);
  assert(KK_copy.get_period() == 0ms);
  assert(LL_copy.get_period() == 0ms);
  assert(MM_copy.get_period() == 0ms);
  assert(NN_copy.get_period() == 0ms);
  assert(K_copy.get_period() == 0ms);
  assert(L_copy.get_period() == 103ms);
  assert(M_copy.get_period() == 103ms);
  assert(N_copy.get_period() == 0ms);
}

#define CREATE_TEST(ID, bus_idx, key, msg)                                      \
  static void testMessageType##ID() {                                           \
    ID ID##_copy;                                                               \
    Frame f;                                                                    \
    ID##_copy.unpack(f);                                                        \
    int idx = identify(AbstractBus::map##bus_idx, f);                           \
    assert(map##bus_idx::message_types[idx] == map##bus_idx::MessageType::msg); \
  }
CREATE_TEST(A, 1, 0X2FF, A)
CREATE_TEST(B, 1, 0X305, B)
CREATE_TEST(C, 1, 0X306, C)
CREATE_TEST(D, 1, 0X307, D)
CREATE_TEST(E, 1, 0X308, E)
CREATE_TEST(F, 2, 0X016, F)
CREATE_TEST(G, 2, 0X017, G)
CREATE_TEST(H, 2, 0X0A0, H)
CREATE_TEST(I, 2, 0XA2, I)
CREATE_TEST(AA, 2, 0, J_AA)
CREATE_TEST(BB, 2, 0X14, J_BB)
CREATE_TEST(AAA, 2, 0X69, J_CC_AAA)
CREATE_TEST(BBB, 2, 0X70, J_CC_BBB)
CREATE_TEST(CCC, 2, 0X71, J_CC_CCC)
CREATE_TEST(DDD, 2, 0X72, J_CC_DDD)
CREATE_TEST(DD, 2, 0X65, J_DD)
CREATE_TEST(EE, 2, 0X66, J_EE)
CREATE_TEST(FF, 2, 0X67, J_FF)
CREATE_TEST(GG, 2, 0X68, J_GG)
CREATE_TEST(HH, 2, 0X6A, J_HH)
CREATE_TEST(II, 2, 0X6B, J_II)
CREATE_TEST(JJ, 2, 0X6C, J_JJ)
CREATE_TEST(KK, 2, 0X6D, J_KK)
CREATE_TEST(LL, 2, 0X6F, J_LL)
CREATE_TEST(MM, 2, 0XAB, J_MM)
CREATE_TEST(NN, 2, 0X91, J_NN)
CREATE_TEST(K, 2, 0X0BB, K)
CREATE_TEST(L, 2, 0X0DF, L)
CREATE_TEST(M, 2, 0X0F0, M)
CREATE_TEST(N, 2, 0X400, N)

void testMessageType() {
  testMessageTypeA();
  testMessageTypeB();
  testMessageTypeC();
  testMessageTypeD();
  testMessageTypeE();
  testMessageTypeF();
  testMessageTypeG();
  testMessageTypeH();
  testMessageTypeI();
  testMessageTypeAA();
  testMessageTypeBB();
  testMessageTypeAAA();
  testMessageTypeBBB();
  testMessageTypeCCC();
  testMessageTypeDDD();
  testMessageTypeDD();
  testMessageTypeEE();
  testMessageTypeFF();
  testMessageTypeGG();
  testMessageTypeHH();
  testMessageTypeII();
  testMessageTypeJJ();
  testMessageTypeKK();
  testMessageTypeLL();
  testMessageTypeMM();
  testMessageTypeNN();
  testMessageTypeK();
  testMessageTypeL();
  testMessageTypeM();
  testMessageTypeN();
}

void testBusNames() {
  map1::A A_copy;
  map1::B B_copy;
  map1::C C_copy;
  map1::D D_copy;
  map1::E E_copy;
  map2::F F_copy;
  map2::G G_copy;
  map2::H H_copy;
  map2::I I_copy;
  map2::J::AA AA_copy;
  map2::J::BB BB_copy;
  map2::J::CC::AAA AAA_copy;
  map2::J::CC::BBB BBB_copy;
  map2::J::CC::CCC CCC_copy;
  map2::J::CC::DDD DDD_copy;
  map2::J::DD DD_copy;
  map2::J::EE EE_copy;
  map2::J::FF FF_copy;
  map2::J::GG GG_copy;
  map2::J::HH HH_copy;
  map2::J::II II_copy;
  map2::J::JJ JJ_copy;
  map2::J::KK KK_copy;
  map2::J::LL LL_copy;
  map2::J::MM MM_copy;
  map2::J::NN NN_copy;
  map2::K K_copy;
  map2::L L_copy;
  map2::M M_copy;
  map2::N N_copy;
  assert(A_copy.get_bus_name() == AbstractBus::map1);
  assert(B_copy.get_bus_name() == AbstractBus::map1);
  assert(C_copy.get_bus_name() == AbstractBus::map1);
  assert(D_copy.get_bus_name() == AbstractBus::map1);
  assert(E_copy.get_bus_name() == AbstractBus::map1);
  assert(F_copy.get_bus_name() == AbstractBus::map2);
  assert(G_copy.get_bus_name() == AbstractBus::map2);
  assert(H_copy.get_bus_name() == AbstractBus::map2);
  assert(I_copy.get_bus_name() == AbstractBus::map2);
  assert(AA_copy.get_bus_name() == AbstractBus::map2);
  assert(BB_copy.get_bus_name() == AbstractBus::map2);
  assert(AAA_copy.get_bus_name() == AbstractBus::map2);
  assert(BBB_copy.get_bus_name() == AbstractBus::map2);
  assert(CCC_copy.get_bus_name() == AbstractBus::map2);
  assert(DDD_copy.get_bus_name() == AbstractBus::map2);
  assert(DD_copy.get_bus_name() == AbstractBus::map2);
  assert(EE_copy.get_bus_name() == AbstractBus::map2);
  assert(FF_copy.get_bus_name() == AbstractBus::map2);
  assert(GG_copy.get_bus_name() == AbstractBus::map2);
  assert(HH_copy.get_bus_name() == AbstractBus::map2);
  assert(II_copy.get_bus_name() == AbstractBus::map2);
  assert(JJ_copy.get_bus_name() == AbstractBus::map2);
  assert(KK_copy.get_bus_name() == AbstractBus::map2);
  assert(LL_copy.get_bus_name() == AbstractBus::map2);
  assert(MM_copy.get_bus_name() == AbstractBus::map2);
  assert(NN_copy.get_bus_name() == AbstractBus::map2);
  assert(K_copy.get_bus_name() == AbstractBus::map2);
  assert(L_copy.get_bus_name() == AbstractBus::map2);
  assert(M_copy.get_bus_name() == AbstractBus::map2);
  assert(N_copy.get_bus_name() == AbstractBus::map2);
}
