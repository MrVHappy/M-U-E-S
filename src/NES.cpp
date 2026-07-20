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

// References:
// https://en.cppreference.com/cpp/utility/bitset


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

    public:
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
        uint8_t immediate(){
            return fetch_byte();
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
            uint16_t final_address = base_address + this->y;
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
};