#ifndef PPU_H
#define PPU_H
#include <array>
#include <bitset>
// references:
// https://www.nesdev.org/wiki/PPU_registers
class PPU{
    private:
        // registers:
        // control register at address 0x2000
        uint8_t ctrl;
        // render register at address 0x2001
        uint8_t mask;
        // status register at address 0x2002
        uint8_t status;
        // OAM address at address 0x2003
        uint8_t oam_addr;
        // OAM data at address 0x2004
        uint8_t oam_data;
        // current VRAM address at addresses 0x2006/0x2007
        uint16_t v;
        // temp VRAM address at addresses 0x2005/0x2006
        uint16_t t;
        // horizontal scroll at address 0x2005
        uint8_t fine_x;
        // VRAM data
        uint8_t vram_data;
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
        // constructor:
        PPU(class BUS *bus); 
        // advance cycle
        void tick();

        // getters
        uint8_t get_ctrl();
        uint8_t get_mask();
        uint8_t get_status();
        uint8_t get_oam_adrr();
        uint8_t get_oam_data();
        uint16_t get_v();
        uint16_t get_t();
        uint8_t get_fine_x();
        uint8_t get_vram_data();
        bool get_write_toggle();
        bool get_nmi();

        // setters
        void set_ctrl(uint8_t value);
        void set_mask(uint8_t value);
        void set_status(uint8_t value);
        void set_oam_addr(uint8_t value);
        void set_oam_data(uint8_t value);
        void set_v(uint16_t value);
        void set_t(uint16_t value);
        void set_fine_x(uint8_t value);
        void set_vram_data(uint8_t value);

        // clears
        void clear_v_blank();
        void clear_write_toggle();
        void clear_nmi();

        // updates
        void update_oam_addr();
        void update_write_toggle();

        // writes to RAM
        void write_vram(uint16_t address, uint8_t value);
        void write_pal_ram(uint16_t address, uint8_t value);
        void write_oam_ram(uint16_t address, uint8_t value);

        // reads to RAM
        uint8_t read_vram(uint16_t address);
        uint8_t read_pal_ram(uint16_t address);
        uint8_t read_oam_ram(uint16_t address);
};
#endif