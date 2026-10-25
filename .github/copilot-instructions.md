# Copilot Instructions for Lynx LLVM Basecode

## Project Overview
This is a bare-metal development project for the Atari Lynx handheld console using the LLVM-MOS SDK. The codebase targets the 65C02 processor with a custom memory layout and BLL cartridge format, focusing on direct hardware manipulation without an operating system.

## Architecture
- **Startup Sequence**: `crt0.S` sets up the stack using registers `__rc0` and `__rc1`, then jumps to `main`.
- **Main Application**: `main.cpp` implements the core loop, interacting with hardware via memory-mapped I/O.
- **Hardware Interfaces**: `b.c` provides low-level functions like `readButton()` that read from fixed addresses (e.g., 0xfcb0).
- **Memory Layout**: `bll.ld` defines ROM (32K), code (0x200-0x1200), RAM (0x1000-0x2000), and generates a BLL header for cartridge execution.

Key hardware addresses:
- Green LED control: 0xfda0
- Red/Blue LED control: 0xfdb0
- Button input: 0xfcb0

## Build Workflow
Build using `make` with the LLVM-MOS SDK located at `../submodules/llvm-mos-sdk/`.

- `make`: Compiles `main.cpp` and `b.c` to object files, then links with `bll.ld` to produce `program.o`.
- `make clean`: Removes `.o` files and `program.o`.

Compilers: `mos-lynx-clang` (C) and `mos-lynx-clang++` (C++), with flags `-O2 -nostartfiles -nostdlib`.

## Conventions
- Hardware registers accessed via `volatile uint8_t *` pointers to specific addresses.
- No standard library; avoid `<iostream>`, `<string>`, etc.—use only `<stdint.h>` if needed.
- Prefer `.cpp` for application logic, `.c` for hardware-specific functions.
- Linker script customizes for BLL format with header bytes and section placement.

## Debugging and Testing
- Test on Atari Lynx emulator (e.g., Handy) or hardware.
- No automated tests; verify by observing LED changes and button responses in the main loop.

## Key Files
- [src/Makefile](src/Makefile): Build rules and compiler setup.
- [src/crt0.S](src/crt0.S): Assembly startup code.
- [src/main.cpp](src/main.cpp): Main loop with hardware interaction.
- [src/b.c](src/b.c): Hardware read functions.
- [src/bll.ld](src/bll.ld): Memory and section definitions.</content>
<parameter name="filePath">/home/muncher/Documents/development/lynx_llvm_basecode/.github/copilot-instructions.md