#pragma once
#include <random>
#include "bus.hpp"
#include "driver.hpp"
#include "structs.hpp"
#include "testfamily.hpp"

#ifdef ARCH0
#define MAP1_CAN can2
#define MAP2_CAN can1
#endif

#ifdef ARCH1
#define MAP1_CAN can2
#define MAP2_CAN can3
#endif

#ifdef ARCH2
#define MAP1_CAN can3
#define MAP2_CAN can1
#endif

extern std::default_random_engine generator;
extern std::uniform_int_distribution<uint16_t> distribution;

void testArch();
void testPackUnpack();
void testKeys();
void testPeriod();
void testMessageType();
void testIdentify();
void testSend();
