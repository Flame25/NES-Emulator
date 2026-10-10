#pragma once

#include "olc6502.h"
#include <array>
#include <cstdint>
class Bus {

public:
  Bus();
  ~Bus();

public: // Devices on bus
  olc6502 cpu;

  // Fake RAM
  // 6502 Has only up to 64Kb memory
  std::array<uint8_t, 64 * 1024> ram;

public: // Bus read & write
  // Based on olc6502 docs basically it write 8 bit data to 16 bit address
  void write(uint16_t addr, uint8_t data);

  uint8_t read(uint16_t addr, bool bReadOnly = false);
};
