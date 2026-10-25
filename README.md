Atari lynx basecode using llvm compiler

## Setup

Requires Python 3 and git. Everything else is installed by the setup script.

```bash
git clone https://github.com/pwsqrd/lynx_llvm_basecode.git
cd lynx_llvm_basecode
python3 setup.py
```

`setup.py` installs the host prerequisites (cmake, ninja, clang, libc++,
OpenSSL, make, gcc), fetches the submodules and builds the toolchain. The
first run builds LLVM and takes a long time. Re-running it skips finished steps.

Supported hosts: Linux only for now (Arch/Manjaro/CachyOS, Debian/Ubuntu,
Fedora). On Windows, use WSL2 with Ubuntu. macOS is not supported yet. On
other Linux distros, install the prerequisites yourself and run
`python3 setup.py --no-install`.

## Building

Build the lynx project (settings in `src/project.json`)
```bash
python3 tools/build.py
```

Build outputs in `out/`:
- `a.o` - Raw BLL binary (for ComLynx/development)
- `a.lnx` - LNX format (for emulators and flash carts)

To clean and rebuild:
```bash
python3 tools/build.py clean
python3 tools/build.py
```
