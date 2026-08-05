#include "Mapper.hpp"
#include "Cartridge.hpp"
#include <iostream>


Mapper::Mapper(){
    // point to NULL until set cart_info is called
    this->cart_info = NULL;
}

Mapper::Mapper(Cartridge *cartridge){
    this->cart_info = cartridge;
}

uint8_t Mapper::read_PRG(uint16_t address){
    int prg_size = static_cast<int>(this->cart_info->get_PRG_data().size());
    // check if valid address
    if(address < 0x8000){
        return 0;
    }
    uint16_t new_address;
    // get the new address
    if(this->cart_info->get_PRG_bank() == 2){
        
        // adjust new address for 32KiB ROM
        new_address = address - 0x8000;
    }
    else if(this->cart_info->get_PRG_bank() == 1) {
        // adjust new address for 16KiB ROM
        new_address = address % prg_size;
    }
    else{
        return 0;
    }
    
    // return the value stored in ROM
    return this->cart_info->get_PRG_data().data()[new_address];
}

uint8_t Mapper::read_CHR(uint16_t address){
    return this->cart_info->get_CHR_data().data()[address];
}

void Mapper::write_CHR(uint16_t address, uint8_t value){
    this->cart_info->get_CHR_data().data()[address] = value;
}

void Mapper::set_cart_info(Cartridge *cartridge){
    this->cart_info = cartridge;
}
