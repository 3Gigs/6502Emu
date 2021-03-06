/*
 * http://archive.6502.org/datasheets/rockwell_r65c00_microprocessors.pdf
 */

#include "CPU.h"
#include <iostream>
#include <map>

emu6502::CPU::CPU(RAM *_ram) {
  ram = _ram;

  instructions = {
      {"BRK", &CPU::BRK, &CPU::IMM, 7}, {"ORA", &CPU::ORA, &CPU::IZX, 6},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 3}, {"ORA", &CPU::ORA, &CPU::ZP0, 3},
      {"ASL", &CPU::ASL, &CPU::ZP0, 5}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"PHP", &CPU::PHP, &CPU::IMP, 3}, {"ORA", &CPU::ORA, &CPU::IMM, 2},
      {"ASL", &CPU::ASL, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 2},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"ORA", &CPU::ORA, &CPU::ABS, 4},
      {"ASL", &CPU::ASL, &CPU::ABS, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"BPL", &CPU::BPL, &CPU::REL, 2}, {"ORA", &CPU::ORA, &CPU::IZY, 5},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"ORA", &CPU::ORA, &CPU::ZPX, 4},
      {"ASL", &CPU::ASL, &CPU::ZPX, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"CLC", &CPU::CLC, &CPU::IMP, 2}, {"ORA", &CPU::ORA, &CPU::ABY, 4},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"ORA", &CPU::ORA, &CPU::ABX, 4},
      {"ASL", &CPU::ASL, &CPU::ABX, 7}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"JSR", &CPU::JSR, &CPU::ABS, 6}, {"AND", &CPU::AND, &CPU::IZX, 6},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"BIT", &CPU::BIT, &CPU::ZP0, 3}, {"AND", &CPU::AND, &CPU::ZP0, 3},
      {"ROL", &CPU::ROL, &CPU::ZP0, 5}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"PLP", &CPU::PLP, &CPU::IMP, 4}, {"AND", &CPU::AND, &CPU::IMM, 2},
      {"ROL", &CPU::ROL, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 2},
      {"BIT", &CPU::BIT, &CPU::ABS, 4}, {"AND", &CPU::AND, &CPU::ABS, 4},
      {"ROL", &CPU::ROL, &CPU::ABS, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"BMI", &CPU::BMI, &CPU::REL, 2}, {"AND", &CPU::AND, &CPU::IZY, 5},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"AND", &CPU::AND, &CPU::ZPX, 4},
      {"ROL", &CPU::ROL, &CPU::ZPX, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"SEC", &CPU::SEC, &CPU::IMP, 2}, {"AND", &CPU::AND, &CPU::ABY, 4},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"AND", &CPU::AND, &CPU::ABX, 4},
      {"ROL", &CPU::ROL, &CPU::ABX, 7}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"RTI", &CPU::RTI, &CPU::IMP, 6}, {"EOR", &CPU::EOR, &CPU::IZX, 6},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 3}, {"EOR", &CPU::EOR, &CPU::ZP0, 3},
      {"LSR", &CPU::LSR, &CPU::ZP0, 5}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"PHA", &CPU::PHA, &CPU::IMP, 3}, {"EOR", &CPU::EOR, &CPU::IMM, 2},
      {"LSR", &CPU::LSR, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 2},
      {"JMP", &CPU::JMP, &CPU::ABS, 3}, {"EOR", &CPU::EOR, &CPU::ABS, 4},
      {"LSR", &CPU::LSR, &CPU::ABS, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"BVC", &CPU::BVC, &CPU::REL, 2}, {"EOR", &CPU::EOR, &CPU::IZY, 5},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"EOR", &CPU::EOR, &CPU::ZPX, 4},
      {"LSR", &CPU::LSR, &CPU::ZPX, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"CLI", &CPU::CLI, &CPU::IMP, 2}, {"EOR", &CPU::EOR, &CPU::ABY, 4},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"EOR", &CPU::EOR, &CPU::ABX, 4},
      {"LSR", &CPU::LSR, &CPU::ABX, 7}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"RTS", &CPU::RTS, &CPU::IMP, 6}, {"ADC", &CPU::ADC, &CPU::IZX, 6},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 3}, {"ADC", &CPU::ADC, &CPU::ZP0, 3},
      {"ROR", &CPU::ROR, &CPU::ZP0, 5}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"PLA", &CPU::PLA, &CPU::IMP, 4}, {"ADC", &CPU::ADC, &CPU::IMM, 2},
      {"ROR", &CPU::ROR, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 2},
      {"JMP", &CPU::JMP, &CPU::IND, 5}, {"ADC", &CPU::ADC, &CPU::ABS, 4},
      {"ROR", &CPU::ROR, &CPU::ABS, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"BVS", &CPU::BVS, &CPU::REL, 2}, {"ADC", &CPU::ADC, &CPU::IZY, 5},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"ADC", &CPU::ADC, &CPU::ZPX, 4},
      {"ROR", &CPU::ROR, &CPU::ZPX, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"SEI", &CPU::SEI, &CPU::IMP, 2}, {"ADC", &CPU::ADC, &CPU::ABY, 4},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"ADC", &CPU::ADC, &CPU::ABX, 4},
      {"ROR", &CPU::ROR, &CPU::ABX, 7}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"STA", &CPU::STA, &CPU::IZX, 6},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"STY", &CPU::STY, &CPU::ZP0, 3}, {"STA", &CPU::STA, &CPU::ZP0, 3},
      {"STX", &CPU::STX, &CPU::ZP0, 3}, {"???", &CPU::XXX, &CPU::IMP, 3},
      {"DEY", &CPU::DEY, &CPU::IMP, 2}, {"???", &CPU::NOP, &CPU::IMP, 2},
      {"TXA", &CPU::TXA, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 2},
      {"STY", &CPU::STY, &CPU::ABS, 4}, {"STA", &CPU::STA, &CPU::ABS, 4},
      {"STX", &CPU::STX, &CPU::ABS, 4}, {"???", &CPU::XXX, &CPU::IMP, 4},
      {"BCC", &CPU::BCC, &CPU::REL, 2}, {"STA", &CPU::STA, &CPU::IZY, 6},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"STY", &CPU::STY, &CPU::ZPX, 4}, {"STA", &CPU::STA, &CPU::ZPX, 4},
      {"STX", &CPU::STX, &CPU::ZPY, 4}, {"???", &CPU::XXX, &CPU::IMP, 4},
      {"TYA", &CPU::TYA, &CPU::IMP, 2}, {"STA", &CPU::STA, &CPU::ABY, 5},
      {"TXS", &CPU::TXS, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"???", &CPU::NOP, &CPU::IMP, 5}, {"STA", &CPU::STA, &CPU::ABX, 5},
      {"???", &CPU::XXX, &CPU::IMP, 5}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"LDY", &CPU::LDY, &CPU::IMM, 2}, {"LDA", &CPU::LDA, &CPU::IZX, 6},
      {"LDX", &CPU::LDX, &CPU::IMM, 2}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"LDY", &CPU::LDY, &CPU::ZP0, 3}, {"LDA", &CPU::LDA, &CPU::ZP0, 3},
      {"LDX", &CPU::LDX, &CPU::ZP0, 3}, {"???", &CPU::XXX, &CPU::IMP, 3},
      {"TAY", &CPU::TAY, &CPU::IMP, 2}, {"LDA", &CPU::LDA, &CPU::IMM, 2},
      {"TAX", &CPU::TAX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 2},
      {"LDY", &CPU::LDY, &CPU::ABS, 4}, {"LDA", &CPU::LDA, &CPU::ABS, 4},
      {"LDX", &CPU::LDX, &CPU::ABS, 4}, {"???", &CPU::XXX, &CPU::IMP, 4},
      {"BCS", &CPU::BCS, &CPU::REL, 2}, {"LDA", &CPU::LDA, &CPU::IZY, 5},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"LDY", &CPU::LDY, &CPU::ZPX, 4}, {"LDA", &CPU::LDA, &CPU::ZPX, 4},
      {"LDX", &CPU::LDX, &CPU::ZPY, 4}, {"???", &CPU::XXX, &CPU::IMP, 4},
      {"CLV", &CPU::CLV, &CPU::IMP, 2}, {"LDA", &CPU::LDA, &CPU::ABY, 4},
      {"TSX", &CPU::TSX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 4},
      {"LDY", &CPU::LDY, &CPU::ABX, 4}, {"LDA", &CPU::LDA, &CPU::ABX, 4},
      {"LDX", &CPU::LDX, &CPU::ABY, 4}, {"???", &CPU::XXX, &CPU::IMP, 4},
      {"CPY", &CPU::CPY, &CPU::IMM, 2}, {"CMP", &CPU::CMP, &CPU::IZX, 6},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"CPY", &CPU::CPY, &CPU::ZP0, 3}, {"CMP", &CPU::CMP, &CPU::ZP0, 3},
      {"DEC", &CPU::DEC, &CPU::ZP0, 5}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"INY", &CPU::INY, &CPU::IMP, 2}, {"CMP", &CPU::CMP, &CPU::IMM, 2},
      {"DEX", &CPU::DEX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 2},
      {"CPY", &CPU::CPY, &CPU::ABS, 4}, {"CMP", &CPU::CMP, &CPU::ABS, 4},
      {"DEC", &CPU::DEC, &CPU::ABS, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"BNE", &CPU::BNE, &CPU::REL, 2}, {"CMP", &CPU::CMP, &CPU::IZY, 5},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"CMP", &CPU::CMP, &CPU::ZPX, 4},
      {"DEC", &CPU::DEC, &CPU::ZPX, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"CLD", &CPU::CLD, &CPU::IMP, 2}, {"CMP", &CPU::CMP, &CPU::ABY, 4},
      {"NOP", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"CMP", &CPU::CMP, &CPU::ABX, 4},
      {"DEC", &CPU::DEC, &CPU::ABX, 7}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"CPX", &CPU::CPX, &CPU::IMM, 2}, {"SBC", &CPU::SBC, &CPU::IZX, 6},
      {"???", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"CPX", &CPU::CPX, &CPU::ZP0, 3}, {"SBC", &CPU::SBC, &CPU::ZP0, 3},
      {"INC", &CPU::INC, &CPU::ZP0, 5}, {"???", &CPU::XXX, &CPU::IMP, 5},
      {"INX", &CPU::INX, &CPU::IMP, 2}, {"SBC", &CPU::SBC, &CPU::IMM, 2},
      {"NOP", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::SBC, &CPU::IMP, 2},
      {"CPX", &CPU::CPX, &CPU::ABS, 4}, {"SBC", &CPU::SBC, &CPU::ABS, 4},
      {"INC", &CPU::INC, &CPU::ABS, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"BEQ", &CPU::BEQ, &CPU::REL, 2}, {"SBC", &CPU::SBC, &CPU::IZY, 5},
      {"???", &CPU::XXX, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 8},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"SBC", &CPU::SBC, &CPU::ZPX, 4},
      {"INC", &CPU::INC, &CPU::ZPX, 6}, {"???", &CPU::XXX, &CPU::IMP, 6},
      {"SED", &CPU::SED, &CPU::IMP, 2}, {"SBC", &CPU::SBC, &CPU::ABY, 4},
      {"NOP", &CPU::NOP, &CPU::IMP, 2}, {"???", &CPU::XXX, &CPU::IMP, 7},
      {"???", &CPU::NOP, &CPU::IMP, 4}, {"SBC", &CPU::SBC, &CPU::ABX, 4},
      {"INC", &CPU::INC, &CPU::ABX, 7}, {"???", &CPU::XXX, &CPU::IMP, 7},
  };
}

