#pragma once
#include <cstdint>
#include "memory/Memory.h"

namespace frm {

// --- I/O Memory-Mapped Constants for Gamepad Buttons ---
constexpr uint16_t IO_BUTTON_UP    = 0x00F0;
constexpr uint16_t IO_BUTTON_DOWN  = 0x00F1;
constexpr uint16_t IO_BUTTON_LEFT  = 0x00F2;
constexpr uint16_t IO_BUTTON_RIGHT = 0x00F3;

/// @brief Manages memory-mapped I/O devices (such as buttons and peripherals) for the FRM architecture.
class IO {
public:
      /// @brief Constructs an IO manager linked to a system Memory object.
      /// @param mem Reference to system memory.
      IO(Memory& mem);

      /// @brief Sets or clears the state of a specific button in memory.
      /// @param address Memory-mapped I/O address of the button.
      /// @param pressed True if the button is pressed, false otherwise.
      void setButton(uint16_t address, bool pressed);

private:
      Memory& memory_; ///< Reference to system memory
};

} // namespace frm