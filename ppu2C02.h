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
  uint8_t tblPalette[32];

  // This should be inside of the cartridge instead of the nes. For now this
  // wont be used.
  uint8_t tblPattern[2][4096];

public:
  uint8_t tblName[2][1024]; // "VRAM"

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
  void reset();
  bool nmi = false;

private:
  int16_t scanline = 0;
  int16_t cycle = 0;

  union {
    struct {
      uint8_t unused : 5;
      uint8_t sprite_overflow : 1;
      uint8_t sprite_zero_hit : 1;
      uint8_t vertical_blank : 1;
    };
    uint8_t reg;
  } status;

  union {
    struct {
      uint8_t grayscale : 1;
      uint8_t render_background_left : 1;
      uint8_t render_sprites_left : 1;
      uint8_t render_background : 1;
      uint8_t render_sprites : 1;
      uint8_t enhance_red : 1;
      uint8_t enhance_green : 1;
      uint8_t enhance_blue : 1;
    };

    uint8_t reg;
  } mask;

  union PPUCTRL {
    struct {
      uint8_t nametable_x : 1;
      uint8_t nametable_y : 1;
      uint8_t increment_mode : 1;
      uint8_t pattern_sprite : 1;
      uint8_t pattern_background : 1;
      uint8_t sprite_size : 1;
      uint8_t slave_mode : 1; // unused
      uint8_t enable_nmi : 1;
    };

    uint8_t reg;
  } control;

  union loopy_register {
    // Credit to Loopy for working this out :D
    struct {

      // Coarse is for tile -> Each tile contain 8 pixel
      // Fine is for pixel
      uint16_t coarse_x : 5;
      uint16_t coarse_y : 5;
      uint16_t nametable_x : 1;
      uint16_t nametable_y : 1;
      uint16_t fine_y : 3;
      uint16_t unused : 1;
    };

    uint16_t reg = 0x0000;
  };

  loopy_register vram_addr; // Active "pointer" address into nametable to
                            // extract background tile info
  loopy_register tram_addr; // Temporary store of information to be
                            // "transferred" into "pointer" at various time

  uint8_t ppu_data_buffer = 0x00;
  uint8_t fine_x = 0x00;

  uint8_t address_latch = 0x00;

  // Background rendering
  uint8_t bg_next_tile_id = 0x00;
  uint8_t bg_next_tile_attrib = 0x00;
  uint8_t bg_next_tile_lsb = 0x00;
  uint8_t bg_next_tile_msb = 0x00;
  uint16_t bg_shifter_pattern_lo = 0x0000;
  uint16_t bg_shifter_pattern_hi = 0x0000;
  uint16_t bg_shifter_attrib_lo = 0x0000;
  uint16_t bg_shifter_attrib_hi = 0x0000;

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
  olc::Sprite &GetPatternTable(uint8_t i, uint8_t palette);
  olc::Pixel &GetColourFromPaletteRam(uint8_t palette, uint8_t pixel);
  bool frame_complete = false;

private:
  // Primary OAM = 256 bytes = 64 sprites x 4 bytes.
  // Sprite n occupies OAM[n*4 + 0..3].
  //
  // Byte 0 "y"      : yyyy yyyy  top edge on screen (hardware draws at y+1)
  // Byte 1 "id"     : tttt tttt  tile index into pattern table
  // Byte 2 "attribute":
  //
  //      7   6   5   4   3   2   1   0
  //    +---+---+---+---+---+---+---+---+
  //    | V | H | P | - | - | - | p | p |
  //    +---+---+---+---+---+---+---+---+
  //
  //   bits 1-0  pp : sprite palette 0-3 -> PPU $3F10 + pp*4 (colors
  //   $3F11-$3F1F)
  //   bits 4-2  -  : unused, PPU ignores / reads back 0
  //   bit  5    P  : priority, 0 = in front of BG, 1 = behind BG
  //   bit  6    H  : horizontal flip
  //   bit  7    V  : vertical flip
  struct sObjectAttributeEntry {
    uint8_t y;
    uint8_t id;
    uint8_t attribute;
    uint8_t x;
  } OAM[64];

  uint8_t oam_addr = 0x00;

  // This is basically sOAM
  sObjectAttributeEntry spriteScanline[8];
  uint8_t sprite_count = 0;

  uint8_t sprite_shifter_pattern_lo[8];
  uint8_t sprite_shifter_pattern_hi[8];

public:
  uint8_t *pOAM = (uint8_t *)OAM;
};