emu6502::CPU::~CPU() {}

uint8_t emu6502::CPU::getx() { return x; }
uint8_t emu6502::CPU::gety() { return y; }
uint8_t emu6502::CPU::getA() { return A; }
uint8_t emu6502::CPU::getS() { return stkp; }
uint16_t emu6502::CPU::getP() { return pc; }

void emu6502::CPU::reset() {
  // Set program counter
  abs_addr = 0xFFFC; // Reads from 0xFFFC to get absolute value to address to jump to
  uint16_t low = ram->mem_read(abs_addr);
  uint16_t high = ram->mem_read(abs_addr + 1);
  pc = (high << 8) | low;

  stkp = 0xFD;

  // Clear registers
  A = 0x00;
  x = 0x00;
  y = 0x00;
  status = 0x00;

  // Clear helper variables
  abs_addr = 0x0000;
  rel_addr = 0x00;
  ir = 0x00;

  // Resets take time, 8 cycles in fact
  clock.addCycles(8);
}

/*
 * Executes instructions, which take a variable amount of time,
 * given by the opcode, addressing mode, or the instruction itself.
 * Then waits until the clock has fully cycled
 * I simulated cycle delays for testing
 */
void emu6502::CPU::execute() {
  do {
    if (clock.clockCycles == 0) {
      ir = ram->mem_read(pc++);
      // Add cycles based given by instruction
      clock.addCycles(instructions[ir].cycles);
      // Call given addressing mode function. Stores additional cycles
      uint8_t aCycles1 = (this->*instructions[ir].mode)();
      // Execute instruction. Stores additional cycles
      uint8_t aCycles2 = (this->*instructions[ir].op)();
      clock.addCycles(aCycles1 + aCycles2);
    }

    clock.cycle();
  } while (clock.clockCycles != 0);
}

