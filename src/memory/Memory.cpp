#include "Memory.h"

namespace frm {

Memory::Memory() {
      // Initialize all memory contents to zero
      memory_.fill(0);
}

uint8_t Memory::read(uint16_t address) const {
      // Route read requests to VRAM if the address falls within the VRAM range
      if (readVRAM(address)) {
            return vram_.read(address - VRAM_START);
      }
      // Return 0 for addresses beyond the RAM size boundary
      if (address >= RAM_SIZE) return 0;
      
      return memory_[address];
}

void Memory::write(uint16_t address, uint8_t value) {
      // Route write requests to VRAM if the address falls within the VRAM range
      if (readVRAM(address)) {
            vram_.write(address - VRAM_START, value);
            return;
      }
      // Ignore writes to addresses beyond the RAM size boundary
      if (address >= RAM_SIZE) return;
      
      memory_[address] = value;
}

uint16_t Memory::readWord(uint16_t address) const {
      // Read 16-bit word in little-endian format (low byte first, high byte second)
      uint16_t lo = read(address);
      uint8_t hi = read(static_cast<uint16_t>(static_cast<uint32_t>(address) + 1));
      return lo | (hi << 8);
}

void Memory::writeWord(uint16_t address, uint16_t value) {
      // Write 16-bit word in little-endian format (low byte first, high byte second)
      write(address, value & 0xFF);
      write(static_cast<uint16_t>(static_cast<uint32_t>(address) + 1), value >> 8);
}

} // namespace frm