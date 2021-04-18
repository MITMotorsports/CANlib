#include <cassert>
#include <chrono>
#include "test.hpp"

namespace CANlib {
namespace map1 {
extern Message_Type_T message_types[6];
}
namespace map2 {
extern Message_Type_T message_types[28];
}
}  // namespace CANlib

using namespace CANlib;
using namespace std::chrono_literals;
using map1::A_T;
using map1::B_T;
using map1::C_T;
using map1::D_T;
using map1::E_T;
using map2::F_T;
using map2::G_T;
using map2::H_T;
using map2::I_T;
using map2::K_T;
using map2::L_T;
using map2::M_T;
using map2::N_T;
using map2::J::AA_T;
using map2::J::BB_T;
using map2::J::DD_T;
using map2::J::EE_T;
using map2::J::FF_T;
using map2::J::GG_T;
using map2::J::HH_T;
using map2::J::II_T;
using map2::J::JJ_T;
using map2::J::KK_T;
using map2::J::LL_T;
using map2::J::MM_T;
using map2::J::NN_T;
using map2::J::CC::AAA_T;
using map2::J::CC::BBB_T;
using map2::J::CC::CCC_T;
using map2::J::CC::DDD_T;

void testKeys() {
  map1::A_T A_input;
  map1::B_T B_input;
  map1::C_T C_input;
  map1::D_T D_input;
  map1::E_T E_input;
  map2::F_T F_input;
  map2::G_T G_input;
  map2::H_T H_input;
  map2::I_T I_input;
  map2::J::AA_T AA_input;
  map2::J::BB_T BB_input;
  map2::J::CC::AAA_T AAA_input;
  map2::J::CC::BBB_T BBB_input;
  map2::J::CC::CCC_T CCC_input;
  map2::J::CC::DDD_T DDD_input;
  map2::J::DD_T DD_input;
  map2::J::EE_T EE_input;
  map2::J::FF_T FF_input;
  map2::J::GG_T GG_input;
  map2::J::HH_T HH_input;
  map2::J::II_T II_input;
  map2::J::JJ_T JJ_input;
  map2::J::KK_T KK_input;
  map2::J::LL_T LL_input;
  map2::J::MM_T MM_input;
  map2::J::NN_T NN_input;
  map2::K_T K_input;
  map2::L_T L_input;
  map2::M_T M_input;
  map2::N_T N_input;
  assert(A_input.get_key() == 0x2FF);
  assert(map1::A_T::key == 0x2FF);
  assert(B_input.get_key() == 0x305);
  assert(map1::B_T::key == 0x305);
  assert(C_input.get_key() == 0x306);
  assert(map1::C_T::key == 0x306);
  assert(D_input.get_key() == 0x307);
  assert(map1::D_T::key == 0x307);
  assert(E_input.get_key() == 0x308);
  assert(map1::E_T::key == 0x308);
  assert(F_input.get_key() == 0x16);
  assert(map2::F_T::key == 0x16);
  assert(G_input.get_key() == 0x17);
  assert(map2::G_T::key == 0x17);
  assert(H_input.get_key() == 0xA0);
  assert(map2::H_T::key == 0xA0);
  assert(I_input.get_key() == 0xA2);
  assert(map2::I_T::key == 0xA2);
  assert(map2::J::key == 0xC2);
  assert(AA_input.get_key() == 0x0);
  assert(map2::J::AA_T::key == 0x0);
  assert(BB_input.get_key() == 0x14);
  assert(map2::J::BB_T::key == 0x14);
  assert(map2::J::CC::key == 0x64);
  assert(AAA_input.get_key() == 0x69);
  assert(map2::J::CC::AAA_T::key == 0x69);
  assert(BBB_input.get_key() == 0x70);
  assert(map2::J::CC::BBB_T::key == 0x70);
  assert(CCC_input.get_key() == 0x71);
  assert(map2::J::CC::CCC_T::key == 0x71);
  assert(DDD_input.get_key() == 0x72);
  assert(map2::J::CC::DDD_T::key == 0x72);
  assert(DD_input.get_key() == 0x65);
  assert(map2::J::DD_T::key == 0x65);
  assert(EE_input.get_key() == 0x66);
  assert(map2::J::EE_T::key == 0x66);
  assert(FF_input.get_key() == 0x67);
  assert(map2::J::FF_T::key == 0x67);
  assert(GG_input.get_key() == 0x68);
  assert(map2::J::GG_T::key == 0x68);
  assert(HH_input.get_key() == 0x6A);
  assert(map2::J::HH_T::key == 0x6A);
  assert(II_input.get_key() == 0x6B);
  assert(map2::J::II_T::key == 0x6B);
  assert(JJ_input.get_key() == 0x6C);
  assert(map2::J::JJ_T::key == 0x6C);
  assert(KK_input.get_key() == 0x6D);
  assert(map2::J::KK_T::key == 0x6D);
  assert(LL_input.get_key() == 0x6F);
  assert(map2::J::LL_T::key == 0x6F);
  assert(MM_input.get_key() == 0xAB);
  assert(map2::J::MM_T::key == 0xAB);
  assert(NN_input.get_key() == 0x91);
  assert(map2::J::NN_T::key == 0x91);
  assert(K_input.get_key() == 0xBB);
  assert(map2::K_T::key == 0xBB);
  assert(L_input.get_key() == 0xDF);
  assert(map2::L_T::key == 0xDF);
  assert(M_input.get_key() == 0xF0);
  assert(map2::M_T::key == 0xF0);
  assert(N_input.get_key() == 0x400);
  assert(map2::N_T::key == 0x400);
}

