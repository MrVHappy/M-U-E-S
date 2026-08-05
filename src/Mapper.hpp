#ifndef MAPPER_H
#define MAPPER_H
#include <vector>
class Mapper{
    private:   
        // contains information about the cartridge
        class Cartridge *cart_info;
    public:
        // Constructor
        Mapper();
        Mapper(Cartridge *cartridge);
        // CPU access
        uint8_t read_PRG(uint16_t address);

        // PPU access
        uint8_t read_CHR(uint16_t address);
        void write_CHR(uint16_t address, uint8_t value);

        // setter
        void set_cart_info(class Cartridge *cartridge);

};
#endif