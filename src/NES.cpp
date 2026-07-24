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

// References:
// https://en.cppreference.com/cpp/utility/bitset
// https://www.nesdev.org/obelisk-6502-guide/reference.html
// https://en.cppreference.com/cpp/utility/bitset/bitset
// https://en.cppreference.com/cpp/algorithm/copy

class NES{
    private:

        // Registers:
        // accumilator
        uint8_t acc;

        // a flag that determines whether the acc should be used
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

        // Memory:
        // internal CPU RAM 2KiB
        std::array<uint8_t, 2048> sys_ram;

        // program ROM 32KiB
        std::array<uint8_t, 32768> prg_rom;

        // flags
        bool page_crossed;

        // an array of instructions
        std::array<Instruction, 256> instruction_set;


        // holds the address resolved by the most recent addressing mode function
        uint16_t resolved_address;

    public:
        NES(){
            // initialising varaiables 
            this->acc = 0;
            this->x = 0;
            this->y = 0;
            this->pc = 0xC000;
            this->stack_ptr = 0xFF;
            this->status_flag = 0b00000000;
            this->page_crossed = false;
            this->acc_used = false;
            this->page_crossed = false;
            this->resolved_address = 0;

            // instruction locations:
            // LDA:
            instruction_set[0xA9] = { &NES::LDA, &NES::immediate, 2 };
            instruction_set[0xA5] = { &NES::LDA, &NES::zero_page, 3 };
            instruction_set[0xB5] = { &NES::LDA, &NES::zero_page_x, 4 };
            instruction_set[0xAD] = { &NES::LDA, &NES::absolute, 4 };
            instruction_set[0xBD] = { &NES::LDA, &NES::absolute_x, 4 };
            instruction_set[0xB9] = { &NES::LDA, &NES::absolute_y, 4 };
            instruction_set[0xA1] = { &NES::LDA, &NES::indirect_x, 6 };
            instruction_set[0xB1] = { &NES::LDA, &NES::indirect_y, 5 };

            // LDX:
            instruction_set[0xA2] = {&NES::LDX, &NES::immediate, 2};
            instruction_set[0xA6] = {&NES::LDX, &NES::zero_page, 3};
            instruction_set[0xB6] = {&NES::LDX, &NES::zero_page_y, 4};
            instruction_set[0xAE] = {&NES::LDX, &NES::absolute, 4};
            instruction_set[0xBE] = {&NES::LDX, &NES::absolute_y, 4};

            // LDY:
            instruction_set[0xA0] = {&NES::LDY, &NES::immediate, 2};
            instruction_set[0xA4] = {&NES::LDY, &NES::zero_page, 3};
            instruction_set[0xB4] = {&NES::LDY, &NES::zero_page_x, 4};
            instruction_set[0xAC] = {&NES::LDY, &NES::absolute, 4};
            instruction_set[0xBC] = {&NES::LDY, &NES::absolute_x, 4};

            // STA:
            instruction_set[0x85] = {&NES::STA, &NES::zero_page, 3};
            instruction_set[0x95] = {&NES::STA, &NES::zero_page_x, 4};
            instruction_set[0x8D] = {&NES::STA, &NES::absolute, 4};
            instruction_set[0x9D] = {&NES::STA, &NES::absolute_x, 4};
            instruction_set[0x99] = {&NES::STA, &NES::absolute_y, 5};
            instruction_set[0x81] = {&NES::STA, &NES::indirect_x, 6};
            instruction_set[0x91] = {&NES::STA, &NES::indirect_y, 6};

            // STX:
            instruction_set[0x86] = {&NES::STX, &NES::zero_page, 3};
            instruction_set[0x96] = {&NES::STX, &NES::zero_page_y, 4};
            instruction_set[0x8E] = {&NES::STX, &NES::absolute, 4};

            // STY:
            instruction_set[0x84] = {&NES::STY, &NES::zero_page, 3};
            instruction_set[0x94] = {&NES::STY, &NES::zero_page_x, 4};
            instruction_set[0x8C] = {&NES::STY, &NES::absolute, 4};

            // TAX:
            instruction_set[0xAA] = {&NES::TAX, &NES::implied, 2};

            // TAY:
            instruction_set[0xA8] = {&NES::TAY, &NES::implied, 2};

            // TXA:
            instruction_set[0x8A] = {&NES::TXA, &NES::implied, 2};

            // TYA:
            instruction_set[0x98] = {&NES::TYA, &NES::implied, 2};

            // TSX:
            instruction_set[0xBA] = {&NES::TSX, &NES::implied, 2};

            // TXS:
            instruction_set[0x9A] = {&NES::TXS, &NES::implied, 2};

            // CLC:
            instruction_set[0x18] = {&NES::CLC, &NES::implied, 2};

            // CLD:
            instruction_set[0xD8] = {&NES::CLD, &NES::implied, 2};
            
            // CLV:
            instruction_set[0xB8] = {&NES::CLV, &NES::implied, 2};
            
            // CLI:
            instruction_set[0x58] = {&NES::CLI, &NES::implied, 2};
            
            // SEC:
            instruction_set[0x38] = {&NES::SEC, &NES::implied, 2};

            // SED:
            instruction_set[0xF8] = {&NES::SED, &NES::implied, 2};
            
            // SEI:
            instruction_set[0x78] = {&NES::SEI, &NES::implied, 2};

            // PHA:
            instruction_set[0x48] = {&NES::PHA, &NES::implied, 3};

            // PLA:
            instruction_set[0x68] = {&NES::PLA, &NES::implied, 4};

            // PHP:
            instruction_set[0x08] = {&NES::PHP, &NES::implied, 3};

            // PLP:
            instruction_set[0x28] = {&NES::PLP, &NES::implied, 4};

            // AND:
            instruction_set[0x29] = {&NES::AND, &NES::immediate,2};
            instruction_set[0x25] = {&NES::AND, &NES::zero_page,3};
            instruction_set[0x35] = {&NES::AND, &NES::zero_page_x,4};
            instruction_set[0x2D] = {&NES::AND, &NES::absolute,4};
            instruction_set[0x3D] = {&NES::AND, &NES::absolute_x,4};
            instruction_set[0x39] = {&NES::AND, &NES::absolute_y,4};
            instruction_set[0x21] = {&NES::AND, &NES::indirect_x,6};
            instruction_set[0x31] = {&NES::AND, &NES::indirect_y,5};

            // ORA:
            instruction_set[0x09] = {&NES::ORA, &NES::immediate,2};
            instruction_set[0x05] = {&NES::ORA, &NES::zero_page,3};
            instruction_set[0x15] = {&NES::ORA, &NES::zero_page_x,4};
            instruction_set[0x0D] = {&NES::ORA, &NES::absolute,4};
            instruction_set[0x1D] = {&NES::ORA, &NES::absolute_x,4};
            instruction_set[0x19] = {&NES::ORA, &NES::absolute_y,4};
            instruction_set[0x01] = {&NES::ORA, &NES::indirect_x,6};
            instruction_set[0x11] = {&NES::ORA, &NES::indirect_y,5};
            
            // EOR:
            instruction_set[0x49] = {&NES::EOR, &NES::immediate,2};
            instruction_set[0x45] = {&NES::EOR, &NES::zero_page,3};
            instruction_set[0x55] = {&NES::EOR, &NES::zero_page_x,4};
            instruction_set[0x4D] = {&NES::EOR, &NES::absolute,4};
            instruction_set[0x5D] = {&NES::EOR, &NES::absolute_x,4};
            instruction_set[0x59] = {&NES::EOR, &NES::absolute_y,4};
            instruction_set[0x41] = {&NES::EOR, &NES::indirect_x,6};
            instruction_set[0x51] = {&NES::EOR, &NES::indirect_y,5};

            // CMP:
            instruction_set[0xC9] = {&NES::CMP, &NES::immediate,2};
            instruction_set[0xC5] = {&NES::CMP, &NES::zero_page,3};
            instruction_set[0xD5] = {&NES::CMP, &NES::zero_page_x,4};
            instruction_set[0xCD] = {&NES::CMP, &NES::absolute,4};
            instruction_set[0xDD] = {&NES::CMP, &NES::absolute_x,4};
            instruction_set[0xD9] = {&NES::CMP, &NES::absolute_y,4};
            instruction_set[0xC1] = {&NES::CMP, &NES::indirect_x,6};
            instruction_set[0xD1] = {&NES::CMP, &NES::indirect_y,5};

            // CPX:
            instruction_set[0xE0] = {&NES::CPX, &NES::immediate,2};
            instruction_set[0xE4] = {&NES::CPX, &NES::zero_page,3};
            instruction_set[0xEC] = {&NES::CPX, &NES::absolute,4};
            
            // CPY:
            instruction_set[0xC0] = {&NES::CPY, &NES::immediate,2};
            instruction_set[0xC4] = {&NES::CPY, &NES::zero_page,3};
            instruction_set[0xCC] = {&NES::CPY, &NES::absolute,4};

            // BEQ:
            instruction_set[0xF0] = {&NES::BEQ, &NES::relative,2};

            // BNE:
            instruction_set[0xD0] = {&NES::BNE, &NES::relative,2};

            // BCC:
            instruction_set[0x90] = {&NES::BCC, &NES::relative,2};

            // BCS:
            instruction_set[0xB0] = {&NES::BCS, &NES::relative,2};

            // BMI:
            instruction_set[0x30] = {&NES::BMI, &NES::relative,2};

            // BPL:
            instruction_set[0x10] = {&NES::BPL, &NES::relative,2};

            // BVC:
            instruction_set[0x50] = {&NES::BVC, &NES::relative,2};

            // BVS:
            instruction_set[0x70] = {&NES::BVS, &NES::relative,2};

            // ADC:
            instruction_set[0x69] = {&NES::ADC, &NES::immediate,2};
            instruction_set[0x65] = {&NES::ADC, &NES::zero_page,3};
            instruction_set[0x75] = {&NES::ADC, &NES::zero_page_x,4};
            instruction_set[0x6D] = {&NES::ADC, &NES::absolute,4};
            instruction_set[0x7D] = {&NES::ADC, &NES::absolute_x,4};
            instruction_set[0x79] = {&NES::ADC, &NES::absolute_y,4};
            instruction_set[0x61] = {&NES::ADC, &NES::indirect_x,6};
            instruction_set[0x71] = {&NES::ADC, &NES::indirect_y,5};

            // SBC:
            instruction_set[0xE9] = {&NES::SBC, &NES::immediate,2};
            instruction_set[0xE5] = {&NES::SBC, &NES::zero_page,3};
            instruction_set[0xF5] = {&NES::SBC, &NES::zero_page_x,4};
            instruction_set[0xED] = {&NES::SBC, &NES::absolute,4};
            instruction_set[0xFD] = {&NES::SBC, &NES::absolute_x,4};
            instruction_set[0xF9] = {&NES::SBC, &NES::absolute_y,4};
            instruction_set[0xE1] = {&NES::SBC, &NES::indirect_x,6};
            instruction_set[0xF1] = {&NES::SBC, &NES::indirect_y,5};

            // INC:
            instruction_set[0xE6] = {&NES::INC, &NES::zero_page,5};
            instruction_set[0xF6] = {&NES::INC, &NES::zero_page_x, 6};
            instruction_set[0xEE] = {&NES::INC, &NES::absolute, 6};
            instruction_set[0xFE] = {&NES::INC, &NES::absolute_x, 7};

            // DEC:
            instruction_set[0xC6] = {&NES::DEC, &NES::zero_page,5};
            instruction_set[0xD6] = {&NES::DEC, &NES::zero_page_x, 6};
            instruction_set[0xCE] = {&NES::DEC, &NES::absolute, 6};
            instruction_set[0xDE] = {&NES::DEC, &NES::absolute_x, 7};

            // INX:
            instruction_set[0xE8] = {&NES::INX, &NES::implied,2};

            // INY:
            instruction_set[0xC8] = {&NES::INY, &NES::implied,2};

            // DEX:
            instruction_set[0xCA] = {&NES::DEX, &NES::implied,2};

            // DEY:
            instruction_set[0x88] = {&NES::DEY, &NES::implied,2};

            // ASL:
            instruction_set[0x0A] = {&NES::ASL, &NES::accumilator,2};
            instruction_set[0x06] = {&NES::ASL, &NES::zero_page,5};
            instruction_set[0x16] = {&NES::ASL, &NES::zero_page_x,6};
            instruction_set[0x0E] = {&NES::ASL, &NES::absolute,6};
            instruction_set[0x1E] = {&NES::ASL, &NES::absolute_x,7};

            // LSR:
            instruction_set[0x4A] = {&NES::LSR, &NES::accumilator,2};
            instruction_set[0x46] = {&NES::LSR, &NES::zero_page,5};
            instruction_set[0x56] = {&NES::LSR, &NES::zero_page_x,6};
            instruction_set[0x4E] = {&NES::LSR, &NES::absolute,6};
            instruction_set[0x5E] = {&NES::LSR, &NES::absolute_x,7};

            // ROL:
            instruction_set[0x2A] = {&NES::ROL, &NES::accumilator,2};
            instruction_set[0x26] = {&NES::ROL, &NES::zero_page,5};
            instruction_set[0x36] = {&NES::ROL, &NES::zero_page_x,6};
            instruction_set[0x2E] = {&NES::ROL, &NES::absolute,6};
            instruction_set[0x3E] = {&NES::ROL, &NES::absolute_x,7};

            // ROR:
            instruction_set[0x6A] = {&NES::ROR, &NES::accumilator,2};
            instruction_set[0x66] = {&NES::ROR, &NES::zero_page,5};
            instruction_set[0x76] = {&NES::ROR, &NES::zero_page_x,6};
            instruction_set[0x6E] = {&NES::ROR, &NES::absolute,6};
            instruction_set[0x7E] = {&NES::ROR, &NES::absolute_x,7};

            // JMP:
            instruction_set[0x4C] = {&NES::JMP, &NES::absolute,3};
            instruction_set[0x6C] = {&NES::JMP, &NES::indirect,5};

            // JSR:
            instruction_set[0x20] = {&NES::JSR, &NES::absolute, 6};

            // RTS:
            instruction_set[0x60] = {&NES::RTS, &NES::implied, 6};

            // NOP:
            instruction_set[0xEA] = {&NES::NOP, &NES::implied, 2};

            // BIT:
            instruction_set[0x24] = {&NES::BIT, &NES::zero_page, 3};
            instruction_set[0x2C] = {&NES::BIT, &NES::absolute, 4};

            // BRK:
            instruction_set[0x00] = {&NES::BRK, &NES::implied, 7};

            // RTI:
            instruction_set[0x40] = {&NES::RTI, &NES::implied, 6};
        }

