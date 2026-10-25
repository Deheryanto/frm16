#pragma once
#include "memory/Memory.h"
#include "ISA.h"

namespace frm {

/// @brief Represents the FRM processor core, handling execution cycles, registers, flags, and interrupts.
class CPU {
public:
      /// @brief Constructs a CPU instance linked to a system Memory object.
      /// @param memory Reference to the system memory.
      CPU(Memory& memory);
      
      /// @brief Resets the CPU state (registers, PC, SP, flags, and halted status).
      void reset();

      /// @brief Executes a single instruction fetch-decode-execute cycle.
      void step();

      /// @brief Checks whether the CPU is currently in a halted state.
      /// @return True if halted, false otherwise.
      bool isHalted() const { return halted; }

      // --- Register & Program Counter Accessors ---
      void     setPC(uint16_t addr) { pc = addr; }
      uint16_t getPC() const { return pc; }
      uint16_t getReg(uint8_t idx) const { return registers[idx & 0x7]; }

      // --- Interrupt & Timer Configuration ---
      void enableInterrupt(bool enable) { interruptEnabled = enable; }
      void setTimerReload(uint16_t reload) { timerReload = reload; }
      void setInterruptVector(uint16_t vec) { interruptVector = vec; }
      bool isInInterrupt() const { return inInterrupt; }

private:
      Memory& memory_; ///< Reference to system memory
      bool halted;     ///< Halt execution flag

      // --- Timer & Interrupt State ---
      uint16_t timerCounter     = 0;
      uint16_t timerReload      = 1000;
      bool     interruptEnabled = false;
      bool     inInterrupt      = false;
      uint16_t interruptVector  = 0x0100;

      // --- Processor Flags ---
      uint8_t flags = 0;
      void setFlags(uint16_t flag, uint16_t condition);
      void updateFlags(uint16_t result);

      // --- Registers & Architectural State ---
      uint16_t registers[8];   ///< General-purpose registers (R0-R7)
      uint16_t pc;             ///< Program Counter
      uint16_t sp = 0xEFFF;    ///< Stack Pointer

      // --- Core Execution Pipeline ---
      uint16_t fetch();
      void     execute(uint16_t instruction);
      static   isa::Decoded decode(uint16_t instruction);

      // --- Peripheral & Subsystem Helpers ---
      void     drawPixel(uint8_t x, uint8_t y, uint8_t color);
      void     clearScreen(uint8_t color);

      // --- Stack Operations ---
      void     push(uint16_t value);
      uint16_t pop();
};

} // namespace frm