void testPeriod() {
  map1::A_T A_input;
  map1::B_T B_input;
  map1::C_T C_input;
  map1::D_T D_input;
  map1::E_T E_input;
  map2::F_T F_input;
  map2::G_T G_input;
  map2::H_T H_input;
  map2::I_T I_input;
  map2::J::AA_T AA_input;
  map2::J::BB_T BB_input;
  map2::J::CC::AAA_T AAA_input;
  map2::J::CC::BBB_T BBB_input;
  map2::J::CC::CCC_T CCC_input;
  map2::J::CC::DDD_T DDD_input;
  map2::J::DD_T DD_input;
  map2::J::EE_T EE_input;
  map2::J::FF_T FF_input;
  map2::J::GG_T GG_input;
  map2::J::HH_T HH_input;
  map2::J::II_T II_input;
  map2::J::JJ_T JJ_input;
  map2::J::KK_T KK_input;
  map2::J::LL_T LL_input;
  map2::J::MM_T MM_input;
  map2::J::NN_T NN_input;
  map2::K_T K_input;
  map2::L_T L_input;
  map2::M_T M_input;
  map2::N_T N_input;
  assert(A_input.get_period() == 50ms);
  assert(B_input.get_period() == 200ms);
  assert(C_input.get_period() == 200ms);
  assert(D_input.get_period() == 200ms);
  assert(E_input.get_period() == 1000ms);
  assert(F_input.get_period() == 101ms);
  assert(G_input.get_period() == 31ms);
  assert(H_input.get_period() == 0ms);
  assert(I_input.get_period() == 0ms);
  assert(AA_input.get_period() == 0ms);
  assert(BB_input.get_period() == 0ms);
  assert(AAA_input.get_period() == 0ms);
  assert(BBB_input.get_period() == 0ms);
  assert(CCC_input.get_period() == 0ms);
  assert(DDD_input.get_period() == 0ms);
  assert(DD_input.get_period() == 0ms);
  assert(EE_input.get_period() == 0ms);
  assert(FF_input.get_period() == 0ms);
  assert(GG_input.get_period() == 0ms);
  assert(HH_input.get_period() == 0ms);
  assert(II_input.get_period() == 0ms);
  assert(JJ_input.get_period() == 0ms);
  assert(KK_input.get_period() == 0ms);
  assert(LL_input.get_period() == 0ms);
  assert(MM_input.get_period() == 0ms);
  assert(NN_input.get_period() == 0ms);
  assert(K_input.get_period() == 0ms);
  assert(L_input.get_period() == 103ms);
  assert(M_input.get_period() == 103ms);
  assert(N_input.get_period() == 0ms);
}

