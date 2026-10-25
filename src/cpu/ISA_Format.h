#pragma once
#include "ISA.h"

namespace frm {
namespace isa {

// --- Instruction Format Lookup Table ---
// Maps each 8-bit opcode (0x00 - 0xFF) to its corresponding instruction Format type.
inline constexpr Format FORMAT_TABLE[256] = {
      // ===== 0x00 - 0x0F: Control & System =====
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,
      Format::NONE,

      // ===== 0x10 - 0x1F: ALU Register-Register =====
      Format::R, Format::R, Format::R, Format::R, Format::R,
      Format::R, Format::R, Format::R, Format::R, Format::R,
      Format::R, Format::R, Format::R, Format::R, Format::R, Format::R,

      // ===== 0x20 - 0x2F: ALU Immediate =====
      Format::I, Format::I, Format::I, Format::I, Format::I,
      Format::I, Format::I, Format::I, Format::I, Format::I,
      Format::I, Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,

      // ===== 0x30 - 0x37: Memory Load =====
      Format::M, Format::M, Format::M, Format::M, Format::NONE,
      Format::NONE, Format::NONE, Format::NONE,

      // ===== 0x38 - 0x3F: Memory Store =====
      Format::M, Format::M, Format::M, Format::M, Format::NONE,
      Format::NONE, Format::NONE, Format::NONE,

      // ===== 0x40 - 0x4F: Control Flow =====
      Format::J, Format::J, Format::J, Format::J, Format::J,
      Format::J, Format::NONE, Format::J, Format::J, Format::J,
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,

      // ===== 0x50 - 0x5F: VRAM & I/O =====
      Format::X, Format::NONE, Format::X, Format::X, Format::X,
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,

      // ===== 0x60 - 0x6F: Stack & Extended =====
      Format::NONE, Format::NONE, Format::R, Format::NONE, Format::NONE,
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,
      Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE, Format::NONE,

      // ===== 0x70 - 0xFF: Reserved Range (Defaults to Format::NONE) =====
};

} // namespace isa
} // namespace frm