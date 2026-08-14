#include "BUS.hpp"
#include "Cartridge.hpp"
#include "Mapper.hpp"
#include "PPU.hpp"

    // read to system RAM
    uint8_t BUS::read(uint16_t address){
        // check if the address value is between 0x0000-0x1FFF
        if (address < 0x2000){
            // CPU data
            return this->sys_ram[address & 0x07FF];
        }
        // check if the address is between 0x2000-0x3FFF
        if((address >= 0x2000) && (address <= 0x3FFF)){
            // PPU data
            // mask the address to allow mirroring
            uint16_t masked_addr = address & 0x2007;
            // determine which PPU register to use
            switch (masked_addr){
                case 0x2002:{
                    // extract the PPU status register
                    uint8_t ppu_status = this->ppu->get_status();
                    // clear the v blank and write toggle
                    this->ppu->clear_v_blank();
                    this->ppu->clear_write_toggle();
                    // PPU status register
                    return ppu_status;
                }
                case 0x2004:{
                    // extract PPU OAM data register
                    uint8_t oam_data = this->ppu->get_oam_data();
                    // update the PPU OAM address
                    this->ppu->update_oam_addr();
                    // return OAM data
                    return oam_data;
                }   
                case 0x2007:{
                    // PPU data register
                    return this->ppu->get_vram_data();
                }                    
                default:
                    return 0;
            }
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
        
    void BUS::write(uint16_t address, uint8_t value){
        // write data to system RAM
        if(address < 0x2000){
            this->sys_ram[address & 0x07FF] = value;
        }
        // PPU functionality
        else if((address >= 0x2000) && (address < 0x4000)){
            // temp
        }
        // APU and Controller registers
        else if((address >= 0x4000) && (address < 0x4018)){
            // temp
        }
        // write to SRAM
        else if((address >= 0x6000) && (address < 0x8000)){
            // temp
        }
        // Cartridge mapper
        else if((address >= 0x8000) && (address <= 0xFFFF)){
            // temp
        }
    } 

    std::array<uint8_t, 2048> BUS::get_sys_ram(){
        return this->sys_ram;
    }

    void BUS::set_cartridge(Cartridge *rom){
        this->rom = rom;
    }

    Cartridge BUS::get_rom(){
        return *this->rom;
    }

