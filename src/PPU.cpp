#include "PPU.hpp"
#include "BUS.hpp"
#include "Cartridge.hpp"
#include "Mapper.hpp"
#include <iostream>
PPU::PPU(BUS *bus){
    this->bus = bus;
}

void PPU::tick(){
    // 1-239 rendering
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
        // left shift low and high shift by 1
        this->low_shift = this->low_shift << 1;
        this->high_shift = this->high_shift << 1;

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
                uint8_t coarse_x = this->v & 0b11111;
                // extract coarse y from register v
                uint8_t coarse_y = (this->v & 0b1111100000) >> 5;
                // get the horizontal quadrant from bit 1 of coarse x
                bool horizontal_quad = (coarse_x & 0b0010) >> 1;
                // get the vertical quadrant from bit 1 of coarse y
                bool vertical_quad = (coarse_y & 0b0010) >> 1;
                // get the attribute byte from attribute buffer at tile index
                uint8_t attribute_byte = this->attribute_buffer[tile_index];
                // check which attribute bit to use
                if((horizontal_quad == 0) && (vertical_quad == 0)){
                    // only use bits 0-1
                    attribute_byte = attribute_byte & 0b00000011;
                }
                else if((horizontal_quad == 1) && (vertical_quad == 0)){
                    // only use bits 2-3
                    attribute_byte = (attribute_byte & 0b00001100) >> 2;
                }
                else if((horizontal_quad == 0) && (vertical_quad == 1)){
                    // only use bits 4-5
                    attribute_byte = (attribute_byte & 0b00110000) >> 4;
                }
                else{
                    // only use bits 6-7
                    attribute_byte = (attribute_byte & 0b11000000) >> 6;
                }

                // set the updated attribute byte into pal state
                this->pal_state = attribute_byte;
                break;
            }
        }
        // extract the 15th bit from low shift
        uint8_t low_bit = this->low_shift >> 15;
        // extract the 15th bit from high shift
        uint8_t high_bit = this->high_shift >> 15;

        // calculate pattern value using high and low bit
        uint8_t pattern_val = low_bit + (high_bit * 2);
        uint16_t palelet_val;
        // check if the pattern value is 0
        if(pattern_val == 0){
            // uses a transparent or background pattern
            palelet_val = 0x3F00;
        }
        else{
            // calculate palelet_val using pal state and pattern value
            palelet_val = 0x3F00 + this->pal_state * 4 + pattern_val;
        }
        uint8_t colour_byte;
        // check the current address of v
        if ((palelet_val >= 0x3F00) && (palelet_val < 0x4000)){
            // mask the address to allow mirroring
            uint16_t address = palelet_val;
            address = address % 32;
            // check for special mirroring
            if((address == 0x10) || (address == 0x14) || (address == 0x18) || (address == 0x1C)){
                // mirror to 0x00, 0x04, 0x08 and 0x0C
                address = address & 0x0F;
            }
            // get the colour byte from PAL RAM using address 
            colour_byte = this->read_pal_ram(address);
            // get the x coordinate
            uint8_t x = this->dot_count -1;
            // get the y coordinate
            uint8_t y = this->scan_ln_count;
            // store colour byte in the frame buffer
            this->frame_buffer[y][x] = colour_byte;
        }
        // check if the tile fetch sequence is at 7
        if(tile_fetch_seq == 7){
            // get coarse x from v
            uint8_t coarse_x = this->v & 0b11111;
            // get the horizontal bit from v at bit 10
            bool horizontal_bit = (this->v & 0b10000000000) >> 10;
            // check if coarse x is between 0-30
            if(coarse_x < 31){
                // increment coarse x
                coarse_x++;
                // update v
                this->v = this->v &0b1111111111100000;
                this->v = this->v | coarse_x;
            }
            else{
                // reset coarse x
                coarse_x = 0;
                // check if horizontal bit is set at 0 or 1
                if(!horizontal_bit){
                    // set horizontal bit to true (1)
                    horizontal_bit = true;
                    // update v
                    this->v = this->v & 0b1111111111100000;
                    this->v = this->v | 0b10000000000;
                }
                else{
                    // set horizontal bit to false (0)
                    horizontal_bit = false;
                    // update v
                    this->v = this->v &0b1111101111100000;
                }

            }
        }
        // check if dot count is at 256
        if(this->dot_count == 256){
            // get the contents of fine Y
            uint8_t fine_y = (this->v & 0b111000000000000) >> 12;
            // get the contents of coarse Y
            uint8_t coarse_y = (this->v & 0b1111100000) >> 5;
            // get the contents of vertical bit
            bool vertical_bit = (this->v & 0b100000000000) >> 11;
            // check if fine Y if between 0-6
            if(fine_y < 7){
                // increment fine y
                fine_y++;
                // update v
                this->v = this->v & 0b1000111111111111;
                this->v = this->v | (fine_y << 12);
            }
            // check if fine Y is 7
            else if(fine_y == 7){
                // reset fine y to 0
                fine_y = 0;
                // update v
                this->v = this->v & 0b1000111111111111;

                // check if coarse Y is between 0-28
                if(coarse_y < 29){
                    // increment coarse Y
                    coarse_y++;

                    // update v
                    this->v = this->v & 0b1111110000011111;
                    this->v = this->v | (coarse_y << 5);
                }
                // check if coarse Y is 29
                else if(coarse_y == 29){
                    // reset coarse Y
                    coarse_y = 0;
                    // check if the vertical bit is false(0)
                    if(!vertical_bit){
                        // set it to true (1)
                        vertical_bit = true;
                        // update v
                        this->v = this->v & 0b1111010000011111;
                        this->v = this->v | (coarse_y << 5);
                        this->v = this->v | 0b100000000000;
                    }
                    else{
                        // set it to false (0)
                        vertical_bit = false;
                        
                        // update v
                        this->v = this->v & 0b1111010000011111;
                        this->v = this->v | (coarse_y << 5);
                    }
                }
                // check if coarse Y is 30 or 31
                else if(coarse_y >= 30){
                    // reset coarse Y
                    coarse_y = 0;

                    // update v
                    this->v = this->v & 0b1111110000011111;
                }
            }
            
        }      
    }
    // check if dot count is between 257-320
    if((this->dot_count >= 257) && (this->dot_count < 321)){
        // check if dot count is 257
        if(this->dot_count == 257){
            // get coarse X from register t
            uint8_t new_coarse_x = this->t & 0b11111;
            // get horizontal bit from register t
            uint16_t new_horizontal_bit = this->t & 0b10000000000;
            // update v
            this->v = this->v & 0b1111101111100000;
            this->v = this->v | new_coarse_x | new_horizontal_bit;
        }
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
        uint8_t tile_index = (this->dot_count - 257) / 8;
        // get the tile fetch sequence
        uint8_t tile_fetch_seq = (this->dot_count - 257) % 8; 

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
                uint8_t coarse_x = this->v & 0b11111;
                // extract coarse y from register v
                uint8_t coarse_y = (this->v & 0b1111100000) >> 5;
                // get the horizontal quadrant from bit 1 of coarse x
                bool horizontal_quad = (coarse_x & 0b0010) >> 1;
                // get the vertical quadrant from bit 1 of coarse y
                bool vertical_quad = (coarse_y & 0b0010) >> 1;
                // get the attribute byte from attribute buffer at tile index
                uint8_t attribute_byte = this->attribute_buffer[tile_index];
                // check which attribute bit to use
                if((horizontal_quad == 0) && (vertical_quad == 0)){
                    // only use bits 0-1
                    attribute_byte = attribute_byte & 0b00000011;
                }
                else if((horizontal_quad == 1) && (vertical_quad == 0)){
                    // only use bits 2-3
                    attribute_byte = (attribute_byte & 0b00001100) >> 2;
                }
                else if((horizontal_quad == 0) && (vertical_quad == 1)){
                    // only use bits 4-5
                    attribute_byte = (attribute_byte & 0b00110000) >> 4;
                }
                else{
                    // only use bits 6-7
                    attribute_byte = (attribute_byte & 0b11000000) >> 6;
                }

                // set the updated attribute byte into pal state
                this->pal_state = attribute_byte;
                break;
            }            
        }

        // check if the tile fetch sequence is at 7
        if(tile_fetch_seq == 7){
            // get coarse x from v
            uint8_t coarse_x = this->v & 0b11111;
            // get the horizontal bit from v at bit 10
            bool horizontal_bit = (this->v & 0b10000000000) >> 10;
            // check if coarse x is between 0-30
            if(coarse_x < 31){
                // increment coarse x
                coarse_x++;
                // update v
                this->v = this->v &0b1111111111100000;
                this->v = this->v | coarse_x;
            }
            else{
                // reset coarse x
                coarse_x = 0;
                // check if horizontal bit is set at 0 or 1
                if(!horizontal_bit){
                    // set horizontal bit to true (1)
                    horizontal_bit = true;
                    // update v
                    this->v = this->v & 0b1111111111100000;
                    this->v = this->v | 0b10000000000;
                }
                else{
                    // set horizontal bit to false (0)
                    horizontal_bit = false;
                    // update v
                    this->v = this->v &0b1111101111100000;
                }
            }
        }
    }

    // check if dot count is between 321-336
    if((this->dot_count >= 321) && (this->dot_count < 337)){
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
        uint8_t tile_index = (this->dot_count - 321) / 8;
        // get the tile fetch sequence
        uint8_t tile_fetch_seq = (this->dot_count - 321) % 8; 

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
                uint8_t coarse_x = this->v & 0b11111;
                // extract coarse y from register v
                uint8_t coarse_y = (this->v & 0b1111100000) >> 5;
                // get the horizontal quadrant from bit 1 of coarse x
                bool horizontal_quad = (coarse_x & 0b0010) >> 1;
                // get the vertical quadrant from bit 1 of coarse y
                bool vertical_quad = (coarse_y & 0b0010) >> 1;
                // get the attribute byte from attribute buffer at tile index
                uint8_t attribute_byte = this->attribute_buffer[tile_index];
                // check which attribute bit to use
                if((horizontal_quad == 0) && (vertical_quad == 0)){
                    // only use bits 0-1
                    attribute_byte = attribute_byte & 0b00000011;
                }
                else if((horizontal_quad == 1) && (vertical_quad == 0)){
                    // only use bits 2-3
                    attribute_byte = (attribute_byte & 0b00001100) >> 2;
                }
                else if((horizontal_quad == 0) && (vertical_quad == 1)){
                    // only use bits 4-5
                    attribute_byte = (attribute_byte & 0b00110000) >> 4;
                }
                else{
                    // only use bits 6-7
                    attribute_byte = (attribute_byte & 0b11000000) >> 6;
                }

                // set the updated attribute byte into pal state
                this->pal_state = attribute_byte;
                break;
            }
        }
    }
    
    // dummy fetches
    // check if dot count is 337 or 338
    if((this->dot_count == 337) || (this->dot_count == 338)){
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
        // read nametable but don't store it in the tile buffer
        this->read_vram(address);
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

        // check if dot count is between 280 and 304
        if((this->dot_count >= 280) && (this->dot_count < 305)){
            // get coarse Y from register t
            uint16_t new_coarse_y = this->t & 0b1111100000;
            // get vertical bit from register t
            uint16_t new_vertical_bit = this->t & 0b100000000000;
            // get fine y from register t
            uint16_t new_fine_y = this->t & 0b111000000000000;

            // update v
            this->v = this->v & 0b1000010000011111;
            this->v = this->v | new_coarse_y | new_vertical_bit | new_fine_y;
        }
    }

    // increment dot count
    this->dot_count++;
    // check if there have been 340 dots in order to advance the scan line
    if(this->dot_count == 341){
        // increment scan ln count and MOD 262
        this->scan_ln_count = (this->scan_ln_count + 1) % 262;
        // wrap dot count back to 0
        this->dot_count = 0;
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
int PPU::get_scan_ln_count(){
    return this->scan_ln_count;
}
int PPU::get_dot_count(){
    return this->dot_count;
}
std::array<uint8_t,32> PPU::get_tile_buffer(){
    return this->tile_buffer;
}
std::array<uint8_t,32> PPU::get_attribute_buffer(){
    return this->attribute_buffer;
}
uint8_t PPU::get_pattern_low(){
    return this->pattern_low;
}
uint8_t PPU::get_pattern_high(){
    return this->pattern_high;
}
uint16_t PPU::get_low_shift(){
    return this->low_shift;
}
uint16_t PPU::get_high_shift(){
    return this->high_shift;
}
std::array<std::array<uint8_t,256>,240> PPU::get_frame_buffer(){
    return this->frame_buffer;
}
uint8_t PPU::get_pal_state(){
    return this->pal_state;
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
void PPU::set_dot_count(int value){
    this->dot_count = value;
}
void PPU::set_nmi_output(bool value){
    this->nmi_output = value;
}
void PPU::set_pattern_low(uint8_t value){
    this->pattern_low = value;
}
void PPU::set_pattern_high(uint8_t value){
    this->pattern_high = value;
}
void PPU::set_low_shift(uint16_t value){
    this->low_shift = value;
}
void PPU::set_high_shift(uint16_t value){
    this->high_shift = value;
}
void PPU::set_pal_state(uint8_t value){
    this->pal_state = value;
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

// DEBUG FUNCTIONS
void PPU::clear_tile_buffer(){
    for(int i = 0; i < 32; i++){
        this->tile_buffer[i] = 0;
    }
}
void PPU::clear_attribute_buffer(){
    for(int i = 0; i < 32; i++){
        this->attribute_buffer[i] = 0;
    }
}

void PPU::write_tile_buffer(uint8_t addr, uint8_t val){
    this->tile_buffer[addr] = val;
}
void PPU::write_attribute_buffer(uint8_t addr, uint8_t val){
    this->attribute_buffer[addr] = val;

}
