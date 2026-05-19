# BaseX

A lightweight, cross-platform CLI tool for converting numbers between Binary, Decimal, and Hexadecimal formats. It supports both integer and floating-point bit-pattern conversions.

## Features

- **Binary to Decimal/Hex**
- **Decimal to Binary/Hex**
- **Hexadecimal to Binary/Decimal**
- **Floating-point support**: View the bit-patterns of 32-bit `float` and 64-bit `double` from their hex representations.
- **Cross-platform**: Supports macOS, Linux, and Windows.
- **Gradient UI**: Beautiful ANSI shadow gradient banners.

## Installation

Download the latest version for your operating system from the [Releases](https://github.com/LuckiBit/basex/releases) page.

### Manual Build

If you prefer to build from source, ensure you have `CMake` and a C compiler (like `gcc`, `clang`, or `msvc`) installed.

```bash
mkdir build
cmake -B build
cmake --build build
```

The executable will be located in `build/bin/`.

## Usage

Run the executable and follow the interactive menu:

```bash
./basex
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
