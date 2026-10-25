#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include "ISA.h"

using namespace frm::isa;

/// @brief Splits an assembly line into separate token arguments separated by commas, spaces, or tabs.
/// @param line The raw assembly instruction string.
/// @return A vector of parsed argument strings.
std::vector<std::string> splitArgs(const std::string& line) {
      std::vector<std::string> args;
      std::string current;
      for (char c : line) {
            if (c == ',' || c == ' ' || c == '\t') {
                  if (!current.empty()) {
                        args.push_back(current);
                        current.clear();
                  }
            } else {
                  current += c;
            }
      }
      if (!current.empty()) {
            args.push_back(current);
      }
      return args;
}

/// @brief Parses a register string representation (e.g., "R1" or "r3") into its index value.
/// @param str The string containing the register identifier.
/// @return The 8-bit register index.
uint8_t parseRegister(const std::string& str) {
      if (str.empty() || (str[0] != 'R' && str[0] != 'r')) {
            throw std::runtime_error("Invalid register identifier: " + str);
      }
      return static_cast<uint8_t>(std::stoi(str.substr(1)));
}

int main(int argc, char* argv[]) {
      // Validate command-line arguments to ensure both input and output paths are provided
      if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <input.asm> <output.bin>\n";
            return 1;
      }

      // Open the input assembly source file
      std::ifstream inFile(argv[1]);
      if (!inFile) {
            std::cerr << "Error: Could not open input file " << argv[1] << "\n";
            return 1;
      }

      // --- Pass 1: Read and Clean Lines ---
      std::vector<std::string> lines;
      std::string line;
      while (std::getline(inFile, line)) {
            // Strip comments starting with ';'
            size_t commentPos = line.find(';');
            if (commentPos != std::string::npos) {
                  line = line.substr(0, commentPos);
            }
            
            // Trim leading and trailing whitespace
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);
            
            if (!line.empty()) {
                  lines.push_back(line);
            }
      }
      inFile.close();

      // --- Pass 2: Label Resolution and Address Tracking ---
      std::map<std::string, uint16_t> labelAddresses;
      uint16_t currentAddr = 0x0000;

      for (const std::string& l : lines) {
            if (!l.empty() && l.back() == ':') {
                  std::string labelName = l.substr(0, l.length() - 1);
                  labelName.erase(std::remove_if(labelName.begin(), labelName.end(), ::isspace), labelName.end());
                  labelAddresses[labelName] = currentAddr;
            } else {
                  currentAddr += 2; // Each instruction takes 2 bytes (1 word)
            }
      }

      // Open output binary file
      std::ofstream outFile(argv[2], std::ios::binary);
      if (!outFile) {
            std::cerr << "Error: Could not create output file " << argv[2] << "\n";
            return 1;
      }

      // --- Pass 3: Instruction Encoding and Binary Generation ---
      int lineNum = 0;
      for (const std::string& l : lines) {
            lineNum++;
            
            // Skip pure label lines
            if (!l.empty() && l.back() == ':') continue;

            std::vector<std::string> args = splitArgs(l);
            if (args.empty()) continue;

            std::string opStr = args[0];
            std::transform(opStr.begin(), opStr.end(), opStr.begin(), ::toupper);

            uint16_t inst = 0;
            bool valid = true;

            try {
                  // --- Control & Zero-Operand Opcodes ---
                  if (opStr == "NOP")       inst = encode::N(Op::NOP);
                  else if (opStr == "HALT") inst = encode::N(Op::HALT);
                  else if (opStr == "CLS")  inst = encode::N(Op::CLS);
                  else if (opStr == "IRET") inst = encode::N(Op::IRET);
            
                  // --- Immediate ALU & Format I Opcodes ---
                  else if (opStr == "LOADI" || opStr == "ADDI" || opStr == "SUBI" || opStr == "CMPI" || opStr == "NEG") {
                        if (opStr == "NEG") {
                              if (args.size() < 2) throw std::runtime_error("Missing arguments");
                              uint8_t rd = parseRegister(args[1]);
                              inst = encode::I(Op::NEG, rd, 0);
                        } else {
                              if (args.size() < 3) throw std::runtime_error("Missing arguments");
                              uint8_t rd = parseRegister(args[1]);
                              uint8_t imm = static_cast<uint8_t>(std::stoi(args[2], nullptr, 0));
                              uint8_t op = Op::LOADI;
                              if (opStr == "ADDI") op = Op::ADDI;
                              else if (opStr == "SUBI") op = Op::SUBI;
                              else if (opStr == "CMPI") op = Op::CMPI;
                              inst = encode::I(op, rd, imm);
                        }
                  }

                  else if (opStr == "LOADI8") {
                        if (args.size() < 2) throw std::runtime_error("Missing arguments");
                        uint8_t imm8 = static_cast<uint8_t>(std::stoi(args[1], nullptr, 0));
                        inst = encode::I8(Op::LOADI8, imm8);
                  }
            
                  // --- Register-Register ALU & Format R Opcodes ---
                  else if (opStr == "ADD" || opStr == "SUB" || opStr == "MOV" || opStr == "DRAWR" || opStr == "CMP") {
                        if (args.size() < 3) throw std::runtime_error("Missing arguments");
                        uint8_t rd = parseRegister(args[1]);
                        uint8_t rs = parseRegister(args[2]);
                        uint8_t op = Op::ADD;
                        if (opStr == "SUB") op = Op::SUB;
                        else if (opStr == "MOV") op = Op::MOV;
                        else if (opStr == "DRAWR") op = Op::DRAWR;
                        else if (opStr == "CMP") op = Op::CMP;
                        inst = encode::R(op, rd, rs);
                  }

                  // --- Memory & Stack Operations (Format M) ---
                  else if (opStr == "STORE" || opStr == "LOAD" || opStr == "STOREW" || opStr == "LOADW" || opStr == "PUSH" || opStr == "POP") {
                        if (args.size() < 2) throw std::runtime_error("Missing arguments");
                        uint8_t rd = parseRegister(args[1]);
                        uint8_t rs = parseRegister(args[2]);
                        uint8_t op = Op::STORE;
                        
                        if (opStr == "LOAD") op = Op::LOAD;
                        else if (opStr == "STOREW") op = Op::STOREW;
                        else if (opStr == "LOADW") op = Op::LOADW;
                        else if (opStr == "PUSH") op = Op::PUSH;
                        else if (opStr == "POP") op = Op::POP;
                        
                        inst = encode::M(op, rd, rs);
                  }

                  // --- VRAM Graphics Commands (Format X) ---
                  else if (opStr == "DRAW") {
                        if (args.size() < 3) throw std::runtime_error("Missing arguments");
                        uint8_t x = static_cast<uint8_t>(std::stoi(args[1], nullptr, 0));
                        uint8_t y = static_cast<uint8_t>(std::stoi(args[2], nullptr, 0));
                        inst = encode::X(Op::DRAW, x, y);
                  }

                  // --- Control Flow & Jump Instructions (Format J) ---
                  else if (opStr == "JMP" || opStr == "JZ" || opStr == "JNZ" || opStr == "JLT" || opStr == "JGT" || opStr == "JE" || opStr == "CALL" || opStr == "RET") {
                        if (opStr == "RET") {
                              inst = encode::N(Op::RET);
                        } else {
                              if (args.size() < 2) throw std::runtime_error("Missing arguments");
                              
                              std::string targetStr = args.back();
                              uint8_t addr = 0;
                              if (labelAddresses.find(targetStr) != labelAddresses.end()) {
                                    addr = static_cast<uint8_t>(labelAddresses[targetStr]);
                              } else {
                                    addr = static_cast<uint8_t>(std::stoi(targetStr, nullptr, 0));
                              }

                              uint8_t op = Op::JMP;
                              if (opStr == "JZ") op = Op::JZ;
                              else if (opStr == "JNZ") op = Op::JNZ;
                              else if (opStr == "JLT") op = Op::JLT;
                              else if (opStr == "JGT") op = Op::JGT;
                              else if (opStr == "JE") op = Op::JE;
                              else if (opStr == "CALL") op = Op::CALL;
                              
                              inst = encode::J(op, addr);
                        }
                  }

                  else {
                        std::cerr << "Error at line " << lineNum << ": Unknown opcode -> " << opStr << "\n";
                        valid = false;
                  }

                  // Write the encoded 16-bit instruction in little-endian format to the output stream
                  if (valid) {
                        outFile.put(static_cast<char>(inst & 0xFF));
                        outFile.put(static_cast<char>((inst >> 8) & 0xFF));
                  }

            } catch (const std::exception& e) {
                  std::cerr << "Error at line " << lineNum << ": " << e.what() << "\n";
                  valid = false;
            }
      }

      std::cout << "Assembly complete. Output written successfully to " << argv[2] << "\n";
      return 0;
}