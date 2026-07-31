#ifndef NES_H
#define NES_H
#include <cstdint>   // fixed-width types: uint8_t, uint16_t, uint32_t
#include <cstring>   // memset, memcpy
#include <fstream>   // reading the ROM file
#include <array>     // std::array for fixed-size buffers (safer than raw C arrays)
#include <iostream>  // debugging/console output
#include <string>    // file paths
#include <bitset>
#include <climits>
#include <utility>
#include <vector>
#include <cstdlib>
#include <map>
#include <algorithm>
#include "register_bit.hpp"
#include "Instruction.hpp"
#include "BUS.hpp"
class NES{
    private:
        // Registers:
        // accumilator
        uint8_t acc;

        // a flag that determiNES whether the acc should be used
        bool acc_used;

        // index register used for addressing and loops
        uint8_t x;

        // second index register similar to x
        uint8_t y;

        // Program Counter
        uint16_t pc;

        // stack pointer
        uint8_t stack_ptr;

        // offset for stack_ptr
        const uint16_t OFFSET = 0x100;

        // status flag register
        std::bitset<8> status_flag;

        // Memory BUS
        BUS *bus;

        // flags
        bool page_crossed;

        // an array of instructions
        std::array<Instruction, 256> instruction_set;

        // holds the address resolved by the most recent addressing mode function
        uint16_t resolved_address;

    public:
        NES();

        // getters:
        uint8_t get_acc();
        uint8_t get_x();
        uint8_t get_y();
        uint16_t get_pc();
        uint8_t get_stack_ptr();
        std::bitset<8> get_status_flag();
        std::array<uint8_t, 32768> get_prg_rom();
        // load ROM
        bool load_ROM(const char* path);

        // read to system RAM
        uint8_t read(uint16_t address);
        // write data to system RAM
        void write(uint16_t address, uint8_t value);

        uint8_t fetch_byte();

        // NES addressing modes:
        uint16_t immediate();
        uint16_t zero_page();
        uint16_t absolute();
        uint16_t zero_page_x();
        uint16_t zero_page_y();
        uint16_t absolute_x();
        uint16_t absolute_y();
        uint16_t relative();
        uint16_t indirect();
        uint16_t indirect_x();
        uint16_t indirect_y();
        uint16_t implied();
        uint16_t accumilator();

        // execute instruction:
        void execute();

        // set flag functions:
        void set_Z_and_N_flags(uint8_t value);
        void set_N_flag(uint8_t value);

        void compare(uint8_t val1, uint8_t val2);

        // NES Instruction set:
        void LDA();
        void LDX();
        void LDY();
        void STA();
        void STX();
        void STY();
        void TAX();
        void TAY();
        void TXA();
        void TYA();
        void TSX();
        void TXS();
        void CLC();   
        void CLD();
        void CLV();
        void CLI();
        void SEC();
        void SED();   
        void SEI();
        void PHA();
        void PLA();
        void PHP();
        void PLP();
        void AND();
        void ORA();
        void EOR();
        void CMP();
        void CPX();
        void CPY();
        void BEQ();
        void BNE();
        void BCC();
        void BCS();
        void BMI();
        void BPL();
        void BVC();
        void BVS();
        void ADC();
        void SBC();
        void INC();
        void DEC();
        void INX();
        void INY();
        void DEX();
        void DEY();
        void ASL();
        void LSR();
        void ROL();
        void ROR();
        void JMP();
        void JSR();
        void RTS();
        void NOP();
        void BIT();
        void BRK();
        void RTI();
        // Illegal opcodes:
        void LAX();
        void SAX();
        void DCP();
        void ISB();
        void SLO();
        void RLA();
        void SRE();
        void RRA();
};
#endif