        // load ROM
        bool load_ROM(const char* path){
            // read the file and format it to binary at the end position
            std::ifstream file(path,std::ios::binary | std::ios::ate);
            // check if the file exsists
            if(file.is_open()){
                // get the file size of the ROM
                std::streamsize file_size = file.tellg();
                // set ROM to start position
                file.seekg(0,std::ios::beg);
                // create a header to validate the ROM
                std::array<uint8_t,16> header;
                // ectract the first 16 bytes of the file to header
                file.read(reinterpret_cast<char*>(header.data()),16);
                // validating the first 4 bytes:
                if((header[0] != 'N') || (header[1] != 'E') || (header[2] != 'S') || (header[3] != 0x1A) ){
                    // if incorrect invalid ROM
                    return false;
                }
                // set start position after reading the header
                uint8_t start_pos = 16;
                // detecting if the ROM contains a trainer
                if((header[6] & 0x04) != 0){
                    // trainer is present
                    start_pos = 512;
                }
                // update start position
                file.seekg(start_pos,std::ios::beg);
                // extract the bank count
                uint8_t prg_bank_no = header[4];
                // calculate the size of the ROM
                size_t prg_size = prg_bank_no * 16384;
                // get the entire file
                std::vector<uint8_t> prg_data(prg_size);
                file.read(reinterpret_cast<char*>(prg_data.data()),prg_size);
                // check if there is only 1 bank
                if(prg_bank_no == 1){
                    // copy the ROM info twice
                    std::copy(prg_data.begin(), prg_data.end(), this->prg_rom.begin());
                    std::copy(prg_data.begin(), prg_data.end(), this->prg_rom.begin() + 16384);
                }
                // check if there are 2 banks
                if (prg_bank_no == 2){
                    std::copy(prg_data.begin(), prg_data.end(), this->prg_rom.begin());
                }
                
                return true;
            }
            else{
                return false;
            }
        }

