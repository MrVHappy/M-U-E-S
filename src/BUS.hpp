#ifndef BUS_H
#define BUS_H
#include <array>
#include <cstdint>
#include <vector>
#include <bitset>
class BUS{
    private:
        // Memory:
        // internal CPU RAM 2KiB
        std::array<uint8_t, 2048> sys_ram;

        // program ROM 32KiB
        std::array<uint8_t, 32768> prg_rom;

        // link to the Cartridge
        class Cartridge *rom;

        // link to the PPU
        class PPU *ppu;

        // OAM DMA
        uint8_t oam_dma;
        // DMA timer
        int dma_timer = 514;
        // DMA flag
        bool dma_active = true;

    public:
        // read to system RAM
        uint8_t read(uint16_t address);
        // write data to system RAM
        void write(uint16_t address, uint8_t value);
        // getters
        std::array<uint8_t, 2048> get_sys_ram();
        class Cartridge & get_rom();
        class PPU & get_ppu();
        
        // setters
        void set_cartridge(class Cartridge *rom);
        void set_ppu(class PPU *ppu);
        void set_dma_timer(int value);

        // getters
        bool get_dma_active();
        int get_dma_timer();

        // increments
        void increment_timer(int cycles);

        // clear and updates
        void dma_activate();
        void dma_deactivate();

        // DEBUG functions
        void set_NOP_sys_ram();
        void clear_sys_ram();
        void set_sys_ram(uint8_t value);
};
#endif