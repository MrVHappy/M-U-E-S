#ifndef BUS_H
#define BUS_H
#include <array>
#include <cstdint>
#include <vector>
#include <bitset>
#include "Cartridge.hpp"
#include "Mapper.hpp"
class BUS{
    private:
        // Memory:
        // internal CPU RAM 2KiB
        std::array<uint8_t, 2048> sys_ram;

        // program ROM 32KiB
        std::array<uint8_t, 32768> prg_rom;

        // link to the Cartridge
        Cartridge *rom;

    public:
        // read to system RAM
        uint8_t read(uint16_t address);
        // write data to system RAM
        void write(uint16_t address, uint8_t value);
        // getters
        std::array<uint8_t, 2048> get_sys_ram();
};
#endif