#define CREATE_TEST(ID, bus_idx, key, msg)                                         \
  static void testMessageType##ID() {                                              \
    ID##_T ID##_input;                                                             \
    Frame f;                                                                       \
    ID##_input.unpack(f);                                                          \
    int idx = Identify(Bus_Names_T::map##bus_idx, f);                              \
    assert(map##bus_idx::message_types[idx] == map##bus_idx::Message_Type_T::msg); \
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
  map1::A_T A_input;
  map1::B_T B_input;
  map1::C_T C_input;
  map1::D_T D_input;
  map1::E_T E_input;
  map2::F_T F_input;
  map2::G_T G_input;
  map2::H_T H_input;
  map2::I_T I_input;
  map2::J::AA_T AA_input;
  map2::J::BB_T BB_input;
  map2::J::CC::AAA_T AAA_input;
  map2::J::CC::BBB_T BBB_input;
  map2::J::CC::CCC_T CCC_input;
  map2::J::CC::DDD_T DDD_input;
  map2::J::DD_T DD_input;
  map2::J::EE_T EE_input;
  map2::J::FF_T FF_input;
  map2::J::GG_T GG_input;
  map2::J::HH_T HH_input;
  map2::J::II_T II_input;
  map2::J::JJ_T JJ_input;
  map2::J::KK_T KK_input;
  map2::J::LL_T LL_input;
  map2::J::MM_T MM_input;
  map2::J::NN_T NN_input;
  map2::K_T K_input;
  map2::L_T L_input;
  map2::M_T M_input;
  map2::N_T N_input;
  assert(A_input.get_bus_name() == Bus_Names_T::map1);
  assert(B_input.get_bus_name() == Bus_Names_T::map1);
  assert(C_input.get_bus_name() == Bus_Names_T::map1);
  assert(D_input.get_bus_name() == Bus_Names_T::map1);
  assert(E_input.get_bus_name() == Bus_Names_T::map1);
  assert(F_input.get_bus_name() == Bus_Names_T::map2);
  assert(G_input.get_bus_name() == Bus_Names_T::map2);
  assert(H_input.get_bus_name() == Bus_Names_T::map2);
  assert(I_input.get_bus_name() == Bus_Names_T::map2);
  assert(AA_input.get_bus_name() == Bus_Names_T::map2);
  assert(BB_input.get_bus_name() == Bus_Names_T::map2);
  assert(AAA_input.get_bus_name() == Bus_Names_T::map2);
  assert(BBB_input.get_bus_name() == Bus_Names_T::map2);
  assert(CCC_input.get_bus_name() == Bus_Names_T::map2);
  assert(DDD_input.get_bus_name() == Bus_Names_T::map2);
  assert(DD_input.get_bus_name() == Bus_Names_T::map2);
  assert(EE_input.get_bus_name() == Bus_Names_T::map2);
  assert(FF_input.get_bus_name() == Bus_Names_T::map2);
  assert(GG_input.get_bus_name() == Bus_Names_T::map2);
  assert(HH_input.get_bus_name() == Bus_Names_T::map2);
  assert(II_input.get_bus_name() == Bus_Names_T::map2);
  assert(JJ_input.get_bus_name() == Bus_Names_T::map2);
  assert(KK_input.get_bus_name() == Bus_Names_T::map2);
  assert(LL_input.get_bus_name() == Bus_Names_T::map2);
  assert(MM_input.get_bus_name() == Bus_Names_T::map2);
  assert(NN_input.get_bus_name() == Bus_Names_T::map2);
  assert(K_input.get_bus_name() == Bus_Names_T::map2);
  assert(L_input.get_bus_name() == Bus_Names_T::map2);
  assert(M_input.get_bus_name() == Bus_Names_T::map2);
  assert(N_input.get_bus_name() == Bus_Names_T::map2);
}
