#include "Bus.h"
#include <cstdint>

Bus::Bus() {

  cpu.ConnectBus(this);

  // Cleaning RAM
  for (auto &i : cpuRam)
    i = 0x00;
}

Bus::~Bus() {
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {

  if (addr >= 0x0000 && addr <= 0x1FFF) {
    cpuRam[addr & 0x07FF] = data;
  } else if (addr >= 0x0000 && addr <= 0x3FFF) {
    ppu.cpuWrite(addr & 0x0007, data);
  }
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {

  uint8_t data = 0x00;
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    data = cpuRam[addr & 0x07FF];
  } else if (addr >= 0x0000 && addr <= 0x3FFF) {
    data = ppu.cpuRead(addr & 0x0007);
  }

  return data;
}

void Bus::reset() {
  cpu.reset();
  nSystemClockCounter = 0;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
  this->cart = cartridge;
  ppu.ConnectCartridge(cartridge);
}
