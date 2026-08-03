#include "Cartridge.hpp"
#include <fstream>

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
            
                           
            return true;
        }
        else{
            return false;
        }
}