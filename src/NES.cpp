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
};