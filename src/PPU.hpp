#ifndef PPU_H
#define PPU_H
#include <array>
#include <bitset>
// references:
// https://www.nesdev.org/wiki/PPU_registers
class PPU{
    private:
        // registers:
        // control register
        uint8_t ctrl;
        // render register
        uint8_t mask;
        // status register
        uint8_t status;
        // OAM address
        uint8_t oam_addr;
        // OAM data
        uint8_t oam_data;
        // current VRAM address
        uint16_t v;
        // temp VRAM address
        uint16_t t;
        // horizontal scroll
        uint8_t fine_x;
        // VRAM data
        uint8_t vram_data;
        // // OAM DMA
        // uint8_t oam_dma;
        // RAM
        // name table memory 2KiB
        std::array<uint8_t,2048> vram;
        // 32B pallet RAM
        std::array<uint8_t,32> pal_ram;
        // 256B of sprite RAM
        std::array<uint8_t,256> oam_ram;
        // scanline counter
        int scan_ln_count;
        // cycle count
        int cycle_count;
        // boolean flags
        bool v_blank, nmi, sprite_overflow, sprite_0_hit, write_toggle;
        // link PPU to bus
        class BUS *bus;

    public:
        // constructor
        PPU(class BUS* bus);
        // advance cycle
        void tick();
};
#endif