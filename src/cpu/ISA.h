#pragma once
#include <cstdint>

namespace frm {
namespace isa {

/// @brief Instruction format types for the FRM architecture.
enum class Format : uint8_t {
      R,    // Register-Register
      I,    // Immediate 5-bit
      M,    // Memory indirect (via register)
      J,    // Jump 8-bit
      X,    // XY coordinates (VRAM)
      NONE  // NOP, HALT
};

// ------------------------------------------------------------
// OPCODE 8-BIT (0x00 - 0xFF)
// ------------------------------------------------------------
namespace Op {
      // ===== 0x00 - 0x0F: Control & System =====
      constexpr uint8_t NOP    = 0x00;
      constexpr uint8_t HALT   = 0x01;
      constexpr uint8_t RESET  = 0x02;
      constexpr uint8_t INT    = 0x03;  // Reserved for interrupts
      constexpr uint8_t IRET   = 0x04;  // Reserved for interrupt return
      constexpr uint8_t LOADI8 = 0x05;  // Load 8-bit immediate

      // ===== 0x10 - 0x1F: ALU Register-Register (Format R) =====
      constexpr uint8_t ADD    = 0x10;
      constexpr uint8_t SUB    = 0x11;
      constexpr uint8_t MUL    = 0x12;
      constexpr uint8_t DIV    = 0x13;
      constexpr uint8_t MOD    = 0x14;
      constexpr uint8_t AND    = 0x15;
      constexpr uint8_t OR     = 0x16;
      constexpr uint8_t XOR    = 0x17;
      constexpr uint8_t SHL    = 0x18;
      constexpr uint8_t SHR    = 0x19;
      constexpr uint8_t CMP    = 0x1A;
      constexpr uint8_t MOV    = 0x1B;
      constexpr uint8_t NEG    = 0x1C;
      constexpr uint8_t NOT    = 0x1D;
      constexpr uint8_t INC    = 0x1E;
      constexpr uint8_t DEC    = 0x1F;

      // ===== 0x20 - 0x2F: ALU Immediate (Format I) =====
      constexpr uint8_t LOADI  = 0x20;
      constexpr uint8_t ADDI   = 0x21;
      constexpr uint8_t SUBI   = 0x22;
      constexpr uint8_t ANDI   = 0x23;
      constexpr uint8_t ORI    = 0x24;
      constexpr uint8_t XORI   = 0x25;
      constexpr uint8_t SHLI   = 0x26;
      constexpr uint8_t SHRI   = 0x27;
      constexpr uint8_t CMPI   = 0x28;
      constexpr uint8_t MULI   = 0x29;
      constexpr uint8_t DIVI   = 0x2A;
      // 0x2B - 0x2F: Reserved

      // ===== 0x30 - 0x37: Memory Load (Format M) =====
      constexpr uint8_t LOAD   = 0x30;  // Rd = memory[Rs] (byte)
      constexpr uint8_t LOADW  = 0x31;  // Rd = memory[Rs] (word)
      constexpr uint8_t POP    = 0x32;  // Rd = stack.pop()
      constexpr uint8_t LOADR  = 0x33;  // Reserved
      // 0x34 - 0x37: Reserved

      // ===== 0x38 - 0x3F: Memory Store (Format M) =====
      constexpr uint8_t STORE  = 0x38;  // memory[Rs] = Rd (byte)
      constexpr uint8_t STOREW = 0x39;  // memory[Rs] = Rd (word)
      constexpr uint8_t PUSH   = 0x3A;  // stack.push(Rd)
      constexpr uint8_t STORER = 0x3B;  // Reserved
      // 0x3C - 0x3F: Reserved

      // ===== 0x40 - 0x4F: Control Flow (Format J) =====
      constexpr uint8_t JMP    = 0x40;  // pc = addr8
      constexpr uint8_t JZ     = 0x41;  // if Rd == 0: pc = addr8
      constexpr uint8_t JNZ    = 0x42;  // if Rd != 0: pc = addr8
      constexpr uint8_t JC     = 0x43;  // if carry:   pc = addr8
      constexpr uint8_t JNC    = 0x44;  // if !carry:  pc = addr8
      constexpr uint8_t CALL   = 0x45;  // push(pc); pc = addr8
      constexpr uint8_t RET    = 0x46;  // pc = pop()
      constexpr uint8_t JLT    = 0x47;  // if Rd < 0:  pc = addr8
      constexpr uint8_t JGT    = 0x48;  // if Rd > 0:  pc = addr8
      constexpr uint8_t JE     = 0x49;  // if Rd == flag: pc = addr8
      // 0x4A - 0x4F: Reserved

