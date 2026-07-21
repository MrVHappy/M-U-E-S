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
#include "register_bit.hpp"
#include "Instruction.hpp"

// References:
// https://en.cppreference.com/cpp/utility/bitset
// https://www.nesdev.org/obelisk-6502-guide/reference.html

class NES{
    private:

        // Registers:
        // accumilator
        uint8_t acc;

        // index register used for addressing and loops
        uint8_t x;

        // second index register similar to x
        uint8_t y;

        // Program Counter
        uint16_t pc;

        // stack pointer
        uint8_t stack_ptr;

        // status flag register
        std::bitset<8> status_flag;

        // Memory:
        // internal CPU RAM 2KiB
        std::array<uint8_t, 2048> sys_ram;

        // flags
        bool page_crossed;

        // an array of instructions
        std::array<Instruction, 256> instruction_set;


        // holds the address resolved by the most recent addressing mode function
        uint16_t resolved_address;

    public:
        NES(){
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

            // CLC
            instruction_set[0x18] = {&NES::CLC, &NES::implied, 2};

            // CLD
            instruction_set[0xD8] = {&NES::CLD, &NES::implied, 2};
        }
        // read to system RAM
        uint8_t read(uint16_t address){
            // check if the address value is between 0x0000-0x1FFF
            if (address < 0x2000){
                return this->sys_ram[address & 0x07FF];
            }
            else{
                // Temp:
                return 0;
            }
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
            // update the pc by adding the signed byte
            this->pc += signed_byte;
            return this->pc;
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
            // no operand to resolve - value returned is unused
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

        // NES Instruction set:
        // LDA
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
            // sett decimal flag to false
            status_flag[bit_index(register_bit::D)] = false;
        }
        
};