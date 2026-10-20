#include "Cartridge.h"
#include "Mapper_000.h"
#include <cstdint>

Cartridge::Cartridge(const std::string &sFileName) {

  // iNES format header
  struct sHeader {
    char name[4];
    uint8_t prg_rom_chunks;
    uint8_t chr_rom_chunks;

    // Mapper is using 2 8-bit not means there is 2 mapper here
    uint8_t mapper1;
    uint8_t mapper2;
    uint8_t prg_ram_size;
    uint8_t tv_system1;
    uint8_t tv_system2;
    char unused[5];
  } header;

  bImageValid = false;

  std::ifstream ifs;
  ifs.open(sFileName, std::ifstream::binary);
  if (ifs.is_open()) {

    // Read file header
    ifs.read((char *)&header, sizeof(sHeader));

    // If a "trainer" exists we just need to read past
    // it before we get to the good stuff
    if (header.mapper1 & 0x04) {
      ifs.seekg(512, std::ios_base::cur);
    }

    // Combine the mapper 1 & 2
    nMapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
    mirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

    uint8_t nFileType = 1;

    if (nFileType == 0) {

    } else if (nFileType == 1) {
      // Read how many banks in the memory
      nPRGBanks = header.prg_rom_chunks;
      vPRGMemory.resize(nPRGBanks *
                        16384); // Resize vector based on the PRG banks
      ifs.read((char *)vPRGMemory.data(), vPRGMemory.size());

      nCHRBanks = header.chr_rom_chunks;

      if (nCHRBanks == 0) {
        // Create CHR RAM
        vCHRMemory.resize(8192);
      } else {
        // Allocate for ROM
        vCHRMemory.resize(nCHRBanks * 8192);
      }
      ifs.read((char *)vCHRMemory.data(), vCHRMemory.size());

    } else if (nFileType == 2) {
    }

    switch (nMapperID) {
    case 0:
      pMapper = std::make_shared<Mapper_000>(nPRGBanks, nCHRBanks);
      break;
    }
    bImageValid = true;
    ifs.close();
  }
}

Cartridge::~Cartridge() {
}

bool Cartridge::ImageValid() {
  return bImageValid;
}

bool Cartridge::cpuRead(uint16_t addr, uint8_t &data) {
  uint32_t mapped_addr = 0;
  // Address will only be transformed if the corresponding mapper for that
  // spesific bus says that information or data need come from the cartrdige. If
  // it's the case mapper will send back the offset (address) of the rom
  // location on the cartridge to that bus
  if (pMapper->cpuMapRead(addr, mapped_addr)) {
    data = vPRGMemory[mapped_addr];

    // Means cartridge handled the particular address
    return true;
  }

  return false;
}

bool Cartridge::cpuWrite(uint16_t addr, uint8_t data) {
  uint32_t mapped_addr = 0;
  if (pMapper->cpuMapWrite(addr, mapped_addr)) {
    vPRGMemory[mapped_addr] = data;
    return true;
  }

  return false;
}

bool Cartridge::ppuRead(uint16_t addr, uint8_t &data) {
  uint32_t mapped_addr = 0;

  if (pMapper->ppuMapRead(addr, mapped_addr)) {
    data = vCHRMemory[mapped_addr];

    return true;
  }

  return false;
}

bool Cartridge::ppuWrite(uint16_t addr, uint8_t data) {
  uint32_t mapped_addr = 0;
  if (pMapper->ppuMapWrite(addr, mapped_addr)) {
    vCHRMemory[mapped_addr] = data;
    return true;
  }

  return false;
}

void Cartridge::reset() {
  // Note: This does not reset the ROM contents,
  // but does reset the mapper.
  if (pMapper != nullptr)
    pMapper->reset();
}