// Addressing Mode implementations

uint8_t emu6502::CPU::IMP() {
  ir = A;
  return 0;
}

uint8_t emu6502::CPU::IMM() {
  abs_addr = pc++;
  return 0;
}

uint8_t emu6502::CPU::ZP0() {
  // Allows us to store the first 256-bytes of data into zero-page,
  // reducing the amount of cycles needed
  abs_addr = ram->mem_read(pc++);
  abs_addr &= 0x00FF;
  return 0;
}

uint8_t emu6502::CPU::ZPX() {
  abs_addr = (ram->mem_read(pc++) + x);
  abs_addr &= 0x00FF;

  return 0;
}

uint8_t emu6502::CPU::ZPY() {
  abs_addr = (ram->mem_read(pc++) + y);
  abs_addr &= 0x00FF;

  return 0;
}

// Allow branching instructions to jump (at 128 signed bit limit)
uint8_t emu6502::CPU::REL() {
  abs_addr = ram->mem_read(pc++);
  // Checks the most significant bit to see if it's signed or not
  if (rel_addr & 0x80) {
    rel_addr |= 0xFF00;
  }

  return 0;
}

// Fetch absolute address
uint8_t emu6502::CPU::ABS() {
  uint16_t low = ram->mem_read(pc++);
  uint16_t high = ram->mem_read(pc++);

  abs_addr = (high << 8) | low;

  return 0;
}