        // read to system RAM
        uint8_t read(uint16_t address){
            // check if the address value is between 0x0000-0x1FFF
            if (address < 0x2000){
                // CPU data
                return this->sys_ram[address & 0x07FF];
            }
            // check if the address is between 0x2000-0x7FFF
            if((address >= 0x2000) && (address <= 0x7FFF)){
                // PPU and APU data
                // temp
                return 0;
            }
            // check if the address is >= 0x8000
            if (address >= 0x8000){
                // ROM data
                return this->prg_rom[address -0x8000];
            }
            return 0;
        }   
        // write data to system RAM
        void write(uint16_t address, uint8_t value){
            if(address < 0x2000){
                this->sys_ram[address & 0x07FF] = value;
            }
        } 

        uint8_t fetch_byte(){
            // store the current pc value in memory
            uint8_t pc_byte = read(this->pc);
            // increment pc
            this->pc++;
            return pc_byte;
        }

        // NES addressing modes:
        uint16_t immediate(){
            // get the address from pc
            uint16_t current_pc = this->pc;
            // increment pc
            this->pc++;
            return current_pc;
        }
        uint16_t zero_page(){
            return fetch_byte();
        }
        uint16_t absolute(){
            // fetch the low byte
            uint16_t low_byte = fetch_byte();
            // fetch the high byte and left shift by 8
            uint16_t high_byte = fetch_byte() << 8;
            // OR the 2 bytes together
            uint16_t address = high_byte | low_byte;
            // return the address
            return address;
        }
        uint16_t zero_page_x(){
            // fetch the the byte
            uint8_t wrapped_byte = fetch_byte();
            // add index x 
            wrapped_byte += this->x;
            // cast wrapped byte into uint16_t
            uint16_t wrapped_address = static_cast<uint16_t>(wrapped_byte);
            return wrapped_address;
        }
        uint16_t zero_page_y(){
            // fetch the the byte
            uint8_t wrapped_byte = fetch_byte();
            // add index y 
            wrapped_byte += this->y;
            // cast wrapped byte into uint16_t
            uint16_t wrapped_address = static_cast<uint16_t>(wrapped_byte);
            return wrapped_address;
        }
        uint16_t absolute_x(){
            // fetch the address using absolute
            uint16_t base_address = absolute();
            // add index x
            uint16_t final_address = base_address + this->x;
            // check if x crossed into the next page
            page_crossed =((base_address & 0xFF00) != (final_address & 0xFF00));
            return final_address;
        }
        uint16_t absolute_y(){
            // fetch the address using absolute
            uint16_t base_address = absolute();
            // add index y
            uint16_t final_address = base_address + this->y;
            // check if y crossed into the next page
            page_crossed =((base_address & 0xFF00) != (final_address & 0xFF00));
            return final_address;
        }
        uint16_t relative(){
            // fetch the byte and convert to a signed byte
            int8_t signed_byte = static_cast<int8_t>(fetch_byte());
            // store the sum of pc and signed byte
            uint16_t target_address = this->pc + signed_byte;
            return target_address;
        }
        uint16_t indirect(){
            // get the absolute address 
            uint16_t ptr_address = absolute();
            // get low byte
            uint16_t low_byte = read(ptr_address);
            if( (ptr_address & 0xFF) == 0xFF){
                // wraps within the same page 
                ptr_address = ptr_address & 0xFF00;
            }
            else{
                // increment the ptr_address
                ptr_address++;
            }
            // get high byte
            uint16_t high_byte = read(ptr_address) ;
            // perform an left shift 8 times
            high_byte = high_byte << 8;
            // merge the bytes together
            uint16_t final_address = high_byte + low_byte;
            return final_address;
        }
        uint16_t indirect_x(){
            // fetch the wrapped_address
            uint16_t wrapped_address = zero_page_x();
            // get the low byte via the wrapped address
            uint16_t low_byte = read(wrapped_address);
            if( (wrapped_address & 0xFF) == 0xFF){
                // set to 0x00
                wrapped_address = 0x00;
            }
            else{
                // increment the wrapped_address
                wrapped_address++;
            }
            // get the high byte
            uint16_t high_byte = read(wrapped_address) << 8;
            // get the final address and return
            uint16_t final_address = high_byte | low_byte;
            return final_address;
        }
        uint16_t indirect_y(){
            // fetch the zero page address byte
            uint8_t zero_page_address = fetch_byte();
            // fetch the low byte via zero_page_address
            uint16_t low_byte = read(zero_page_address);
            // update zero page address
            if( (zero_page_address & 0xFF) == 0xFF){
                // set to 0x00
                zero_page_address = 0x00;
            }
            else{
                // increment the zero_page_address
                zero_page_address++;
            }
            // fetch the high byte via the updated zero page address
            uint16_t high_byte = read(zero_page_address) << 8;
            // combine high and low bytes together
            uint16_t base_byte = (high_byte | low_byte);
            // then add index y
            uint16_t final_byte = base_byte + this->y;
            // second cycle required if the final byte wrapped
            page_crossed = (base_byte & 0xFF00) != (final_byte & 0xFF00);
            return final_byte;
        }
        uint16_t implied(){
            // no operand to be used
            return 0;
        }

