#include "../src/CANlib.h"

int main() {
  CANlib_Init(10);
  CANlib_master_CellTemperatureRange_T ctr;

  CANlib_Transmit_master_CellTemperatureRange(&ctr);
  return 0;
}