// ABS but with X offset
uint8_t emu6502::CPU::ABX() {
  uint16_t lo = ram->mem_read(pc++);
  uint16_t hi = ram->mem_read(pc++);

  abs_addr = (hi << 8) | lo;
  abs_addr += x;

  // Check if page has changed
  return ((abs_addr & 0xFF00) != (hi << 8)) ? 1 : 0;
}

uint8_t emu6502::CPU::ABY() {
  uint16_t lo = ram->mem_read(pc++);
  uint16_t hi = ram->mem_read(pc++);

  abs_addr = (hi << 8) | lo;
  abs_addr += y;

  // Check if page has changed
  return ((abs_addr & 0xFF00) != (hi << 8)) ? 1 : 0;
}

/*
 * IND: Indirect addressing
 * 6502 pointer implementation
 */
uint8_t emu6502::CPU::IND() {
  // Get the actual address where data resides (aka pointer)
  uint16_t ptr_low = ram->mem_read(pc++);
  uint16_t ptr_high = ram->mem_read(pc++);
  uint16_t ptr = (ptr_high << 8) | ptr_low;

  /*
   * See https ://www.nesdev.com/6502bugs.txt
   * Calculating high byte will fail because
   * adding a high byte by 1 changes the page
   */
  if (ptr_low == 0x00FF) // This will simulate the bug
  {
    abs_addr = (ram->mem_read(ptr & 0xFF00) << 8 | ram->mem_read(ptr + 0));
  } else {
    abs_addr = (ram->mem_read(ptr + 1) << 8) | ram->mem_read(ptr + 0);
  }

  return 0;
}

// IND but with X offset
uint8_t emu6502::CPU::IZX() {
  // Somewhere in the zero page...
  uint16_t zp_addr = ram->mem_read(pc++);

  // X offset
  uint16_t low = ram->mem_read((uint16_t)(zp_addr + (uint16_t)x) & 0x00FF);
  uint16_t high = ram->mem_read((uint16_t)(zp_addr + (uint16_t)x + 1) & 0x00FF);

  abs_addr = (high << 8) | low;

  return 0;
}

uint8_t emu6502::CPU::IZY() {
  // Somewhere in the zero page...
  uint16_t zp_addr = ram->mem_read(pc++);

  // X offset
  uint16_t low = ram->mem_read((uint16_t)(zp_addr + (uint16_t)y) & 0x00FF);
  uint16_t high = ram->mem_read((uint16_t)(zp_addr + (uint16_t)y + 1) & 0x00FF);

  abs_addr = (high << 8) | low;

  return ((abs_addr & 0xFF00) != (high << 8)) ? 1 : 0;
}

/*
 *
 *
 * Instruction implementations
 *
 */

/*
 * Fetch data from pc, or by argument
 * @param void or addr
 * @return Single byte of data
 */
uint8_t emu6502::CPU::fetch() {
  if (!((instructions[ir].mode) == &CPU::IMP)) {
    fetched = ram->mem_read(abs_addr);
  }
  return fetched;
}

/*
 * Performs bitwise AND operation
 * @param void
 * @return 1 to specify this requires additional clock cycles
 */
uint8_t emu6502::CPU::AND() {
  fetch();
  A = A & fetched;
  if (A == 0x00) {
    status |= (1 << Z);
  }
  if (A & 0x80) {
    status |= (1 << N);
  }

  return 1;
}

/*
 * ASL: Shifts a bit in accumulator to the left by one
 */
uint8_t emu6502::CPU::ASL() {
  fetch();
  uint16_t temp = (uint16_t)fetched << 1;
  // Carry flag: Activate if high byte is non-zero
  ((temp & 0xFF00) > 0) ? status |= (1 << C) : status &= ~(1 << C);
  // Zero flag: Activate if low byte is zero
  ((temp & 0x00FF) == 0x00) ? status |= (1 << Z) : status &= ~(1 << Z);
  // Negative flag: Activate if most significant low byte is 1
  (temp & 0x80) ? status |= (1 << N) : status &= ~(1 << N);
  if (instructions[ir].mode == &CPU::IMP)
    A = temp & 0x00FF;
  else
    ram->mem_write(abs_addr, temp & 0x00FF);
  return 0;
}

