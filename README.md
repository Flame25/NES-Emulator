# NES Emulator

A Nintendo Entertainment System (NES) emulator written in C++, developed as a learning project by following **OneLoneCoder's NES Emulator From Scratch tutorial series**.

The project explores how the NES works internally, from executing 6502 CPU instructions to rendering backgrounds and sprites through the 2C02 Picture Processing Unit (PPU).

## Overview

The NES is a classic 8-bit gaming console with a CPU, a dedicated graphics processor, and cartridge hardware. Emulating it involves reproducing the behavior and interactions of these components in software.

This project follows OneLoneCoder's tutorial to learn about hardware emulation, CPU instruction execution, memory mapping, graphics rendering, and the timing interactions between the CPU and PPU.

## Features

### CPU — 6502

- CPU registers and processor status flags.
- Instruction decoding and execution.
- Multiple addressing modes.
- Instruction cycle tracking.
- Memory access through the system bus.
- Implementation of the NES's 6502-derived CPU architecture.

### PPU — 2C02

- PPU registers and memory access.
- Background rendering.
- Sprite rendering using Object Attribute Memory (OAM).
- Pattern table and tile decoding.
- Sprite attributes, including palette selection and flipping.
- Background scrolling through PPU scroll registers.
- Sprite 0 Hit detection and status flag handling.
- Scanline- and cycle-based rendering logic.

### Cartridge and Memory Mapping

- iNES ROM loading.
- Cartridge memory mapping.
- Mapper architecture.
- Mapper 000 (NROM) support.

*Features are under development. Full hardware accuracy and compatibility with commercial games are not guaranteed.*

## Demo

[![NES Emulator Demo](https://img.youtube.com/vi/hQ70zf71dVY/maxresdefault.jpg)](https://www.youtube.com/watch?v=hQ70zf71dVY)

*Click the thumbnail to watch the demo on YouTube.*

## Technologies

- **Language:** C++
- **Graphics and input:** [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine)
- **CPU architecture:** MOS 6502-derived processor
- **Graphics hardware:** Ricoh 2C02 PPU
- **ROM format:** iNES

## Learning Objectives

This project serves as a practical exercise in low-level programming and hardware emulation.

The main learning objectives include:

- Understanding CPU registers, instructions, addressing modes, and status flags.
- Implementing instruction execution and cycle tracking.
- Modeling a memory bus and cartridge mapping.
- Understanding PPU registers and memory access.
- Learning how tile-based graphics and bitplanes represent pixels.
- Implementing background and sprite rendering.
- Understanding sprite evaluation, OAM, and sprite attributes.
- Exploring scanlines, PPU cycles, and rendering timing.
- Understanding hardware behavior such as Sprite 0 Hit and its role in raster synchronization.

## Acknowledgments

Special thanks to **[OneLoneCoder (Javidx9)](https://github.com/OneLoneCoder)** for creating the *NES Emulator From Scratch* tutorial series and providing the accompanying [olcNES source code](https://github.com/OneLoneCoder/olcNES).

This project follows his tutorial as a learning exercise. The tutorial and its accompanying source code are the primary references for the implementation.

The project also uses [olcPixelGameEngine](https://github.com/OneLoneCoder/olcPixelGameEngine) for graphics and input.

All credit for the original tutorial and its associated materials belongs to their respective creator. Applicable license terms are preserved for any code or resources derived from the original project.
