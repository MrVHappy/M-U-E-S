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
bool PPU::get_write_toggle(){
    return this->write_toggle;
}

void PPU::set_ctrl(uint8_t value){
    this->ctrl = value;
}
void PPU::set_mask(uint8_t value){
    this->mask = value;
}
void PPU::set_status(uint8_t value){
    this->status = value;
}
void PPU::set_oam_addr(uint8_t value){
    this->oam_addr = value;
}
void PPU::set_oam_data(uint8_t value){
    this-> oam_data = value;
}
void PPU::set_v(uint16_t value){
    this->v = value;
}
void PPU::set_t(uint16_t value){
    this->t = value;
}
void PPU::set_fine_x(uint8_t value){
    this->fine_x = value;
}
void PPU::set_vram_data(uint8_t value){
    this->vram_data = value;
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
void PPU::update_write_toggle(){
    this->write_toggle = true;
}

void PPU::write_vram(uint16_t address, uint8_t value){
    this->vram[address] = value;
}
void PPU::write_pal_ram(uint16_t address, uint8_t value){
    this->pal_ram[address] = value;
}
void PPU::write_oam_ram(uint16_t address, uint8_t value){
    this->oam_ram[address] = value;
}