/*
 * Branche if Carry Set
 * Address is pc added by relative addr if carry mode is ON
 */
uint8_t emu6502::CPU::BCS() {
  if (status & (1 << C)) {
    clock.cycle();
    abs_addr = pc + rel_addr;
    if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
      clock.cycle();
    }
    // Set program counter to new counter
    pc = abs_addr;
  }
  return 0;
}

uint8_t emu6502::CPU::BCC() {
  clock.cycle();
  abs_addr = pc + abs_addr;
  if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
    clock.cycle();
  }
  pc = abs_addr;

  return 0;
}

/*
 * BEQ: Branch if equal
 * If the zero flag is set then add the relative displacement to
 * the program counter to cause a branch to a new location.
 */
uint8_t emu6502::CPU::BEQ() {
  if (status & (1 << Z)) {
    clock.cycle();
    abs_addr = pc + rel_addr;
    if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
      clock.cycle();
    }
    pc = abs_addr;
  }
  return 0;
}

/*
 * BIT: Bit test
 * Test if one or more bits are set in a memory location using AND.
 * Sets status flags based on the result
 */
uint8_t emu6502::CPU::BIT() {
  fetch();
  // 16 bits used to catach overflows
  uint16_t temp = A & fetched;
  // Zero: Set if result is zero
  (temp & 0x00FF) & 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  // Negative: Set if bit 7 is 1
  fetched &(1 << 7) ? (status |= (1 << N)) : (status &= ~(1 << N));
  // Overflow: Set if bit 6 is 1
  fetched &(1 << 6) ? (status |= (1 << V)) : (status &= ~(1 << V));

  return 0;
}

uint8_t emu6502::CPU::BMI() {
  if (status & (1 << N)) {
    clock.cycle();
    abs_addr = pc + rel_addr;
    if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
      clock.cycle();
    }
    pc = abs_addr;
  }
  return 0;
}

uint8_t emu6502::CPU::BNE() {
  if (!(status & (1 << Z))) {
    clock.cycle();
    abs_addr = pc + rel_addr;
    if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
      clock.cycle();
    }
    pc = abs_addr;
  }
  return 0;
}

uint8_t emu6502::CPU::BPL() {
  if (!(status & (1 << N))) {
    clock.cycle();
    abs_addr = pc + rel_addr;
    if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
      clock.cycle();
    }
    pc = abs_addr;
  }
  return 0;
}

uint8_t emu6502::CPU::BRK() {
  pc++;
  // Set interrupt disable flag
  status |= (1 << I);
  // Write current program counter to stack
  ram->mem_write(0x0100 + stkp--, (pc >> 8) & 0x00FF);
  ram->mem_write(0x0100 + stkp--, pc & 0x00FF);
  // Write current status to stack
  status |= (1 << B);
  ram->mem_write(0x0100 + stkp--, status);
  status &= ~(1 << B);
  // Set program counter to defined location
  pc = (uint16_t)ram->mem_read(0xFFFE) | ((uint16_t)ram->mem_read(0xFFFF) << 8);

  return 0;
}

uint8_t emu6502::CPU::BVC() {
  if (!(status & (1 << V))) {
    clock.cycle();
    abs_addr = pc + rel_addr;
    if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
      clock.cycle();
    }
    pc = abs_addr;
  }
  return 0;
}

uint8_t emu6502::CPU::BVS() {
  if (status & (1 << V)) {
    clock.cycle();
    abs_addr = pc + rel_addr;
    if ((abs_addr & 0xFF00) != (pc & 0xFF00)) {
      clock.cycle();
    }
    pc = abs_addr;
  }
  return 0;
}

uint8_t emu6502::CPU::CLC() {
  status &= ~(1 << C);
  return 0;
}

uint8_t emu6502::CPU::CLD() {
  status &= ~(1 << D);
  return 0;
}

uint8_t emu6502::CPU::CLI() {
  status &= ~(1 << I);
  return 0;
}

uint8_t emu6502::CPU::CLV() {
  status &= ~(1 << V);
  return 0;
}

/*
 * CMP: Compare accumulator and fetched value
 * Compare using flags to tell result
 */
uint8_t emu6502::CPU::CMP() {
  fetch();
  uint16_t temp = (uint16_t)A - (uint16_t)fetched;
  // Carry: Set if A >= M
  A >= fetched ? status |= (1 << C) : status &= ~(1 << C);
  // Zero: Set if (A-M) = 0
  (temp & 0x00FF) == 0x0000 ? status |= (1 << Z) : status &= ~(1 << Z);
  // Negative: Set if 7 bit is 1
  temp & 0x0080 ? status |= (1 << N) : status &= ~(1 << N);

  return 1;
}

