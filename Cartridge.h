#pragma once
#include "Mapper_000.h"
#include <cstdint>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

// Basically cartridge physically has 2 data pin one that connect to CPU bus and
// one to PPU bus.
//
// Inside of the cartridge there is mapper because our code (NES) will receive
// data and send address to the cartridge. Therefore, there is mapper inside the
// cartridge work to map our address to Program memory (PRG) or Pattern Memory
// (CHR). Inside of each memory (PRG & CHR), there is somesort of block, (bank
// for PRG and level for CHR) so mapper will map the address send by PPU and CPU
// to read correct bank or level. Mapper can works differently either easier or
// more sophiscated than this.

class Cartridge {
public:
  Cartridge(const std::string &sFileName);
  ~Cartridge();

public:
  bool ImageValid();

  enum MIRROR {
    HORIZONTAL,
    VERTICAL,
    ONESCREEN_LO,
    ONESCREEN_HI,
  } mirror = HORIZONTAL;

private:
  std::vector<uint8_t> vPRGMemory;
  std::vector<uint8_t> vCHRMemory;

  bool bImageValid = false;

  uint8_t nMapperID = 0;
  uint8_t nPRGBanks = 0;
  uint8_t nCHRBanks = 0;

  std::shared_ptr<Mapper> pMapper;

public:
  // We use boolean to tell main system(bus) if the cartridge has been
  // read/write properly or not Communications with main Bus
  bool cpuRead(uint16_t addr, uint8_t &data);
  bool cpuWrite(uint16_t addr, uint8_t data);

  // Communications with PPU Bus
  bool ppuRead(uint16_t addr, uint8_t &data);
  bool ppuWrite(uint16_t addr, uint8_t data);

  void reset();
};
