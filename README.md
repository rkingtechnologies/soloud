# SoLoud – Modern CMake Fork (Miniaudio Backend Only)

**SoLoud** is a lightweight, easy-to-use, portable **C/C++ audio engine** designed for games.

![SoLoud Screenshot](https://raw.githubusercontent.com/jarikomppa/soloud/master/soloud.png)

This fork modernizes the original SoLoud library with:

- Clean, **modern CMake** build system (no autotools, no custom scripts)
- **Modular CMake link libraries** (`soloud::core`, `soloud::audiosource`, and `soloud::filter`)
- **Miniaudio backend only** — the cleanest, most portable, single-file audio backend
- Optional demo builds (can be completely disabled/removed)
- Simplified directory structure

SoLoud remains fully **Zlib/libpng licensed** — free for any use, including commercial projects.

Original project & full documentation: [soloud-audio.com](http://soloud-audio.com)

## Why this fork?

| Feature                       | Original SoLoud          | This Fork                      |
|-------------------------------|--------------------------|------------------------------- |
| Build system                  | Custom + autotools       | Modern CMake                   |
| Backend support               | Many (SDL, WASAPI, etc.) | Miniaudio only                 |
| Size / dependencies           | Larger with backends     | Smaller, minimal external deps |
| Demos                         | Broken                   | Fully supported with a clean ImGui implementation     |
| Maintenance focus             | Feature-complete         | Build & integration quality    |

Miniaudio was chosen because it's:
- Single-header, no external dependencies
- Excellent cross-platform support (Windows, macOS, Linux, Android, iOS, Web, etc.)
- Very small footprint
- Actively maintained (as of 2026)

## Badges

[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg?style=flat-square)](https://opensource.org/licenses/Zlib)
[![C++23](https://img.shields.io/badge/c%2B%2B-23-blue.svg?style=flat-square)](https://en.cppreference.com/w/cpp/23)
[![CMake 4.2+](https://img.shields.io/badge/CMake-4.2+-blue.svg?style=flat-square)](https://cmake.org)

## Quick Integration (Recommended)

```cmake
# In your CMakeLists.txt
add_subdirectory(external/soloud)   # or wherever you put this repo

target_link_libraries(your_game
    PRIVATE
        soloud::core
        soloud::audiosource     # Wav, Speech, Noise, etc.
        soloud::filter          # Optional: reverb, echo, etc.
)

# optionally disable building the demos
set(SOLOUD_BUILD_DEMOS OFF CACHE BOOL "Disable SoLoud demos" FORCE)

```

