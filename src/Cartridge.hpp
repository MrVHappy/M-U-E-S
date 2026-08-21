#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <bitset>
#include <string>
#include <array>
#include <vector>
class Cartridge{
    private:
        // file location of the ROM
        std::string file;
        // header
        std::array<uint8_t,16> header;
        // number of PRG banks
        int PRG_banks_num;
        // number of CHR banks
        int CHR_banks_num;
        // Mapper number
        int Mapper_ID;
        // PRG data stores program data
        std::vector<uint8_t> PRG_data;
        // CHR data stores graphics data
        std::vector<uint8_t> CHR_data;
        // SRAM stores save data
        std::vector<uint8_t> s_ram;
        // mirror affects the graphics rendering
        int mirror;
        // link to cartridge mappers
        class Mapper *mapper_info;
        // link to BUS
        class BUS *bus;
    public:
        // Constructor
        Cartridge(class BUS *bus);
        bool load_ROM(std::string path);
        // getters:
        std::vector<uint8_t>& get_CHR_data();
        std::vector<uint8_t>& get_PRG_data();
        int get_PRG_bank();
        int get_CHR_bank();
        class Mapper& get_mapper_info();
        std::array<uint8_t,16> get_header();

        // setter for debugging
        void set_header(int index);
        void set_CHR(int index);

};
#endif