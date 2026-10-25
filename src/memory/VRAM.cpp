#include "VRAM.h"

namespace frm {

VRAM::VRAM() {
      // Initialize the entire frame buffer to zero (blank/black screen)
      vram_.fill(0);
}

uint8_t VRAM::read(uint16_t offset) const {
      // Return 0 if the offset exceeds the VRAM frame buffer size limit
      if (offset >= SIZE) {
            return 0; 
      }
      return vram_[offset];
}

void VRAM::write(uint16_t offset, uint8_t value) {
      // Write the byte value only if the offset falls within the valid VRAM range
      if (offset < SIZE) {
            vram_[offset] = value;
      }
}

void VRAM::clear() {
      // Reset all pixels in the frame buffer back to zero
      vram_.fill(0);
}

uint8_t VRAM::pixel(int x, int y) const {
      // Return 0 if the requested coordinates are out of screen bounds
      if (x < 0 || x >= static_cast<int>(WIDTH) || y < 0 || y >= static_cast<int>(HEIGHT)) {
            return 0;
      }
      // Compute 1D linear index from 2D coordinates (row-major order: y * WIDTH + x)
      return vram_[y * WIDTH + x];
}

} // namespace frm