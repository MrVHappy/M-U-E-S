#include "BUS.hpp"
#include "Cartridge.hpp"
#include "Mapper.hpp"
#include "PPU.hpp"

    // read to system RAM
    uint8_t BUS::read(uint16_t address){
        // check if the address value is between 0x0000-0x1FFF
        if (address < 0x2000){
            // CPU data
            return this->sys_ram[address & 0x07FF];
        }
        // check if the address is between 0x2000-0x3FFF
        else if((address >= 0x2000) && (address <= 0x3FFF)){
            // PPU data
            // mask the address to allow mirroring
            uint16_t masked_addr = address & 0x2007;
            // determine which PPU register to use
            switch (masked_addr){
                // PPU STATUS
                case 0x2002:{
                    uint8_t bit_5, bit_6, bit_7;
                    // extract bit 5
                    if (this->ppu->get_sprite_overflow()){
                        // if true mark bit 5 as 1
                        bit_5 = 0b100000;
                    }
                    else{
                        // else mark bit 5 as 0
                        bit_5 = 0;
                    }
                    // extract bit 6
                    if (this->ppu->get_sprite_0_hit()){
                        // if true mark bit 6 as 1
                        bit_6 = 0b1000000;
                    }
                    else{
                        // else mark bit 6 as 0
                        bit_6 = 0;
                    }
                    // extract bit 7
                    if(this->ppu->get_v_blank()){
                        // if true mark bit 7 as 1
                        bit_7 = 0b10000000;
                    }
                    else{
                        // mark bit 7 as 0
                        bit_7 = 0;
                    }
                    // add bits 5, 6, 7 together
                    uint8_t status_change = bit_5 | bit_6 | bit_7;
                    // update ppu status
                    uint8_t ppu_status = 0;
                    ppu_status = ppu_status | status_change;
                    this->ppu->set_status(ppu_status);
                    // clear the v blank and write toggle
                    this->ppu->clear_v_blank();
                    this->ppu->clear_write_toggle();
                    // PPU status register
                    return ppu_status;
                }
                // OAM DATA
                case 0x2004:{
                    // extract OAM DATA via OAM address
                    uint8_t oam_addr = this->ppu->get_oam_adrr();
                    uint8_t oam_data = this->ppu->read_oam_ram(oam_addr);
                    // return OAM data
                    return oam_data;
                }
                // PPU DATA
                case 0x2007:{
                    // create a result int 
                    uint8_t result = 0;
                    // get the current VRAM address
                    uint16_t addr_v = this->ppu->get_v() & 0x3FFF;
                    // check the current address of v
                    if ((addr_v >= 0x3F00) && (addr_v < 0x4000)){
                        // mask the address to allow mirroring
                        uint16_t address = addr_v;
                        address = address % 32;
                        // check for special mirroring
                        if((address == 0x10) || (address == 0x14) || (address == 0x18) || (address == 0x1C)){
                            // mirror to 0x00, 0x04, 0x08 and 0x0C
                            address = address & 0x0F;
                        }
                        result = this->ppu->read_pal_ram(address);
                    }
                    else if ((addr_v >= 0x0000) && (addr_v < 0x2000)){
                        // assign result to the value of vram data
                        result = this->ppu->get_vram_data();
                        // store data from CHR at addr v in vram data before update
                        this->ppu->set_vram_data(this->rom->get_mapper_info().read_CHR(addr_v));
                    }
                    else if((addr_v >= 0x3000) && (addr_v < 0x3F00)){
                        // subtract addr_v by 0x1000 so that it mirrors 0x2000-0x2EFF
                        addr_v-= 0x1000;
                    }
                    if ((addr_v >= 0x2000) && (addr_v < 0x3000)){
                        // check which mirroring mode will be used
                        if(this->rom->get_mapper_info().is_vertical()){
                            // using vertical mapping:
                            addr_v = addr_v % 0x0800;
                        }
                        else{
                            // use horizontal mapping:
                            if((addr_v >= 0x2000) && (addr_v < 0x2800)){
                                addr_v = addr_v % 0x0400;
                            }
                            else if((addr_v >= 0x2800) && (addr_v < 0x2C00)){
                                addr_v = (addr_v % 0x0400) + 0x400;
                            }
                            else{
                                addr_v = addr_v % 0x0800;
                            }
                        }
                        // assign result to the value of vram data
                        result = this->ppu->get_vram_data();
                        // store data in VRAM at addr v in vram data before update
                        this->ppu->set_vram_data(this->ppu->read_vram(addr_v));
                    }
                    
                    // update v based on bit 2 of ctrl register
                    uint8_t bit_2 = (this->ppu->get_ctrl() & 0b00000100) >> 2;
                    uint16_t new_v = this->ppu->get_v();
                    if(bit_2 == 1){
                        // increment by 32
                        new_v = (new_v + 32) % 0x8000;
                    }
                    else{
                        // increment by 1
                        new_v = (new_v + 1) % 0x8000;
                    }
                    // update v
                    this->ppu->set_v(new_v);

                    return result;
                }                    
                default:
                    return 0;
            }
        }
        // check if the address is >= 0x8000
        else if (address >= 0x8000){
            // ROM data
            Mapper *mapper = &this->rom->get_mapper_info();
            // check if mapper is NULL
            if(mapper == NULL){
                return 0;
            }
            return mapper->read_PRG(address);
        }
        return 0;
    }   
        
    void BUS::write(uint16_t address, uint8_t value){
        // write data to system RAM
        if(address < 0x2000){
            this->sys_ram[address & 0x07FF] = value;
        }
        // PPU functionality
        else if((address >= 0x2000) && (address < 0x4000)){
            // mask the address to allow mirroring
            uint16_t masked_addr = address & 0x2007;
            // determine which PPU register to use
            switch (masked_addr){
                // PPU CTRL
                case 0x2000:{
                    // write to ctrl register
                    this->ppu->set_ctrl(value);
                    // extract the nametable value from bits 0-1
                    uint16_t name_table_val = this->ppu->get_ctrl() & 0b00000011;
                    // perform a left shift by 10
                    name_table_val = name_table_val << 10;
                    // mask t to clear bits 10 and 11
                    uint16_t new_t = this->ppu->get_t() & 0b111100111111111;
                    // update t
                    new_t = name_table_val | new_t;
                    this->ppu->set_t(new_t);

                    // extract bit 7
                    uint8_t bit_7 = this->ppu->get_ctrl() >> 7;
                    if(bit_7 == 1){
                        // set nmi to true
                        this->ppu->update_nmi();
                    }
                    else{
                        // set nmi to false
                        this->ppu->clear_nmi();
                    }
                    return;
                }
                // PPU MASK
                case 0x2001:{
                    // write to mask register
                    this->ppu->set_mask(value);
                    return;
                }
                // OAM ADDR
                case 0x2003:{
                    // write to the OAM address register
                    this->ppu->set_oam_addr(value);
                    return;
                }
                // OAM DATA
                case 0x2004:{
                    // write value to OAM RAM at OAM ADDR
                    this->ppu->write_oam_ram(this->ppu->get_oam_adrr(), value);
                    // update the OAM address
                    this->ppu->update_oam_addr();
                    return;
                }
                // PPU SCROLL
                case 0x2005:{
                    // check the value of write toggle
                    if(!this->ppu->get_write_toggle()){
                        // extract the lower 3 bits for fine x
                        uint8_t lower_bits = value & 0b00000111;
                        this->ppu->set_fine_x(lower_bits);
                        // extract the higher 5 bits for t
                        uint8_t higher_bits = (value & 0b11111000) >> 3;
                        // update t
                        uint16_t new_t = (this->ppu->get_t() & 0b111111111100000) | higher_bits;
                        this->ppu->set_t(new_t);
                        // set write toggle to true
                        this->ppu->update_write_toggle();
                    }
                    else{
                        // extract the lower 3 bits for fine y stored in t
                        uint16_t lower_bits = (value & 0b00000111) << 12;
                        // extract the higher 5 bits for coarse Y stored in t
                        uint16_t higher_bits = (value & 0b11111000) << 2;
                        // extract t and clear bits 5-9 and 12-14
                        uint16_t new_t = this->ppu->get_t() & 0b000110000011111;
                        // combine the bits together
                        new_t = new_t | lower_bits | higher_bits;
                        this->ppu->set_t(new_t);
                        // clear the write toggle to false
                        this->ppu->clear_write_toggle();
                    }
                    return;
                }
                // PPU ADDR
                case 0x2006:{
                    // check the value of write toggle
                    if(!this->ppu->get_write_toggle()){
                        // extract the lower 6 bits and shift 8 bits
                        uint16_t lower_bits = (value & 0b00111111) << 8;
                        // clear bits 8-14 from t
                        uint16_t new_t = this->ppu->get_t() & 0b0000000011111111;
                        // place the low bits to t
                        new_t = new_t | lower_bits;
                        // update t
                        this->ppu->set_t(new_t);
                        // update write toggle to true
                        this->ppu->update_write_toggle();
                    }
                    else{
                        // extract the lower 8 bits
                        uint16_t lower_bits = value & 0b11111111;
                        // clear bits 0-7
                        uint16_t new_t = this->ppu->get_t() & 0b1111111100000000;
                        // place the lower buts into t
                        new_t = new_t | lower_bits;
                        // update t
                        this->ppu->set_t(new_t);
                        // copy the whole of t into v
                        this->ppu->set_v(new_t);
                        // clear the write toggle to false
                        this->ppu->clear_write_toggle();
                    }
                    return;
                }
                // PPU DATA
                case 0x2007:{
                    // get the address of v
                    uint16_t addr_v = this->ppu->get_v() & 0x3FFF;
                    // check the current address of v
                    if((addr_v >= 0x3F00) && (addr_v < 0x4000)){
                        // mask the address to allow mirroring
                        uint16_t address = addr_v;
                        address = address % 32;
                        // check for special mirroring
                        if((address == 0x10) || (address == 0x14) || (address == 0x18) || (address == 0x1C)){
                            // mirror to 0x00, 0x04, 0x08 and 0x0C
                            address = address & 0x0F;
                        }
                        // write to pallet RAM
                        this->ppu->write_pal_ram(address, value);

                    }
                    else if((addr_v >= 0x0000) && (addr_v < 0x2000)){
                        // handlling pallet table memory/CHR
                        this->rom->get_mapper_info().write_CHR(addr_v, value);
                    }
                    else if ((addr_v >= 0x3000) && (addr_v < 0x3F00)){
                        // subtract addr_v by 0x1000 so that it mirrors 0x2000-0x2EFF
                        addr_v-= 0x1000;
                    }
                    if((addr_v >= 0x2000) && (addr_v < 0x3000)) {
                        // use addr_v for vram address
                        uint16_t vram_addr = addr_v;
                        // check which mirroring mode will be used
                        if(this->rom->get_mapper_info().is_vertical()){
                            // using vertical mapping:
                            vram_addr = vram_addr % 0x0800;
                        }
                        else{
                            // use horizontal mapping:
                            if((vram_addr >= 0x2000) && (vram_addr < 0x2800)){
                                vram_addr = vram_addr % 0x0400;
                            }
                            else if((vram_addr >= 0x2800) && (vram_addr < 0x2C00)){
                                vram_addr = (vram_addr % 0x0400) + 0x400;
                            }
                            else{
                                vram_addr = vram_addr % 0x0800;
                            }
                        }
                        // write to name table RAM
                        this->ppu->write_vram(vram_addr, value);

                    }
                    // update v based on bit 2 of ctrl register
                    uint8_t bit_2 = (this->ppu->get_ctrl() & 0b00000100) >> 2;
                    uint16_t new_v = this->ppu->get_v();
                    if(bit_2 == 1){
                        // increment by 32
                        new_v = (new_v + 32) % 0x8000;
                    }
                    else{
                        // increment by 1
                        new_v = (new_v + 1) % 0x8000;
                    }
                    // update v
                    this->ppu->set_v(new_v);
                    return;
                }
                default:
                    return;
            }
        }
        // APU and Controller registers
        else if((address >= 0x4000) && (address < 0x4018)){
            // temp
        }
        // write to SRAM
        else if((address >= 0x6000) && (address < 0x8000)){
            // temp
        }
        // Cartridge mapper
        else if((address >= 0x8000) && (address <= 0xFFFF)){
            // temp
        }
    } 

    std::array<uint8_t, 2048> BUS::get_sys_ram(){
        return this->sys_ram;
    }

    void BUS::set_cartridge(Cartridge *rom){
        this->rom = rom;
    }

    void BUS::set_ppu(class PPU *ppu){
        this->ppu = ppu;
    }

    Cartridge & BUS::get_rom(){
        return *this->rom;
    }
    PPU & BUS::get_ppu(){
        return *this->ppu;
    }

