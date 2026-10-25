#pragma once
#include <cstddef>
#include <array>
#include <cstdint>

namespace frm {

/// @brief Manages Video RAM (VRAM), frame buffer storage, and pixel access for the FRM architecture.
class VRAM {
public:
      // --- Display & Resolution Constants ---
      static constexpr size_t WIDTH  = 128;            // Screen width in pixels
      static constexpr size_t HEIGHT = 128;            // Screen height in pixels
      static constexpr size_t SIZE   = WIDTH * HEIGHT; // Total VRAM size (16,384 bytes / 16 KB)
      
      /// @brief Constructs a VRAM instance and initializes the frame buffer.
      VRAM();

      /// @brief Reads a single pixel or byte from VRAM using a relative offset.
      /// @param offset Relative offset from the start of VRAM.
      /// @return The byte value read.
      uint8_t read(uint16_t offset) const;

      /// @brief Writes a single byte to VRAM using a relative offset.
      /// @param offset Relative offset from the start of VRAM.
      /// @param value The byte value to write.
      void    write(uint16_t offset, uint8_t value);

      /// @brief Clears the entire VRAM frame buffer (resets all pixels to 0).
      void    clear();

      /// @brief Retrieves the color value of a specific pixel by its 2D coordinates.
      /// @param x X-coordinate (0 to WIDTH - 1).
      /// @param y Y-coordinate (0 to HEIGHT - 1).
      /// @return The color/byte value at the specified coordinate.
      uint8_t pixel(int x, int y) const;

private:
      std::array<uint8_t, SIZE> vram_; ///< Linear frame buffer array for pixel data
};

} // namespace frm