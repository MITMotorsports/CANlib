#include "../src/CANlib.h"

int main() {
  CANlib_Init(10);
  CANlib_can0_CellTemperatureRange_T ctr;

  CANlib_Transmit_can0_CellTemperatureRange(&ctr);
  return 0;
}