      // ===== 0x50 - 0x5F: I/O & VRAM =====
      constexpr uint8_t DRAW   = 0x50;  // VRAM[x][y] = color (Format X)
      constexpr uint8_t CLS    = 0x51;  // Clear screen
      constexpr uint8_t SETPIX = 0x52;  // Set pixel (Format X)
      constexpr uint8_t GETPIX = 0x53;  // Get pixel (Format X)
      constexpr uint8_t SPRITE = 0x54;  // Draw sprite
      constexpr uint8_t SCROLL = 0x55;  // Scroll screen
      constexpr uint8_t PALET  = 0x56;  // Set palette
      constexpr uint8_t DRAWR  = 0x57;  // Draw pixel from registers
      // 0x58 - 0x5F: Reserved

      // ===== 0x60 - 0x6F: Stack & Extended =====
      constexpr uint8_t PUSHA  = 0x60;  // Push all registers
      constexpr uint8_t POPA   = 0x61;  // Pop all registers
      constexpr uint8_t SWAP   = 0x62;  // Swap Rd, Rs
      // 0x63 - 0x6F: Reserved

      // ===== 0x70 - 0x7F: Reserved (Future: timer, sound, input) =====
      // ===== 0x80 - 0xFF: Reserved (User extensions) =====
}

namespace Reg {
      constexpr uint8_t R0 = 0;  // Zero register (hardwired to 0)
      constexpr uint8_t R1 = 1;
      constexpr uint8_t R2 = 2;
      constexpr uint8_t R3 = 3;
      constexpr uint8_t R4 = 4;
      constexpr uint8_t R5 = 5;
      constexpr uint8_t R6 = 6;
      constexpr uint8_t R7 = 7;
}

namespace encode {

/// @brief Format R: [Opcode 8] [Rd 3] [Rs 3] [Fn 2]
inline uint16_t R(uint8_t opcode, uint8_t rd, uint8_t rs, uint8_t fn = 0) {
      return (static_cast<uint16_t>(opcode) << 8)
           | (static_cast<uint16_t>(rd & 0x7) << 5)
           | (static_cast<uint16_t>(rs & 0x7) << 2)
           | (fn & 0x3);
}

/// @brief Format I: [Opcode 8] [Rd 3] [Imm 5]
inline uint16_t I(uint8_t op, uint8_t rd, uint8_t imm5) {
      return (static_cast<uint16_t>(op) << 8)
           | (static_cast<uint16_t>(rd & 0x7) << 5)
           | (imm5 & 0x1F);
}

/// @brief Format M: [Opcode 8] [Rd 3] [Rs 3] [Fn 2] (Memory indirect via register)
inline uint16_t M(uint8_t op, uint8_t rd, uint8_t rs) {
      return (static_cast<uint16_t>(op) << 8)
           | (static_cast<uint16_t>(rd & 0x7) << 5)
           | (static_cast<uint16_t>(rs & 0x7) << 2);
}

/// @brief Format J: [Opcode 8] [Addr 8]
inline uint16_t J(uint8_t op, uint8_t addr8) {
      return (static_cast<uint16_t>(op) << 8) | addr8;
}

/// @brief Format X: [Opcode 8] [X 4] [Y 4]
inline uint16_t X(uint8_t op, uint8_t x4, uint8_t y4) {
      return (static_cast<uint16_t>(op) << 8)
           | (static_cast<uint16_t>(x4 & 0xF) << 4)
           | (y4 & 0xF);
}

/// @brief Format I8: [Opcode 8] [Imm 8]
inline uint16_t I8(uint8_t op, uint8_t imm8) {
      return (static_cast<uint16_t>(op) << 8) | imm8;
}

/// @brief Format NONE: [Opcode 8] [Padding 8]
inline uint16_t N(uint8_t op) {
      return static_cast<uint16_t>(op) << 8;
}

} // namespace encode

/// @brief Structure representing a decoded instruction.
struct Decoded {
      uint8_t opcode;
      uint8_t rd;
      uint8_t rs;
      uint8_t fn;
      uint8_t imm5;
      uint8_t addr8;
      uint8_t x4;
      uint8_t y4;
      Format  format;
};

} // namespace isa
} // namespace frm