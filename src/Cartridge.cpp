#include "Cartridge.hpp"
#include <fstream>

bool Cartridge::load_ROM(std::string path){
    // read the file and format it to binary at the end position
    std::ifstream file(path,std::ios::binary | std::ios::ate);
    // check if the file exsists
        if(file.is_open()){
            // get the file size of the ROM
            std::streamsize file_size = file.tellg();
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
                start_pos = 512;
            }
            // update start position
            file.seekg(start_pos,std::ios::beg);
            // extract the bank count
            PRG_banks_num = this->header[4];
            // calculate the size of the ROM
            size_t prg_size = PRG_banks_num * 16384;
            // get the entire file
            std::vector<uint8_t> prg_data(prg_size);
            file.read(reinterpret_cast<char*>(prg_data.data()),prg_size);
            // check if there is only 1 bank
            if(PRG_banks_num == 1){
                // copy the ROM info twice
                std::copy(prg_data.begin(), prg_data.end(), this->PRG_data.begin());
                std::copy(prg_data.begin(), prg_data.end(), this->PRG_data.begin() + 16384);
                }
            // check if there are 2 banks
            if (PRG_banks_num == 2){
                std::copy(prg_data.begin(), prg_data.end(), this->PRG_data.begin());
            }
                
            return true;
        }
        else{
            return false;
        }
}