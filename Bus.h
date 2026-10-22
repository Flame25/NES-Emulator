#pragma once

#include "cpu6502.h"
#include "ppu2C02.h"
#include <array>
#include <cstdint>
#include <memory>

class Bus {

public:
  Bus();
  ~Bus();

public: // Devices on bus
  cpu6502 cpu;
  ppu2C02 ppu;

  uint8_t cpuRam[2048];

  std::shared_ptr<Cartridge> cart;

  uint8_t controller[2];

public: // Bus read & write
  // Based on 6502 docs basically it write 8 bit data to 16 bit address
  void cpuWrite(uint16_t addr, uint8_t data);

  uint8_t cpuRead(uint16_t addr, bool bReadOnly = false);

public: // System Interface
  // Connects a cartridge object to the internal buses
  void insertCartridge(const std::shared_ptr<Cartridge> &cartridge);
  // Resets the system
  void reset();
  // Clocks the system - a single whole systme tick
  void clock();

private:
  // Accumulate how many clock function has been called
  uint32_t nSystemClockCounter = 0;

  uint8_t controller_state[2];
};