/*
 * CPX: Compare X register w/ fetched value
 */
uint8_t emu6502::CPU::CPX() {
  fetch();
  uint16_t temp = (uint16_t)x - (uint16_t)fetched;
  // Carry: Set if A >= M
  x >= fetched ? status |= (1 << C) : status &= ~(1 << C);
  // Zero: Set if (A-M) = 0
  (temp & 0x00FF) == 0x0000 ? status |= (1 << Z) : status &= ~(1 << Z);
  // Negative: Set if 7 bit is 1
  temp & 0x0080 ? status |= (1 << N) : status &= ~(1 << N);

  return 0;
}

/*
 * CPY: Compare Y register w/ fetched value
 */
uint8_t emu6502::CPU::CPY() {
  fetch();
  uint16_t temp = (uint16_t)y - (uint16_t)fetched;
  // Carry: Set if A >= M
  y >= fetched ? status |= (1 << C) : status &= ~(1 << C);
  // Zero: Set if (A-M) = 0
  (temp & 0x00FF) == 0x0000 ? status |= (1 << Z) : status &= ~(1 << Z);
  // Negative: Set if 7 bit is 1
  temp & 0x0080 ? status |= (1 << N) : status &= ~(1 << N);

  return 0;
}

/*
 * DEC: Decrement value at memory location
 */
