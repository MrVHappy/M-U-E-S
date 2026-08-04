#include "BUS.hpp"

    // read to system RAM
    uint8_t BUS::read(uint16_t address){
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
            Mapper *mapper = &this->rom->get_mapper_info();
            // check if mapper is NULL
            if(mapper == NULL){
                return 0;
            }
            return mapper->read_PRG(address);
        }
        return 0;
    }   
        // write data to system RAM
    void BUS::write(uint16_t address, uint8_t value){
        if(address < 0x2000){
            this->sys_ram[address & 0x07FF] = value;
        }
    } 

    std::array<uint8_t, 2048> BUS::get_sys_ram(){
        return this->sys_ram;
    }

