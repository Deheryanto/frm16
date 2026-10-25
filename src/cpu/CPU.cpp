#include <iostream>
#include <iomanip>
#include "CPU.h"
#include "ISA_Format.h"

namespace frm {

CPU::CPU(Memory& mem) : memory_(mem) {
      reset();
}

void CPU::reset() {
      for (int i = 0; i < 8; i++) {
            registers[i] = 0;
      }
      pc     = 0x0000;
      halted = false;
}

void CPU::setFlags(uint16_t a, uint16_t b) {
      flags = 0;
      if (a == b) flags |= 0x01; // Zero Flag
      if (a < b)  flags |= 0x02; // Less Than Flag
      if (a > b)  flags |= 0x04; // Greater Than Flag
}

void CPU::updateFlags(uint16_t result) {
      if (result == 0) {
            flags |= 0x01;  // Set Zero Flag
      } else {
            flags &= ~0x01; // Clear Zero Flag
      }
}

void CPU::push(uint16_t value) {
      memory_.write(sp, (value >> 8) & 0xFF);
      sp--;
      memory_.write(sp, value & 0xFF);
      sp--;
}

uint16_t CPU::pop() {
      sp++;
      uint8_t lo = memory_.read(sp);
      sp++;
      uint8_t hi = memory_.read(sp);
      return (hi << 8) | lo;
}

isa::Decoded CPU::decode(uint16_t instruction) {
      isa::Decoded d{};

      d.opcode = (instruction >> 8) & 0xFF;
      d.rd     = (instruction >> 5) & 0x07;
      d.rs     = (instruction >> 2) & 0x07;
      d.fn     = instruction & 0x03;
      d.imm5   = instruction & 0x1F;
      d.addr8  = instruction & 0xFF;
      d.x4     = (instruction >> 4) & 0x0F;
      d.y4     = instruction & 0x0F;
      d.format = isa::FORMAT_TABLE[d.opcode];
      
      return d;
}

uint16_t CPU::fetch() {
      uint16_t instruction = memory_.readWord(pc);
      pc += 2;
      return instruction;
}

void CPU::step() {
      if (halted) return;

      // Decrement timer counter if active
      if (timerCounter > 0) {
            timerCounter--;
      }

      // Handle periodic hardware interrupt trigger
      if (timerCounter == 0 && interruptEnabled && !inInterrupt) {
            std::cout << "INTERRUPT! PC=" << pc << " -> " << interruptVector << "\n";
            push(pc);
            pc = interruptVector;
            timerCounter = timerReload;
            inInterrupt = true;
      }

      uint16_t instruction = fetch();
      execute(instruction);
}

void CPU::drawPixel(uint8_t x, uint8_t y, uint8_t color) {
      if (x < 128 && y < 128) {
            int index     = y * 128 + x;
            uint16_t addr = Memory::VRAM_START + index;
            memory_.write(addr, color);
      }
}

void CPU::clearScreen(uint8_t color) {
      for (int y = 0; y < 128; ++y) {
            for (int x = 0; x < 128; ++x) {
                  drawPixel(x, y, color);
            }
      }
}

void CPU::execute(uint16_t instruction) {
      using namespace isa::Op;
      auto d = decode(instruction);

      switch (d.opcode) {
            // ============ Control & System ============
            case NOP: 
                  break;                    
            case HALT: 
                  halted = true; 
                  break;    
            case RESET: 
                  reset(); 
                  break;         
            case IRET:
                  pc = pop();
                  inInterrupt = false;
                  std::cout << "IRET: PC=" << pc << " R2=" << registers[2] << " R4=" << registers[4] << "\n";
                  break;

            // ============ ALU Register-Register (Format R) ============
            case ADD: 
                  registers[d.rd] += registers[d.rs]; 
                  updateFlags(registers[d.rd]); 
                  break;
            case SUB: 
                  registers[d.rd] -= registers[d.rs]; 
                  updateFlags(registers[d.rd]); 
                  break;
            case MUL: 
                  registers[d.rd] *= registers[d.rs]; 
                  break;
            case DIV: 
                  if (registers[d.rs]) registers[d.rd] /= registers[d.rs]; 
                  break;
            case MOD: 
                  if (registers[d.rs]) registers[d.rd] %= registers[d.rs]; 
                  break;
            case AND: 
                  registers[d.rd] &= registers[d.rs]; 
                  break;
            case OR:  
                  registers[d.rd] |= registers[d.rs]; 
                  break;
            case XOR: 
                  registers[d.rd] ^= registers[d.rs]; 
                  break;
            case SHL: 
                  registers[d.rd] <<= registers[d.rs]; 
                  break;
            case SHR: 
                  registers[d.rd] >>= registers[d.rs]; 
                  break;
            case CMP: 
                  setFlags(registers[d.rd], registers[d.rs]); 
                  break;
            case MOV: 
                  registers[d.rd] = registers[d.rs]; 
                  break;
            case NEG: 
                  registers[d.rd] = -registers[d.rd]; 
                  break;
            case NOT: 
                  registers[d.rd] = ~registers[d.rd]; 
                  break;
            case INC: 
                  registers[d.rd]++; 
                  break;
            case DEC: 
                  registers[d.rd]--; 
                  break;

            // ============ ALU Immediate (Format I) ============
            case LOADI: 
                  registers[d.rd] = d.imm5; 
                  break;
            case ADDI: 
                  registers[d.rd] += d.imm5; 
                  updateFlags(registers[d.rd]); 
                  break;
            case SUBI: 
                  registers[d.rd] -= d.imm5; 
                  updateFlags(registers[d.rd]); 
                  break;
            case ANDI:  
                  registers[d.rd] &= d.imm5; 
                  break;
            case ORI:   
                  registers[d.rd] |= d.imm5; 
                  break;
            case XORI:  
                  registers[d.rd] ^= d.imm5; 
                  break;
            case SHLI:  
                  registers[d.rd] <<= d.imm5; 
                  break;
            case SHRI:  
                  registers[d.rd] >>= d.imm5; 
                  break;
            case CMPI: 
                  setFlags(registers[d.rd], d.imm5); 
                  break;
            case MULI:  
                  registers[d.rd] *= d.imm5; 
                  break;
            case DIVI:  
                  if (d.imm5) registers[d.rd] /= d.imm5; 
                  break;
            case LOADI8: 
                  registers[7] = d.addr8;
                  break;

            // ============ Memory Load (Format M) ============
            case LOAD:  
                  registers[d.rd] = memory_.read(registers[d.rs]); 
                  break;
            case LOADW: 
                  registers[d.rd] = memory_.readWord(registers[d.rs]); 
                  break;
            case POP:   
                  registers[d.rd] = pop(); 
                  break;

            // ============ Memory Store (Format M) ============
            case STORE:  
                  memory_.write(registers[d.rs], registers[d.rd] & 0xFF); 
                  break;
            case STOREW: 
                  memory_.writeWord(registers[d.rs], registers[d.rd]); 
                  break;
            case PUSH:   
                  push(registers[d.rd]); 
                  break;

            // ============ Control Flow (Format J) ============
            case JMP: 
                  pc = d.addr8; 
                  break;
            case JZ:  
            case JE:  
                  if (flags & 0x01) pc = d.addr8; 
                  break;
            case JNZ: 
                  if (!(flags & 0x01)) pc = d.addr8; 
                  break;
            case JLT: 
                  if (flags & 0x02) pc = d.addr8; 
                  break;
            case JGT: 
                  if (flags & 0x04) pc = d.addr8; 
                  break;
            case CALL: 
                  push(pc); 
                  pc = d.addr8; 
                  break;
            case RET:  
                  pc = pop(); 
                  break;

            // ============ VRAM (Format X) ============
            case DRAW: 
                  drawPixel(d.x4, d.y4, registers[1]); 
                  break;
            case CLS:  
                  clearScreen(registers[1]); 
                  break;
            case DRAWR: 
                  drawPixel(registers[d.rd], registers[d.rs], registers[1]); 
                  break;
            case SETPIX: 
                  // TODO: Implement SETPIX
                  break;
            case GETPIX: 
                  // TODO: Implement GETPIX
                  break;

            default:
                  std::cerr << "Unknown opcode 0x" << std::hex << std::setw(2)
                            << std::setfill('0') << (int)d.opcode << "\n";
                  halted = true;
                  break;
      }

      // Ensure R0 remains hardwired to zero constant
      registers[isa::Reg::R0] = 0;
}

} // namespace frm