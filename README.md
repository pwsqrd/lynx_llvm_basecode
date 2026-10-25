Atari lynx basecode using llvm compiler

## Prerequisites

- CMake 3.18+
- Ninja build system
- Python 3
- Clang/LLVM (for host compilation)
- **Linux only**: libc++ (required for toolchain build)
  - Arch/Manjaro: `sudo pacman -S libc++`
  - Ubuntu/Debian: `sudo apt install libc++-dev libc++abi-dev`
  - Fedora: `sudo dnf install libcxx libcxxabi`


## Setup

After cloning init submodules
```bash
git submodule update --init --recursive
```
Make the tools
make -f Makefile.toolchain

Build the lynx project (settings in `src/project.json`)
python3 tools/build.py

Build outputs in `out/`:
- `a.o` - Raw BLL binary (for ComLynx/development)
- `a.lnx` - LNX format (for emulators and flash carts)

To clean and rebuild:
```bash
python3 tools/build.py clean
python3 tools/build.py
```
