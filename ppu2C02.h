#pragma once
#include "Cartridge.h"
#include "olcPixelGameEngine.h"
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

private:
  int16_t scanline = 0;
  int16_t cycle = 0;

private:
  olc::Pixel palScreen[0x40];
  // In Video
  // olc::Sprite sprScreen = olc::Sprite(256, 240);
  // olc::Sprite sprNameTable[2] = { olc::Sprite(256, 240), olc::Sprite(256,
  // 240) }; olc::Sprite sprPatternTable[2] = { olc::Sprite(128, 128),
  // olc::Sprite(128, 128) };

  // Changed To for API breaking subsequent PGE Update
  olc::Sprite *sprScreen;
  olc::Sprite *sprNameTable[2];
  olc::Sprite *sprPatternTable[2];

public:
  // Debugging Utilities
  olc::Sprite &GetScreen();
  olc::Sprite &GetNameTable(uint8_t i);
  olc::Sprite &GetPatternTable(uint8_t i);
  bool frame_complete = false;
};