        uint16_t accumilator(){
            // set acc_used to true
            this->acc_used = true;
            // no operand to be used
            return 0;
        }

        // execute instruction:
        void execute(){
            // get the opcode from memory
            uint8_t opcode = fetch_byte();
            // obtain the instruction set
            Instruction new_instruction = this->instruction_set[opcode];
            // call the addressing mode function and collect the address
            this->resolved_address = (this->*new_instruction.addressing_mode)();
            // call the instruction
            (this->*new_instruction.operation)();
            // handel cycle (temp)
        }

        // set flag functions:
        void set_Z_and_N_flags(uint8_t value){
            // check if the address value is 0
            if(value == 0)
            // set the "Z" flag to true
                status_flag[bit_index(register_bit::Z)] = true;
            else
                // set the "Z" flag to false
                status_flag[bit_index(register_bit::Z)] = false;
            // value & 0x80
            if(value & 0x80)
                // set the "N" flag to true
                status_flag[bit_index(register_bit::N)] = true;
            else
                // set the "N" flag to false
                status_flag[bit_index(register_bit::N)] = false;
        }

        void set_N_flag(uint8_t value){
            if(value & 0x80)
                // set the "N" flag to true
                status_flag[bit_index(register_bit::N)] = true;
            else
                // set the "N" flag to false
                status_flag[bit_index(register_bit::N)] = false;
        }


