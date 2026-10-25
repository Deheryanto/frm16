#include <iostream>
#include <fstream>
#include "memory/Memory.h"
#include "memory/VRAM.h"
#include "cpu/CPU.h"
#include "io/IO.h"
#include "ISA.h"

using namespace frm;
using namespace frm::isa;

// Declaration for the SDL display loop function
void run_display(VRAM& vram, CPU& cpu, IO& io);

int main(int argc, char* argv[]) {
      // Validate command-line arguments to ensure a binary file path is provided
      if (argc < 2) {
            std::cerr << "Usage: " << argv[0] << " <program.bin>\n";
            return 1;
      }

      // Initialize system components
      Memory mem;
      CPU cpu(mem);
      IO io(mem);

      // Open the compiled binary program file in binary mode
      std::ifstream inFile(argv[1], std::ios::binary);
      if (!inFile) {
            std::cerr << "Error: Could not open file " << argv[1] << "\n";
            return 1;
      }

      // Load program bytes sequentially into system memory starting from address 0x0000
      uint16_t loadAddr = 0x0000;
      char byteRead;
      while (inFile.get(byteRead)) {
            mem.write(loadAddr++, static_cast<uint8_t>(byteRead));
      }
      inFile.close();

      std::cout << "Program loaded successfully. Launching SDL display window...\n";
      
      // Hand off execution to the display and VM loop
      run_display(mem.getVRAM(), cpu, io);

      return 0;
}