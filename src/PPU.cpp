#include "PPU.hpp"
#include "BUS.hpp"
#include "Cartridge.hpp"
#include "Mapper.hpp"
PPU::PPU(BUS *bus){
    this->bus = bus;
}

void PPU::tick(){
    // increment cycle count by one
    this->cycle_count++;
    // check if cycle count has reached 341
    if(this->cycle_count == 341){
        // reset cycle count
        this->cycle_count = 0;
        // increment scan ln count and MOD 262
        this->scan_ln_count = (this->scan_ln_count + 1) % 262;

        // 0-239 rendering
        if((this->scan_ln_count < 240) && (cycle_count >=1) && (cycle_count < 257)){
            // get the vram address
            uint16_t address = 0x2000 | (this->v & 0x0FFF);
            // check which mirroring mode will be used
            if(this->bus->get_rom().get_mapper_info().is_vertical()){
                // vertical mapping
                address = address % 0x0800;
            }
            else{
                // horizontal mapping
                if((address >= 0x2000) && (address < 0x2800)){
                    address = address % 0x0400;
                }
                else if((address >= 0x2800) && (address < 0x2C00)){
                    address = (address % 0x0400) + 0x400;
                }
                else{
                    address = address % 0x0800;
                }

            }
            // read vram at the mirrored address
            uint8_t tile_num = this->read_vram(address);
        }
        // 240 post render
        if(this->scan_ln_count == 240){

        }
        // 241-260 VBlank start
        if(this->scan_ln_count == 241){
            // set the Vblank flag in status
            this->v_blank = true;
        }

        // prerender
        if(this->scan_ln_count == 261){
            // clear the VBlank
            this->v_blank = false;
            // clear sprite 0 hit
            this->sprite_0_hit = false;
            // clear sprite overflow
            this->sprite_overflow = false;
            // update the status register
            this->status = this->status & 0b00011111;
        }
    }
}

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
bool PPU::get_nmi(){
    return this->nmi;
}
bool PPU::get_sprite_overflow(){
    return this->sprite_overflow;
}
bool PPU::get_v_blank(){
    return this->v_blank;
}
bool PPU::get_sprite_0_hit(){
    return this->sprite_0_hit;
}
bool PPU::get_nmi_output(){
    return nmi_output;
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
void PPU::set_cycle_count(int value){
    this->cycle_count = value;
}
void PPU::set_scan_ln_count(int value){
    this->scan_ln_count = value;
}
void PPU::set_nmi_output(bool value){
    this->nmi_output = value;
}

void PPU::clear_v_blank(){
    this->v_blank = false;
}
void PPU::clear_write_toggle(){
    this->write_toggle = false;
}
void PPU::clear_nmi(){
    this->nmi = false;
}
void PPU::clear_sprite_overflow(){
    this->sprite_overflow = false;
}
void PPU::clear_sprite_0_hit(){
    this->sprite_0_hit = false;
}

void PPU::update_oam_addr(){
    this->oam_addr++;
}
void PPU::update_write_toggle(){
    this->write_toggle = true;
}
void PPU::update_nmi(){
    this->nmi = true;
}
void PPU::update_sprite_overflow(){
    this->sprite_overflow = true;
}
void PPU::update_sprite_0_hit(){
    this->sprite_0_hit = true;
}
void PPU::update_v_blank(){
    this->v_blank = true;
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

uint8_t PPU::read_vram(uint16_t address){
    return this->vram[address];
}
uint8_t PPU::read_pal_ram(uint16_t address){
    return this->pal_ram[address];
}
uint8_t PPU::read_oam_ram(uint16_t address){
    return this->oam_ram[address];
}
