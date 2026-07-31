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
            return this->prg_rom[address -0x8000];
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
    std::array<uint8_t, 32768> BUS::get_prg_rom(){
        return this->prg_rom;
    }
    void BUS::set_prg_rom(std::array<uint8_t, 32768> rom){
        this->prg_rom = rom;
    }
    void BUS::copy_to_rom(std::vector<uint8_t> prg_data, uint8_t prg_bank_no){
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
    }
