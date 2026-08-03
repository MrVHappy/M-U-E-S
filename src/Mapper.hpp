#ifndef MAPPER_H
#define MAPPER_H
#include <vector>
class Mapper{
    private:
        // identifies the mapper that the cartridge uses
        int mapper_ID;    
    public:
        // CPU access
        uint8_t read_PRG(uint16_t address, int bank_num, std::vector<uint8_t> *PRG_data);

        // PPU access
        uint8_t read_CHR(uint16_t address, int bank_num, std::vector<uint8_t> *CHR_data);
        void write_CHR(uint16_t address, int bank_num, std::vector<uint8_t> *CHR_data);

};
#endif