        void compare(uint8_t val1, uint8_t val2){
            if(val1 == val2){
                // set the C and Z flag to true
                status_flag[bit_index(register_bit::C)] = true;
                status_flag[bit_index(register_bit::Z)] = true;
            }
            else if (val1 > val2){
                // set the C flag to true and Z flag to false
                status_flag[bit_index(register_bit::C)] = true;
                status_flag[bit_index(register_bit::Z)] = false;
            }
            else{
                // set the C and Z flag to false
                status_flag[bit_index(register_bit::C)] = false;
                status_flag[bit_index(register_bit::Z)] = false;
            }
        }

        // NES Instruction set:
        void LDA(){
            // get the value by reading the address
            uint8_t address_val = read(this->resolved_address);
            // store in the accumilator
            this->acc = address_val;
            set_Z_and_N_flags(address_val);

        }
        void LDX(){
            // get the value by reading the address
            uint8_t address_val = read(this->resolved_address);
            // store in the index x
            this->x = address_val;
            set_Z_and_N_flags(address_val);
        }
        void LDY(){
            // get the value by reading the address
            uint8_t address_val = read(this->resolved_address);
            // store in index y
            this->y = address_val;
            // check if the address value is 0
            set_Z_and_N_flags(address_val);
        }
        void STA(){
            // write the value stored in ACC in the resolved address
            write(this->resolved_address, this->acc);
        }
        void STX(){
            // write the value stored in index x in the resolved address
            write(this->resolved_address, this->x);
        }
        void STY(){
            // write the value stored in index y in the resolved address
            write(this->resolved_address, this->y);
        }
        void TAX(){
            // copy contents from acc to index x
            this->x = this->acc;
            set_Z_and_N_flags(this->x);
        }
        void TAY(){
            // copy contents from acc to index y
            this->y = this->acc;
            set_Z_and_N_flags(this->y);
        }
        void TXA(){
            // copy contents from index x to acc
            this->acc = this->x;
            set_Z_and_N_flags(this->acc);
        }
        void TYA(){
            // copy contents from index y to acc
            this->acc = this->y;
            set_Z_and_N_flags(this->acc);
        }
        void TSX(){
            // copy contents from stack pointer to index x
            this->x = this->stack_ptr;
            set_Z_and_N_flags(this->x);
        }
        void TXS(){
            // copy contents from index x to stack pointer
            this->stack_ptr = this->x;
        }
        
