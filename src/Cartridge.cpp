#include "Cartridge.hpp"
#include "Mapper.hpp"
#include "BUS.hpp"
#include <fstream>

Cartridge::Cartridge(BUS *bus){
    this->mapper_info = new Mapper();
    this->bus = bus;
}

bool Cartridge::load_ROM(std::string path){
    // read the file and format it to binary at the end position
    std::ifstream file(path,std::ios::binary | std::ios::ate);
    // check if the file exsists
    if(file.is_open()){
        // set ROM to start position
        file.seekg(0,std::ios::beg);
        // ectract the first 16 bytes of the file to header
        file.read(reinterpret_cast<char*>(this->header.data()),16);
        // validating the first 4 bytes:
        if((this->header[0] != 'N') || (this->header[1] != 'E') || (this->header[2] != 'S') || (this->header[3] != 0x1A) ){
            // if incorrect invalid ROM
            return false;
        }
        // set start position after reading the header
        int start_pos = 16;
           // detecting if the ROM contains a trainer
        if((this->header[6] & 0x04) != 0){
            // trainer is present
            start_pos += 512;
        }
        // update start position
        file.seekg(start_pos,std::ios::beg);
        // extract the bank count
        PRG_banks_num = this->header[4];
        // calculate the size of the ROM
        size_t prg_size = PRG_banks_num * 16384;
        // set the size of PRG data to prg size
        this->PRG_data.resize(prg_size);
        // get the entire file
        file.read(reinterpret_cast<char*>(this->PRG_data.data()),prg_size);
        
        // get the number of CHR banks from header
        this->CHR_banks_num = this->header[5];
        // check if CHR is used as ROM
        if(this->CHR_banks_num > 0){
            // calculate the size of the CHR
            size_t chr_size = this->CHR_banks_num * 8192;
            // update the size of CHR data
            this->CHR_data.resize(chr_size);
            // get the entire file
            file.read(reinterpret_cast<char*>(this->CHR_data.data()),chr_size);
        }
        else{
            // use CHR as RAM and resize to 8KiB
            this->CHR_data.resize(8192);
        }
        // extract mapper information
        uint8_t lower_mapper = this->header[6] >> 4;
        uint8_t upper_mapper = (this->header[7] & 0b11110000) >> 4;
        upper_mapper = upper_mapper << 4;
        this->Mapper_ID = (lower_mapper | upper_mapper);
        this->mapper_info->set_cart_info(this);
        
        return true;
    }
    else{
        return false;
    }
}

std::vector<uint8_t>& Cartridge::get_CHR_data(){
    return this->CHR_data;
}

std::vector<uint8_t>& Cartridge::get_PRG_data(){
    return this->PRG_data;
}

int Cartridge::get_PRG_bank(){
    return this->PRG_banks_num;
}

int Cartridge::get_CHR_bank(){
    return this->CHR_banks_num;
}
Mapper& Cartridge::get_mapper_info(){
    return *this->mapper_info;
}

void Cartridge::write_CHR(uint16_t address, uint8_t value){
    // check if CHR is treated as RAM
    if(this->CHR_banks_num > 0){
        // CHR used as ROM
        return;
    }
    CHR_data[address] = value;
}