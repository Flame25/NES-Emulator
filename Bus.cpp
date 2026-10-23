#include "Bus.h"

Bus::Bus() {

  cpu.ConnectBus(this);
}

Bus::~Bus() {
}

void Bus::cpuWrite(uint16_t addr, uint8_t data) {

  // Cartridge being able to interrupt the cpu or main bus. But we wont bother
  // this for now.
  if (cart->cpuWrite(addr, data)) {
  } else if (addr >= 0x0000 && addr <= 0x1FFF) {
    cpuRam[addr & 0x07FF] = data;
  } else if (addr >= 0x2000 && addr <= 0x3FFF) {
    ppu.cpuWrite(addr & 0x0007, data);
  } else if (addr == 0x4014) {
    // A write to this address initiates a DMA transfer
    dma_page = data;
    dma_addr = 0x00;
    dma_transfer = true;
  } else if (addr >= 0x4016 && addr <= 0x4017) {

    // This & operation is to get which controller 0 or 1
    controller_state[addr & 0x0001] = controller[addr & 0x0001];
  }
}

uint8_t Bus::cpuRead(uint16_t addr, bool bReadOnly) {

  uint8_t data = 0x00;
  if (cart->cpuRead(addr, data)) {

  } else if (addr >= 0x0000 && addr <= 0x1FFF) {
    data = cpuRam[addr & 0x07FF];
  } else if (addr >= 0x2000 && addr <= 0x3FFF) {
    data = ppu.cpuRead(addr & 0x0007, bReadOnly);
  } else if (addr >= 0x4016 && addr <= 0x4017) {

    // Need to remember that clock != frame so this operation can be happened
    // several times. Controller in NES is PISO (Parallel In Serial Out). So 8
    // bit in but only 1 bit can be checked or processed. Therefore we check 1
    // bit for each control
    data = (controller_state[addr & 0x0001] & 0x80) > 0;
    controller_state[addr & 0x0001] <<= 1;
  }

  return data;
}

void Bus::reset() {
  cart->reset();
  cpu.reset();
  ppu.reset();
  nSystemClockCounter = 0;
  dma_page = 0x00;
  dma_addr = 0x00;
  dma_data = 0x00;
  dma_dummy = true;
  dma_transfer = false;
}

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge) {
  this->cart = cartridge;
  ppu.ConnectCartridge(cartridge);
}

void Bus::clock() {
  // Clocking. The heart and soul of an emulator. The running
  // frequency is controlled by whatever calls this function.
  // So here we "divide" the clock as necessary and call
  // the peripheral devices clock() function at the correct
  // times.

  // The fastest clock frequency the digital system cares
  // about is equivalent to the PPU clock. So the PPU is clocked
  // each time this function is called.
  ppu.clock();

  // The CPU runs 3 times slower than the PPU so we only call its
  // clock() function every 3 times this function is called. We
  // have a global counter to keep track of this.
  if (nSystemClockCounter % 3 == 0) {

    if (dma_transfer) {

      if (dma_dummy) {

        if (nSystemClockCounter % 2 == 1) {
          dma_dummy = false;
        }

      } else {
        if (nSystemClockCounter % 2 == 0) {
          // On even clock cycle, read from CPU Bus
          dma_data = cpuRead(dma_page << 8 | dma_addr);
        } else {
          // On odd clyce, write to PPU OAM
          ppu.pOAM[dma_addr] = dma_data;
          dma_addr++;

          if (dma_addr == 0x00) {
            dma_transfer = false;
            dma_dummy = true;
          }
        }
      }
    }

    else {
      cpu.clock();
    }
  }

  if (ppu.nmi) {
    ppu.nmi = false;
    cpu.nmi();
  }

  nSystemClockCounter++;
}