uint8_t emu6502::CPU::DEC() {
  fetch();
  uint16_t temp = fetched - 1;
  ram->mem_write(abs_addr, temp & 0x00FF);
  (temp & 0x00FF) == 0x0000 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (temp & 0x0080) ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

uint8_t emu6502::CPU::DEX() {
  x--;
  (x == 0x00) ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (x & 0x80) ? (status |= (1 << N)) : (status &= ~(1 << Z));

  return 0;
}

uint8_t emu6502::CPU::DEY() {
  y--;
  (y == 0x00) ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (y & 0x80) ? (status |= (1 << N)) : (status &= ~(1 << Z));

  return 0;
}

/*
 * EOR: Exclusive OR
 * Set A to A^M
 */
uint8_t emu6502::CPU::EOR() {
  fetch();
  A = A ^ fetched;
  (A == 0x00) ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (A & 0x80) ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 1;
}

/*
 * INC: Increment Memory
 * M = M + 1
 */
uint8_t emu6502::CPU::INC() {
  fetch();
  uint16_t temp = fetched + 1;
  ram->mem_write(abs_addr, temp & 0x00FF);
  (temp & 0x00FF) == 0x0000 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (temp & 0x0080) ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

/*
 * INX: Increment value in X register
 */
uint8_t emu6502::CPU::INX() {
  x++;
  (x == 0x00) ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (x & 0x80) ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

/*
 * INY: Increment value in Y register
 */
uint8_t emu6502::CPU::INY() {
  y++;
  y == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  y & 0x80 ? (status |= (1 << Z)) : (status &= ~(1 << Z));

  return 0;
}

/*
 * JMP: Jump to location
 */
uint8_t emu6502::CPU::JMP() {
  pc = abs_addr;
  return 0;
}

/*
 * JSR: Jump to Sub-Routine
 * Push current pc to stack
 */
uint8_t emu6502::CPU::JSR() {
  pc--;
  ram->mem_write(0x0100 + stkp--, (pc >> 8) & 0x00FF);
  ram->mem_write(0x0100 + stkp--, pc & 0x00FF);
  pc = abs_addr;

  return 0;
}

uint8_t emu6502::CPU::LDA() {
  fetch();
  A = fetched;
  // Sets zero flag if A=0
  (A == 0) ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  // Set negative flag if bit 7 of A is set
  (A & 0x80) ? (status |= (1 << N)) : (status &= ~(1 << N));
  return 1;
}

/*
 * LDX: Load the X register
 * X = M
 */
uint8_t emu6502::CPU::LDX() {
  fetch();
  x = fetched;
  x == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  x & 0x80 ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 1;
}

/*
 * LDY: Load the Y register
 * Y = M
 */
uint8_t emu6502::CPU::LDY() {
  fetch();
  y = fetched;
  y == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  y & 0x80 ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 1;
}

/*
 * LSR: Logical Shift Right
 * Shifts bits in memory to the right to memory or accumulator
 */
uint8_t emu6502::CPU::LSR() {
  fetch();
  fetched & 0x0001 ? (status |= (1 << C)) : (status &= ~(1 << C));
  uint8_t temp = fetched >> 1;
  (temp & 0x00FF) == 0x0000 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  temp & 0x0080 ? (status |= (1 << N)) : (status &= ~(1 << N));
  if (instructions[ir].mode == &CPU::IMP) {
    A = temp & 0x00FF;
  } else {
    ram->mem_write(abs_addr, temp & 0x00FF);
  }

  return 0;
}

/*
 * NOP: No Operation
 * No changes to processor other than incrementing PC
 */
uint8_t emu6502::CPU::NOP() {
  pc++;
  return 0;
}

/*
 * ORA: Logical Inclusive OR
 * Perform inclusive OR, store on accumulator
 * A = A | M
 */
uint8_t emu6502::CPU::ORA() {
  fetch();
  A = A | fetched;
  A == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  A & 0x80 ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 1;
}

/*
 * PHP: Push processor status, copies status flags to stack
 */
uint8_t emu6502::CPU::PHP() {
  ram->mem_write(0x0100 + stkp--, status);

  return 0;
}

/*
 * SEC: Set carry flag to 1
 */
uint8_t emu6502::CPU::SEC() {
  status |= (1 << C);
  return 0;
}

/*
 * SED: Set decimal flag to 1
 */
uint8_t emu6502::CPU::SED() {
  status |= (1 << D);
  return 0;
}

/*
 * SEI: Set decimal flag to 1
 */
uint8_t emu6502::CPU::SEI() {
  status |= (1 << I);
  return 0;
}

/*
 * STA: Set contents of the accumulator into memory
 */
uint8_t emu6502::CPU::STA() {
  ram->mem_write(abs_addr, A);
  return 0;
}

/*
 * STX: Store X register
 */
uint8_t emu6502::CPU::STX() {
  ram->mem_write(abs_addr, x);
  return 0;
}

/*
 * STY: Store Y register
 */
uint8_t emu6502::CPU::STY() {
  ram->mem_write(abs_addr, y);
  return 0;
}

/*
 * TAX: Transfer Accumulator to X register
 */
uint8_t emu6502::CPU::TAX() {
  x = A;
  x == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (x & 0x80) ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

/*
 * TAY: Transfer Accumulator to Y register
 */
uint8_t emu6502::CPU::TAY() {
  y = A;
  y == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  (y & 0x80) ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

/*
 * TSX: Transfer stack pointer to X
 */
uint8_t emu6502::CPU::TSX() {
  x = stkp;
  x == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  x & 0x80 ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

/*
 * TXA: Transfer X register to accumulator
 */
uint8_t emu6502::CPU::TXA() {
  A = x;
  A == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  A & 0x80 ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

/*
 * TXS: Transfer X register to stack pointer
 */
uint8_t emu6502::CPU::TXS() {
  stkp = x;
  return 0;
}

uint8_t emu6502::CPU::TYA() {
  A = y;
  A == 0 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  A & 0x80 ? (status |= (1 << N)) : (status &= ~(1 << N));
  return 0;
}

/*
 * PLP: Pull a byte off the stack and replace the status register w/ that byte
 */
uint8_t emu6502::CPU::PLP() {
  status = ram->mem_read(0x0100 + ++stkp);

  return 0;
}

/*
 * ROL: Rotate Left, shifts the bits in A or M to the left
 */
uint8_t emu6502::CPU::ROL() {
  fetch();
  uint16_t temp = (uint16_t)(fetched << 1) | (status & (1 << C));
  temp & 0xFF00 ? (status |= (1 << C)) : (status &= ~(1 << C));
  (temp & 0x00FF) == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  temp &(1 << 7) ? (status |= (1 << N)) : (status &= ~(1 << N));

  if (instructions[ir].mode == &CPU::IMP) {
    A = temp & 0x00FF;
  } else {
    ram->mem_write(abs_addr, temp & 0x00FF);
  }

  return 0;
}

/*
 * ROR: Rotate Right, shifts the bits in A or M to the left
 */
uint8_t emu6502::CPU::ROR() {
  fetch();
  uint16_t temp = (uint16_t)(status & (1 << C)) | (fetched >> 1);
  fetched & 0x0001 ? (status |= (1 << C)) : (status &= ~(1 << C));
  (temp & 0x00FF) == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  temp & 0x0080 ? (status |= (1 << N)) : (status &= ~(1 << N));
  if (instructions[ir].mode == &CPU::IMP) {
    A = temp & 0x00FF;
  } else {
    ram->mem_write(abs_addr, temp & 0x00FF);
  }

  return 0;
}

uint8_t emu6502::CPU::RTI() {
  // We're doing pre-increments because during IRQ, the stack pointer
  // would've been incremented 3 times, and therefore would've been off-by-one

  // Set back status register
  status = ram->mem_read(0x0100 + ++stkp);
  status &= ~(1 << B);
  status &= ~(1 << U);
  // Set back program counter
  pc = (uint16_t)ram->mem_read(0x0100 + ++stkp);
  pc |= (uint16_t)ram->mem_read(0x0100 + ++stkp) << 8;

  return 0;
}

uint8_t emu6502::CPU::RTS() {
  pc = (uint16_t)ram->mem_read(0x0100 + ++stkp);
  pc |= (uint16_t)ram->mem_read(0x0100 + ++stkp) << 8;
  pc++;

  return 0;
}

uint8_t emu6502::CPU::ADC() {
  /*
   * Add with Carry In
   * A = A + M + C
   */
  fetch();
  // We're using 16 bits temp to capture the carry bit and chain ADC function
  uint16_t temp =
      (uint16_t)A + (uint16_t)fetched + (uint16_t)(status & (1 << C));

  // If result overflows, set carry flag otherwise unset it
  temp > 255 ? (status |= (1 << C)) : (status &= ~(1 << C));
  // Set or unset zero flag
  temp == 0 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  // Set overflow flag
  (~(uint16_t)A ^ (uint16_t)fetched) &
          (((uint16_t)A) ^ ((uint16_t)temp & 0x0080))
      ? (status |= (1 << V))
      : (status &= ~(1 << V));
  // Set negative flag
  temp & 0x80 ? (status |= 1 << N) : (status &= ~(1 << N));
  A = temp & 0x00FF;

  return 1;
}

/*
 * Subtraction formula
 * A = A - M - (1-C)
 * The fetched value (M) will be inverted to perform subtraction,
 * which means it is pretty much the same as ADC
 */
uint8_t emu6502::CPU::SBC() {
  fetch();
  // We still use 16 bits to catch carry outs
  uint16_t value = ((uint16_t)fetched) ^ 0x00FF;

  uint16_t temp = (uint16_t)A + value + (uint16_t)(status & (1 << C));

  // Carry flag modification
  temp > 255 ? (status |= (1 << C)) : (status &= ~(1 << C));
  // Zero flag modification
  temp == 0 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  // Overflow flag modifcation
  (~(uint16_t)A ^ (uint16_t)fetched) &
          (((uint16_t)A) ^ ((uint16_t)temp & 0x0080))
      ? (status |= (1 << V))
      : (status &= ~(1 << V));
  // Negative flag modifcation
  temp & 0x80 ? (status |= 1 << N) : (status &= ~(1 << N));
  A = temp & 0x00FF;

  return 1;
}

/*
 * PHA: Push the accumulator
 */
uint8_t emu6502::CPU::PHA() {
  ram->mem_write(0x0100 + stkp--, A);
  return 0;
}

/*
 * PLA: Pull from accumulator
 * Note: This also pops the top item in stack
 */
uint8_t emu6502::CPU::PLA() {
  A = ram->mem_read(0x0100 + ++stkp);
  A == 0x00 ? (status |= (1 << Z)) : (status &= ~(1 << Z));
  A == 0x80 ? (status |= (1 << N)) : (status &= ~(1 << N));

  return 0;
}

/*
 * XXX: Illegal opcode
 */
uint8_t emu6502::CPU::XXX() { return 0; }

/*
 *
 *
 * Interrupts
 *
 */

/*
 * IRQ: Requests an interrupt when called
 */
void emu6502::CPU::irq() {
  // Check if disable interrupts flag is set
  if (!(status & (1 << I))) {
    // Write current program counter to return from
    ram->mem_write(0x0100 + stkp--, (pc >> 8) & 0x00FF);
    ram->mem_write(0x0100 + stkp--, pc & 0x00FF);

    // Write status for returning
    status &= ~(1 << B);
    status |= (1 << U);
    status |= (1 << I);
    ram->mem_write(0x0100 + stkp--, status);

    // Set program counter
    abs_addr = 0xFFFE;
    uint16_t low = ram->mem_read(abs_addr);
    uint16_t high = ram->mem_read(abs_addr + 1);
    pc = (high << 8) | low;

    clock.addCycles(7);
  }
}

/*
 * NMI: Same as IRQ, but you can't be ignored with something like
 * setting I flag
 */
void emu6502::CPU::nmi() {
  // Push program counter to stack
  ram->mem_write(0x0100 + stkp--, (pc >> 8) & 0x00FF);
  ram->mem_write(0x0100 + stkp--, pc & 0x00FF);

  // Set status and write to stack
  status &= ~(1 << B);
  status |= (1 << U);
  status |= (1 << I);
  ram->mem_write(0x0100 + stkp--, status);

  // Set program counter
  abs_addr = 0xFFFA;
  uint16_t low = ram->mem_read(abs_addr);
  uint16_t high = ram->mem_read(abs_addr + 1);
  pc = (high << 8) | low;

  clock.addCycles(8);
}
