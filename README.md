# Tiny Parser

## Overview

Tiny Parser is a simple parser for tiny language that displays the parse tree of the input code.

## Demo

![demo](https://github.com/user-attachments/assets/76ba1cf4-c8c2-483c-934e-ed950f13f082)

## Setup Development Environment

### Linux

#### Ubuntu and Debian-based systems

1. Install the necessary development tools:
   ```sh
   sudo apt-get update
   sudo apt-get install build-essential cmake graphviz
   ```

#### Arch-based systems

1. Install the necessary development tools:
   ```sh
   sudo pacman -Syu base-devel cmake graphviz
   ```

### Windows

1. Open PowerShell and paste the following to install the necessary tools:
   ```sh
   winget install MSYS2.MSYS2 Kitware.CMake Graphviz.Graphviz
   ```
2. Open the MSYS2 UCRT64 shell and install the necessary packages:
   ```sh
   pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
   ```