        void CLC(){
            // set the carry flag to false
            status_flag[bit_index(register_bit::C)] = false;
        }
        
        void CLD(){
            // set decimal flag to false
            status_flag[bit_index(register_bit::D)] = false;
        }
        
        void CLV(){
            // set the overflow flag to false
            status_flag[bit_index(register_bit::V)] = false;
        }

        void CLI(){
            // set the inturrupt disable flag to false
            status_flag[bit_index(register_bit::I)] = false;
        }

        void SEC(){
            // set the carry flag to true
            status_flag[bit_index(register_bit::C)] = true;
        }
        
        void SED(){
            // set decimal flag to true
            status_flag[bit_index(register_bit::D)] = true;
        }
        
        void SEI(){
            // set the inturrupt disable flag to true
            status_flag[bit_index(register_bit::I)] = true;
        }

        void PHA(){
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // push value of the acc onto the stack
            write(stack_address,this->acc);
            // update the stack pointer
            this->stack_ptr--;

        }

        void PLA(){
            // update the stack pointer
            this->stack_ptr++;
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // pull the value from the stack to the acc
            this->acc = read(stack_address);
            set_Z_and_N_flags(this->acc);
        }

        void PHP(){
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // convert status flag as a uint_8_t
            uint8_t status_info = static_cast<uint8_t>(this->status_flag.to_ulong());
            // force flag B and flag DASH to true
            status_info = status_info | 0b00110000;
            // write status info to stack
            write(stack_address,status_info);
            // update stack ptr
            this->stack_ptr--;
        }

        void PLP(){
            // update the stack pointer
            this->stack_ptr++;
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // copy the status info to the status flag
            this->status_flag = (read(stack_address));
        }

        void AND(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            // perform acc AND address val
            this->acc = this->acc & address_val;
            set_Z_and_N_flags(acc);  
        }

        void ORA(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            // perform acc OR address val
            this->acc = this->acc | address_val;
            set_Z_and_N_flags(acc); 
        }

        void EOR(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            // perform acc XOR address val
            this->acc = this->acc ^ address_val;
            set_Z_and_N_flags(acc); 
        }

        void CMP(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            compare(this->acc, address_val);
            // calculate the result by subtracting acc with address val
            uint8_t result = this->acc - address_val;
            set_N_flag(result);
        }

        void CPX(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            compare(this->x, address_val);
            // calculate the result by subtracting index x with address val
            uint8_t result = this->x - address_val;
            set_N_flag(result);
        }

        void CPY(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            compare(this->y, address_val);
            // calculate the result by subtracting index y with address val
            uint8_t result = this->y - address_val;
            set_N_flag(result);
        }

