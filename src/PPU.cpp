#include "PPU.hpp"
#include "BUS.hpp"
#include "Cartridge.hpp"
#include "Mapper.hpp"
PPU::PPU(BUS *bus){
    this->bus = bus;
}

void PPU::tick(){
    // 0-239 rendering
    if((this->scan_ln_count < 240) && (this->dot_count >=1) && (this->dot_count < 257)){
        // get the nametable tile address
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
        // get the tile index 
        uint8_t tile_index = (this->dot_count - 1) / 8;
        // get the tile fetch sequence
        uint8_t tile_fetch_seq = (this->dot_count - 1) % 8;    
        switch(tile_fetch_seq){
            // nametable processing
            case 0:{
                // read nametable
                uint8_t vram_data = this->read_vram(address);
                // store in the tile buffer
                this->tile_buffer[tile_index] = vram_data;
                break;
            }
            // attribute processing
            case 2:{
                // calculate the attribute address
                address = 0x23C0 + (this->v & 0x0C00)
                    + ((this->v >> 4) & 0x38) 
                    + ((this->v >> 2) & 0x07);
                
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

                // extract the mirrored address from vram
                uint8_t attribute_byte = this->read_vram(address);
                // store in the tile buffer
                this->attribute_buffer[tile_index] = attribute_byte;
                break;
            }
            // Pattern low byte fetch
            case 4:{
                // extract bit 4 from ctrl register
                bool bit_4 = (this->ctrl & 0b00010000) >> 4;
                // extract the tile number from the tile buffer at tile index
                uint8_t tile_num = tile_buffer[tile_index];
                // extract fine y from v register
                uint8_t fine_y = (this->v & 0b11100000000000) >> 12;
                // calculate the low byte
                uint16_t low_addr;
                if(bit_4){
                    // pattern table is 0x1000
                    low_addr = 0x1000 + (tile_num * 16) + fine_y;
                }
                else{
                    // pattern table is 0
                    low_addr = (tile_num * 16) + fine_y;
                }
                // extract the contents from CHR at index low addr and store in pattern low
                this->pattern_low = this->bus->get_rom().get_mapper_info().read_CHR(low_addr);
                break;
            }
            case 6:{
                // extract bit 4 from ctrl register
                bool bit_4 = (this->ctrl & 0b00010000) >> 4;
                // extract the tile number from the tile buffer at tile index
                uint8_t tile_num = tile_buffer[tile_index];
                // extract fine y from v register
                uint8_t fine_y = (this->v & 0b11100000000000) >> 12;
                // calculate the low byte
                uint16_t high_addr;
                if(bit_4){
                    // pattern table is 0x1000
                    high_addr = 0x1000 + (tile_num * 16) + fine_y + 8;
                }
                else{
                    // pattern table is 0
                    high_addr = (tile_num * 16) + fine_y + 8;
                }
                // extract the contents from CHR at index low addr and store in pattern high
                this->pattern_high = this->bus->get_rom().get_mapper_info().read_CHR(high_addr);
                break;
            }
            // tile boundary loading
            case 7:{
                // remove the bits 0-7 from low shift
                this->low_shift = this->low_shift & 0b1111111100000000;
                // then add pattern low to bits 0-7
                this->low_shift = this->low_shift | this->pattern_low;
                // remove the bits 0-7 from high shift
                this->high_shift = this->high_shift & 0b1111111100000000;
                // then add pattern high to bits 0-7
                this->high_shift = this->high_shift | this->pattern_high;
                // extract coarse x from register v
                uint8_t coarse_x = this->v & 0b1111;
                // extract coarse y from register v
                uint8_t coarse_y = (this->v & 0b11110000) >> 4;
                // get the horizontal quadrant from bit 1 of coarse x
                bool horizontal_quad = (coarse_x & 0b0010) >> 1;
                // get the vertical quadrant from bit 1 of coarse y
                bool vertical_quad = (coarse_y & 0b0010) >> 1;
                // get the attribute byte from attribute buffer at tile index
                uint8_t attribute_byte = this->attribute_buffer[tile_index];
                // check which attribute bit to use
                if((horizontal_quad == 0) && (vertical_quad == 0)){

                }
                else if((horizontal_quad == 0) && (vertical_quad == 1)){

                }
                else if((horizontal_quad == 1) && (vertical_quad == 0)){

                }
                else{

                }
                break;
            }
        }
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

    // increment dot count
    this->dot_count++;
    // check if there have been 340 dots in order to advance the scan line
    if(this->dot_count == 341){
        // increment scan ln count and MOD 262
        this->scan_ln_count = (this->scan_ln_count + 1) % 262;
    }
    // wrap dot count back to 0
    this->dot_count = 0;
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

