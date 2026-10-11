#pragma once
#include "Cartridge.h"
#include <cstdint>
#include <memory>

class ppu2C02 {

public:
  ppu2C02();
  ~ppu2C02();

private:
  // One full name table is 1024. But, NES capable to store 2 fullname table.
  uint8_t tblName[2][1024];
  uint8_t tblPallete[32];

  // This should be inside of the cartridge instead of the nes. For now this
  // wont be used.
  uint8_t tblPatter[2][4096];

public:
  // Communications with main Bus
  uint8_t cpuRead(uint16_t addr, bool rdonly = false);
  void cpuWrite(uint16_t addr, uint8_t data);

  // Communications with PPU Bus
  uint8_t ppuRead(uint16_t addr, bool rdonly = false);
  void ppuWrite(uint16_t addr, uint8_t data);

private:
  std::shared_ptr<Cartridge> cart;

public:
  void ConnectCartridge(const std::shared_ptr<Cartridge> &cart);
  void clock();
};