        void BEQ(){
            // check if the flag Z is true
            if(status_flag[bit_index(register_bit::Z)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void BNE(){
            // check if the flag Z is false
            if(!status_flag[bit_index(register_bit::Z)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void BCC(){
            // check if the flag C is false
            if(!status_flag[bit_index(register_bit::C)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void BCS(){
            // check if the flag C is true
            if(status_flag[bit_index(register_bit::C)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void BMI(){
            // check if the flag N is true
            if(status_flag[bit_index(register_bit::N)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void BPL(){
            // check if the flag N is false
            if(!status_flag[bit_index(register_bit::N)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void BVC(){
            // check if the flag N is false
            if(!status_flag[bit_index(register_bit::V)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void BVS(){
            // check if the flag N is true
            if(status_flag[bit_index(register_bit::V)])
                // update the pc
                this->pc = this->resolved_address;
        }

        void ADC(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            // convert carry flag to uint8_t
            uint8_t carry = static_cast<uint8_t>(this->status_flag[bit_index(register_bit::C)]);
            // calculate sum
            uint16_t sum = address_val + this->acc + carry;
            // check if sum exceeds 255
            if(sum > 255){
                // set carry to true
                this->status_flag[bit_index(register_bit::C)] = true;
            }
            else{
                // set carry to false
                this->status_flag[bit_index(register_bit::C)] = false;
            }
            // cast sum as uint8_t
            uint8_t result = static_cast<uint8_t>(sum);
            // get the result of overflow
            this->status_flag[bit_index(register_bit::V)] = ((this->acc ^ result) & (address_val ^ result) & 0x80) != 0;
            // store result in acc
            this->acc = result;
            set_Z_and_N_flags(this->acc);
            
        }

        void SBC(){
            // get the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            // convert carry flag to uint8_t
            uint8_t carry = static_cast<uint8_t>(this->status_flag[bit_index(register_bit::C)]);
            // calculate sum
            uint16_t sum = this->acc + (~address_val & 0xFF) + carry;
            // check if sum exceeds 255
            if(sum > 255){
                // set carry to true
                this->status_flag[bit_index(register_bit::C)] = true;
            }
            else{
                // set carry to false
                this->status_flag[bit_index(register_bit::C)] = false;
            }
            // cast sum as uint8_t
            uint8_t result = static_cast<uint8_t>(sum);
            // get the result of overflow
            this->status_flag[bit_index(register_bit::V)] = ((this->acc ^ result) & (address_val ^ result) & 0x80) != 0;
            // store result in acc
            this->acc = result;
            set_Z_and_N_flags(this->acc);
        }

        void INC(){
            // extract the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            // incrament this by one
            address_val++;
            // write the new value in the same address
            write(this->resolved_address, address_val);
            set_Z_and_N_flags(address_val);
        }

        void DEC(){
            // extract the value from the resolved address
            uint8_t address_val = read(this->resolved_address);
            // decrament this by one
            address_val--;
            // write the new value in the same address
            write(this->resolved_address, address_val);
            set_Z_and_N_flags(address_val);
        }

        void INX(){
            // increment index x by one
            this->x++;
            set_Z_and_N_flags(this->x);
        }

        void INY(){
            // increment index y by one
            this->y++;
            set_Z_and_N_flags(this->y);
        }

        void DEX(){
            // decrement index x by one
            this->x--;
            set_Z_and_N_flags(this->x);
        }

        void DEY(){
            // decrement index y by one
            this->y--;
            set_Z_and_N_flags(this->y);
        }

        void ASL(){
            // check if to use the acc or not
            if(this->acc_used){
                // left shift acc by 1
                this->acc = this->acc << 1;
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = (this->acc & 0x80) != 0;
                // set acc_used to false
                this->acc_used = false;
                set_Z_and_N_flags(this->acc);
            }
            else{
                // obtain the address value using the resolved address
                uint8_t address_val = read(resolved_address);
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = (address_val & 0x80) != 0;
                // left shift address val
                address_val = address_val << 1;
                write(this->resolved_address,address_val);
                set_Z_and_N_flags(address_val);
            }
        }

        void LSR(){
            // check if to use the acc or not
            if(this->acc_used){
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = (this->acc & 0x01) != 0;
                // right shift acc by 1
                this->acc = this->acc >> 1;
                // set acc_used to false
                this->acc_used = false;
                set_Z_and_N_flags(this->acc);
            }
            else{
                // obtain the address value using the resolved address
                uint8_t address_val = read(resolved_address);
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = (address_val & 0x01) != 0;
                // right shift address val
                address_val = address_val >> 1;
                write(this->resolved_address,address_val);
                set_Z_and_N_flags(address_val);
            }
        }

        void ROL(){
            uint8_t carry = static_cast<uint8_t>(this->status_flag[bit_index(register_bit::C)]);
            // check if to use the acc or not
            if(this->acc_used){
                // capture shifted bit
                uint8_t old_bit_7 = (this->acc & 0x80);
                // left shift acc by 1 and then OR old_bit
                this->acc = (this->acc << 1) | carry;
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = old_bit_7;
                // set acc_used to false
                this->acc_used = false;
                set_Z_and_N_flags(this->acc);
            }
            else{
                // obtain the address value using the resolved address
                uint8_t address_val = read(resolved_address);
                // capture shifted bit
                uint8_t old_bit_7 = (address_val & 0x80);
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = old_bit_7;
                // left shift address val and then OR 0x01
                address_val = (address_val << 1) | carry;
                write(this->resolved_address,address_val);
                set_Z_and_N_flags(address_val);
            }
        }

        void ROR(){
            uint8_t carry = static_cast<uint8_t>(this->status_flag[bit_index(register_bit::C)]);
            // check if to use the acc or not
            if(this->acc_used){
                // capture shifted bit
                uint8_t old_bit_0 = this->acc & 0x01;
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = old_bit_0;
                // right shift acc by 1
                this->acc = (this->acc >> 1) | carry;
                // set acc_used to false
                this->acc_used = false;
                set_Z_and_N_flags(this->acc);
            }
            else{
                // obtain the address value using the resolved address
                uint8_t address_val = read(resolved_address);
                // capture shifted bit
                uint8_t old_bit_0 = address_val & 0x01;
                // set the carry index
                this->status_flag[bit_index(register_bit::C)] = old_bit_0;
                // right shift address val
                address_val = (address_val >> 1) | (carry << 7);
                write(this->resolved_address,address_val);
                set_Z_and_N_flags(address_val);
            }
        }

        void JMP(){
            // set PC to resolved address
            this->pc = this->resolved_address;
        }

        void JSR(){
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // calculate PC - 1
            uint16_t return_target = this->pc - 1;
            // get the high byte from the PC - 1
            uint8_t high_byte = static_cast<uint8_t>(return_target >> 8);
            // get the low byte from the PC - 1
            uint8_t low_byte = static_cast<uint8_t>(return_target);
            // push high byte into the stack
            write(stack_address,high_byte);
            // update the stack pointer
            this->stack_ptr--;
            // get the address from the stack
            stack_address = this->OFFSET + this->stack_ptr;
            // push low byte into the stack
            write(stack_address,low_byte);
            // update the stack pointer
            this->stack_ptr--;
            // update the PC
            this->pc = resolved_address;
        }

        void RTS(){
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // extract the low byte from the stack
            uint8_t low_byte = read(stack_address);
            // update the stack pointer
            stack_ptr++;
            // get the address from the stack
            stack_address = this->OFFSET + this->stack_ptr;
            // extract the high byte from the stack
            uint8_t high_byte = read(stack_address);
            // get the return address
            uint16_t return_address = (high_byte << 8) | low_byte;
            // update the PC
            this->pc = return_address + 1;
        }

        void NOP(){
            // No operation
            return;
        }

        void BIT(){
            // get the address val from resolved address
            uint8_t address_val = read(this->resolved_address);
            // perform acc AND address_val
            uint8_t result = this->acc & address_val;
            if(result == 0)
                // set the "Z" flag to true
                status_flag[bit_index(register_bit::Z)] = true;
            else
                // set the "Z" flag to false
                status_flag[bit_index(register_bit::Z)] = false;         
            set_N_flag(address_val);
            // set the V flag from bit 6 of address val
            this->status_flag[bit_index(register_bit::V)] = (address_val & 0x40) != 0;
        }

        void BRK(){
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // calculate PC + 1
            uint16_t return_target = this->pc + 1;
            // get the high byte from the PC + 1
            uint8_t high_byte = static_cast<uint8_t>(return_target >> 8);
            // get the low byte from the PC + 1
            uint8_t low_byte = static_cast<uint8_t>(return_target);
            // push high byte into the stack
            write(stack_address,high_byte);
            // update the stack pointer
            this->stack_ptr--;
            // get the address from the stack
            stack_address = this->OFFSET + this->stack_ptr;
            // push low byte into the stack
            write(stack_address,low_byte);
            // update the stack pointer
            this->stack_ptr--;
            // convert status flag as a uint_8_t
            uint8_t status_info = static_cast<uint8_t>(this->status_flag.to_ulong());
            // force flag B and flag DASH to true
            status_info = status_info | 0b00110000;
            // get the address from the stack
            stack_address = this->OFFSET + this->stack_ptr;
            // push status info into the stack
            write(stack_address,status_info);
            // update the stack pointer
            this->stack_ptr--;
        }

        void RTI(){
            // update the stack pointer
            this->stack_ptr++;
            // get the address from the stack
            uint16_t stack_address = this->OFFSET + this->stack_ptr;
            // extract the status info from the stack
            this->status_flag = read(stack_address);
            // update the stack pointer
            this->stack_ptr++;
            // get the address from the stack
            stack_address = this->OFFSET + this->stack_ptr;
            // extract the low byte from the stack
            uint8_t low_byte = read(stack_address);
            // update the stack pointer
            this->stack_ptr++;
            // get the address from the stack
            stack_address = this->OFFSET + this->stack_ptr;
            // extract the high byte from the stack
            uint8_t high_byte = read(stack_address);
            // combine the two bytes
            uint16_t return_address = (high_byte << 8) | low_byte;
            // store return_address to PC
            this->pc = return_address;
        }
};