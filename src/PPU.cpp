#include "PPU.hpp"

uint8_t PPU::get_ctrl(){
    return this->ctrl;
}
uint8_t PPU::get_mask(){
    return this->mask;
}
uint8_t PPU::get_status(){
    return this->status;
}
uint8_t PPU::get_oam_adrr(){
    return this->oam_addr;
}
uint8_t PPU::get_oam_data(){
    return this->oam_data;
}
uint16_t PPU::get_v(){
    return this->v;
}
uint16_t PPU::get_t(){
    return this->t;
}
uint8_t PPU::get_fine_x(){
    return this->fine_x;
}
uint8_t PPU::get_vram_data(){
    return this->vram_data;
}

void PPU::clear_v_blank(){
    this->v_blank = false;
}
void PPU::clear_write_toggle(){
    this->write_toggle = false;
}

void PPU::update_oam_addr(){
    this->oam_addr++;
}