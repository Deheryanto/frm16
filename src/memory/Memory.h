#pragma once
#include <cstdint>
#include <array>
#include <cstddef>
#include "VRAM.h"

namespace frm {

/// @brief Manages system memory mapping, RAM, and video RAM (VRAM) for the FRM architecture.
class Memory {
public:
      // --- Memory Layout & Sizing Constants ---
      static constexpr size_t RAM_SIZE      = 49152;                // 48 KB RAM
      static constexpr size_t VRAM_SIZE     = 16384;                // 16 KB VRAM
      static constexpr size_t MEMORY_SIZE   = RAM_SIZE + VRAM_SIZE; // Total 64 KB Address Space

      // --- Address Boundary Constants ---
      static constexpr uint16_t VRAM_START  = 0xC000;
      static constexpr uint16_t VRAM_END    = 0xFFFF;

      /// @brief Constructs a Memory instance and initializes its storage.
      Memory();

      /// @brief Reads a single byte from the specified memory address.
      /// @param address 16-bit memory address to read from.
      /// @return The byte value read.
      uint8_t read(uint16_t address) const;

      /// @brief Writes a single byte to the specified memory address.
      /// @param address 16-bit memory address to write to.
      /// @param value The byte value to write.
      void    write(uint16_t address, uint8_t value);

      /// @brief Reads a 16-bit word (two bytes) from the specified memory address.
      /// @param address 16-bit base memory address.
      /// @return The 16-bit word read.
      uint16_t readWord(uint16_t address) const;

      /// @brief Writes a 16-bit word (two bytes) to the specified memory address.
      /// @param address 16-bit base memory address.
      /// @param value The 16-bit word value to write.
      void     writeWord(uint16_t address, uint16_t value);

      /// @brief Retrieves a reference to the underlying VRAM subsystem.
      /// @return Reference to the VRAM object.
      VRAM&    getVRAM() { return vram_; }

private:
      std::array<uint8_t, MEMORY_SIZE> memory_; ///< Linear storage buffer for the 64 KB address space
      VRAM vram_;                               ///< Video RAM management instance

      /// @brief Checks whether a given address falls within the VRAM region.
      /// @param address 16-bit memory address to check.
      /// @return True if the address is in VRAM range, false otherwise.
      bool readVRAM(uint16_t address) const {
            return address >= VRAM_START && address <= VRAM_END;
      }
};

} // namespace frm