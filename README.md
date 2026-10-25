# 🚀 FRM16 — A 16-bit RISC Virtual Machine

> A custom 16-bit RISC virtual machine, assembler, and game console built entirely from scratch in modern C++.

![Status](https://img.shields.io/badge/status-active-success.svg)
![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

---

## 📖 Overview

**FRM16** (Fast RISC Machine 16-bit) is a fully functional virtual machine designed to teach and demonstrate the core principles of computer architecture, low-level systems programming, and game development.

This project is not just a CPU emulator — it is an entire ecosystem built from zero:
- A custom **Instruction Set Architecture (ISA)**
- A hand-written **CPU** with an ALU, Flags, Stack, and Interrupts
- A **Memory Bus** with Memory-Mapped I/O
- A **Two-Pass Assembler** with label support
- A **Graphics Unit** (VRAM + SDL2 renderer)
- Playable **games** written in pure Assembly

---

## ✨ Features

### 🧠 CPU Core
- **16-bit** architecture with an **8-bit opcode** (up to 256 instructions)
- **Multi-format instruction encoding** (R, I, M, J, X)
- Full **ALU** (ADD, SUB, MUL, DIV, AND, OR, XOR, SHL, SHR, CMP, MOV, NEG, NOT, INC, DEC)
- **Flag system** (Zero, Less, Greater) for conditional branching
- **Stack** support (PUSH, POP, CALL, RET)
- **Timer & Interrupts** (hardware-level IRQ with `IRET`)

### 💾 Memory
- **64 KB** address space
- **48 KB RAM** for program & data
- **16 KB VRAM** (128×128 pixel framebuffer) mapped at `0xC000`
- **Memory-Mapped I/O** for keyboard input at `0xF0`–`0xF3`
- Safe bounds checking to prevent out-of-bounds access

### 🎨 Graphics
- **128×128 pixels** framebuffer
- **RGB24 palette** with 7 preset colors (White, Red, Green, Blue, Yellow, Magenta, Cyan)
- Hardware-accelerated rendering via **SDL2**
- 4× scaling for a comfortable 512×512 window

### 🛠️ Toolchain
- **Two-Pass Assembler** (`frmas`)
  - Label support (`LOOP:`, `FUNCTION:`)
  - Comments (`;`)
  - Register and immediate parsing
  - 8-bit immediate loading via `LOADI8` (uses `R7`)
- **VM Runner** (`frmrun`)
  - Loads `.bin` files into memory
  - Initializes SDL2 display
  - Runs CPU at configurable speed

---

## 🚀 Quick Start

### Prerequisites
- **CMake** ≥ 3.16
- **C++17** compiler (GCC, Clang, MSVC)
- **SDL2** development libraries

```bash
# Ubuntu / Debian
sudo apt install build-essential cmake libsdl2-dev

# macOS
brew install cmake sdl2

# Arch Linux
sudo pacman -S base-devel cmake sdl2