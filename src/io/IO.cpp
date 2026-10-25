#include "IO.h"

namespace frm {

IO::IO(Memory& mem) : memory_(mem) {}

void IO::setButton(uint16_t address, bool pressed) {
      // Write 1 if the button is pressed, 0 otherwise to the memory-mapped address
      memory_.write(address, pressed ? 1 : 0);
}

} // namespace frm