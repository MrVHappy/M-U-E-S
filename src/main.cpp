#include "NES.hpp"
#include "Mapper.hpp"
#include "BUS.hpp"
#include "Cartridge.hpp"
#include "PPU.hpp"
#include <iomanip>
#include <Windows.h>
#define TEST_ROM "C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\TEST\\nestest.nes"
// array for testing:
// Create this in your NES class header or as a module-level constant
// const char* opcode_names[256] = {
//     "BRK",    "ORA",    "NOP",    "SLO",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x00-0x07
//     "PHP",    "ORA",    "ASL",    "NOP",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x08-0x0F
//     "BPL",    "ORA",    "NOP",    "SLO",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x10-0x17
//     "CLC",    "ORA",    "NOP",    "SLO",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x18-0x1F

//     "JSR",    "AND",    "NOP",    "RLA",    "BIT",    "AND",    "ROL",    "RLA",    // 0x20-0x27
//     "PLP",    "AND",    "ROL",    "NOP",    "BIT",    "AND",    "ROL",    "RLA",    // 0x28-0x2F
//     "BMI",    "AND",    "NOP",    "RLA",    "NOP",    "AND",    "ROL",    "RLA",    // 0x30-0x37
//     "SEC",    "AND",    "NOP",    "RLA",    "NOP",    "AND",    "ROL",    "RLA",    // 0x38-0x3F

//     "RTI",    "EOR",    "NOP",    "SRE",    "NOP",    "EOR",    "LSR",    "SRE",    // 0x40-0x47
//     "PHA",    "EOR",    "LSR",    "NOP",    "JMP",    "EOR",    "LSR",    "SRE",    // 0x48-0x4F
//     "BVC",    "EOR",    "NOP",    "SRE",    "NOP",    "EOR",    "LSR",    "SRE",    // 0x50-0x57
//     "CLI",    "EOR",    "NOP",    "SRE",    "NOP",    "EOR",    "LSR",    "SRE",    // 0x58-0x5F

//     "RTS",    "ADC",    "NOP",    "RRA",    "NOP",    "ADC",    "ROR",    "RRA",    // 0x60-0x67
//     "PLA",    "ADC",    "ROR",    "NOP",    "JMP",    "ADC",    "ROR",    "RRA",    // 0x68-0x6F
//     "BVS",    "ADC",    "NOP",    "RRA",    "NOP",    "ADC",    "ROR",    "RRA",    // 0x70-0x77
//     "SEI",    "ADC",    "NOP",    "RRA",    "NOP",    "ADC",    "ROR",    "RRA",    // 0x78-0x7F

//     "NOP",    "STA",    "NOP",    "SAX",    "STY",    "STA",    "STX",    "SAX",    // 0x80-0x87
//     "DEY",    "NOP",    "TXA",    "NOP",    "STY",    "STA",    "STX",    "SAX",    // 0x88-0x8F
//     "BCC",    "STA",    "NOP",    "NOP",    "STY",    "STA",    "STX",    "SAX",    // 0x90-0x97
//     "TYA",    "STA",    "TXS",    "NOP",    "NOP",    "STA",    "NOP",    "NOP",    // 0x98-0x9F

//     "LDY",    "LDA",    "LDX",    "LAX",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xA0-0xA7
//     "TAY",    "LDA",    "TAX",    "LAX",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xA8-0xAF
//     "BCS",    "LDA",    "NOP",    "LAX",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xB0-0xB7
//     "CLV",    "LDA",    "TSX",    "NOP",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xB8-0xBF

//     "CPY",    "CMP",    "NOP",    "DCP",    "CPY",    "CMP",    "DEC",    "DCP",    // 0xC0-0xC7
//     "INY",    "CMP",    "DEX",    "NOP",    "CPY",    "CMP",    "DEC",    "DCP",    // 0xC8-0xCF
//     "BNE",    "CMP",    "NOP",    "DCP",    "NOP",    "CMP",    "DEC",    "DCP",    // 0xD0-0xD7
//     "CLD",    "CMP",    "NOP",    "DCP",    "NOP",    "CMP",    "DEC",    "DCP",    // 0xD8-0xDF

//     "CPX",    "SBC",    "NOP",    "ISB",    "CPX",    "SBC",    "INC",    "ISB",    // 0xE0-0xE7
//     "INX",    "SBC",    "NOP",    "SBC",    "CPX",    "SBC",    "INC",    "ISB",    // 0xE8-0xEF
//     "BEQ",    "SBC",    "NOP",    "ISB",    "NOP",    "SBC",    "INC",    "ISB",    // 0xF0-0xF7
//     "SED",    "SBC",    "NOP",    "ISB",    "NOP",    "SBC",    "INC",    "ISB"     // 0xF8-0xFF
// };


#define DEBUG

int main(int argc, char*argv[]){
    // initialisation of the emulator
    BUS bus = BUS();
    // connect the bus with the cartridge
    Cartridge rom = Cartridge(&bus);
    rom.get_mapper_info().set_cart_info(&rom);
    #ifdef DEBUG
        rom.reset_cart();
    #endif
    // if(!rom.load_ROM("C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\TEST\\nestest.nes")){
    //     // if ROM is invalid load error
    //     std::cout << "Error:\t failed to load ROM" << std::endl;
    //     rom.reset_cart();
    // }
    // // connect the cartridge with the bus
    bus.set_cartridge(&rom);
    // connect bus with PPU
    PPU ppu = PPU(&bus);
    // connect PPU with BUS
    bus.set_ppu(&ppu);
    // connect the bus with the CPU
    NES nes = NES(&bus);
    // reset the CPU
    nes.reset();

    ppu.set_mask(0x08);
    
    std::cout << std::hex << std::setfill('0');

    std::cout << "SPRITE BACKGROUND INTERGRATION TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x18);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_fine_x(0);

    ppu.db_write_pal_ram(0x3F01,0x21);
    ppu.clear_frame_buffer();
    ppu.write_frame_buffer(0x20,0x10,0x21);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_pattern_low(0,0x00);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x10] == 0x21){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x18);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x0000);
    ppu.set_high_shift(0x0000);

    ppu.db_write_pal_ram(0x3F00,0x0F);
    ppu.db_write_pal_ram(0x3F11,0x25);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x10] == 0x25){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "INDEX $20,$10: " << static_cast<int>(ppu.get_frame_buffer()[0x20][0x10]) << std::endl;
        std::cout << "EXPECTED: " << 0x25 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x18);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_fine_x(0);

    ppu.set_pal_state(0);
    ppu.db_write_pal_ram(0x3F01,0x21);
    ppu.db_write_pal_ram(0x3F11,0x25);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x10] == 0x25){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x18);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_fine_x(0);

    ppu.set_pal_state(0);
    ppu.db_write_pal_ram(0x3F01,0x21);
    ppu.db_write_pal_ram(0x3F11,0x25);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x20);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x10] == 0x21){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x18);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_fine_x(0);

    ppu.set_pal_state(0);
    ppu.db_write_pal_ram(0x3F01,0x21);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x18);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_fine_x(0);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_pattern_low(0,0x00);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 7" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x12);

    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_fine_x(0);
    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x00);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 8" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);

    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_fine_x(0);
    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x00);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 9" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);

    ppu.set_scan_ln_count(241);
    ppu.set_dot_count(1);

    ppu.set_fine_x(0);
    ppu.set_low_shift(0x4000);
    ppu.set_low_shift(0x0000);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x00);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 10" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(256);

    ppu.set_fine_x(0);
    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,255);

    ppu.write_sprite_pattern_low(0,0x01);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 11" << std::endl;
    ppu.set_mask(0x1E);
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x11);
    ppu.set_fine_x(0);
    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    ppu.set_dot_count(0x30);
    ppu.set_low_shift(0x0000);
    ppu.set_high_shift(0x0000);
    ppu.write_sprite_pattern_low(0,0x00);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 12" << std::endl;
    ppu.reset_state();
    ppu.update_sprite_0_hit();
    int result = bus.read(0x2002);
    if(result == 0x40){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 13" << std::endl;
    ppu.reset_state();
    ppu.update_v_blank();

    result = bus.read(0x2002);
    if(result == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 14" << std::endl;
    ppu.reset_state();
    ppu.update_write_toggle();
    bus.read(0x2002);
    if(!ppu.get_write_toggle()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 15" << std::endl;
    ppu.reset_state();
    ppu.update_sprite_overflow();
    result = bus.read(0x2002);
    if((result & 0x20) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 16" << std::endl;
    ppu.reset_state();
    ppu.update_sprite_overflow();
    ppu.update_sprite_0_hit();
    ppu.update_v_blank();

    result = bus.read(0x2002);
    
    if((result & 0xE0) == 0xE0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 17" << std::endl;
    ppu.reset_state();
    ppu.clear_sprite_overflow();
    ppu.clear_sprite_0_hit();
    ppu.clear_v_blank();

    result = bus.read(0x2002);

    ppu.clear_sprite_overflow();
    ppu.clear_sprite_0_hit();
    ppu.clear_v_blank();

    result = bus.read(0x2002);

    if((result & 0xE0) == 0x00){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 18" << std::endl;
    ppu.reset_state();

    ppu.update_sprite_overflow();
    ppu.update_sprite_0_hit();
    ppu.update_v_blank();

    result = bus.read(0x2002);

    if((result & 0xE0) == 0xE0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    result = bus.read(0x2002);

    if((result & 0x80) == 0x00){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if((result & 0x40) == 0x00){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    if((result & 0x20) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 19" << std::endl;
    ppu.reset_state();

    ppu.update_sprite_overflow();
    ppu.update_sprite_0_hit();
    ppu.update_v_blank();
    ppu.update_write_toggle();

    result = bus.read(0x2002);

    if((result & 0xE0) == 0xE0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_write_toggle()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 20" << std::endl;
    ppu.reset_state();

    ppu.update_sprite_overflow();
    ppu.clear_sprite_0_hit();
    ppu.clear_v_blank();

    result = bus.read(0x2002);

    if((result & 0xE0) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    result = bus.read(0x2002);

    if((result & 0xE0) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "SPRITE 0 HIT PIPELINE TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1A);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x00);

    ppu.tick();

    if(ppu.get_frame_buffer()[0][0] == 0x25){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(9);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x08);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(9);

    ppu.set_low_shift(0x0000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x00);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x08);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(9);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0x00);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0x00);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0x00);
    ppu.write_sprite_pattern_low(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x08);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x16);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(9);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x08);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x0E);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(9);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x08);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 7" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1C);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x00);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 8" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1A);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 9" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);
    rom.set_CHR(0);

    ppu.clear_sprite_0_hit();
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.db_write_vram(0x0000,0x01);
    ppu.db_write_vram(0x0001,0x00);
    ppu.db_write_vram(0x03C0,0x00);

    rom.get_mapper_info().write_CHR(0x20,0xFF);
    rom.get_mapper_info().write_CHR(0x28,0x00);
    rom.get_mapper_info().write_CHR(0x10,0xFF);
    rom.get_mapper_info().write_CHR(0x18,0x00);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.db_write_pal_ram(0x01,0x21);
    ppu.db_write_pal_ram(0x11,0x25);

    ppu.write_sprite_oam(0,0x00);
    ppu.write_sprite_oam(1,0x02);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0xFF);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 10" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);
    ppu.set_ctrl(0);

    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(17);

    ppu.set_low_shift(0x7FFF);
    ppu.set_high_shift(0x0000);
    ppu.set_pal_state(0);

    ppu.set_sprite_patterns(0);

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x01);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x10);

    ppu.write_sprite_oam(4,0x20);
    ppu.write_sprite_oam(5,0x02);
    ppu.write_sprite_oam(6,0x00);
    ppu.write_sprite_oam(7,0x10);

    ppu.write_sprite_pattern_low(0,0x00);
    ppu.write_sprite_pattern_high(0,0x00);
    ppu.write_sprite_pattern_low(1,0xFF);
    ppu.write_sprite_pattern_high(1,0x00);

    ppu.db_write_pal_ram(0x3F01,0x21);
    ppu.db_write_pal_ram(0x3F11,0x25);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_frame_buffer()[0x20][16] == 0x25){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 11" << std::endl;
    ppu.reset_state();
    ppu.set_mask(0x1E);
    ppu.set_ctrl(0);

    ppu.set_scan_ln_count(0x20);
    ppu.set_oam_ram(0xFF);

    ppu.write_oam_ram(0,0x10);
    ppu.write_oam_ram(1,0x01);
    ppu.write_oam_ram(2,0x00);
    ppu.write_oam_ram(3,0x10);

    ppu.write_oam_ram(4,0x20);
    ppu.write_oam_ram(5,0x02);
    ppu.write_oam_ram(6,0x00);
    ppu.write_oam_ram(7,0x10);

    for(int dot = 64; dot <= 320; dot++){
        ppu.set_dot_count(dot);
        ppu.tick();
    }

    if(ppu.get_sprite_oam()[0] == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_oam()[3] == 0x10){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.set_low_shift(0x7FFF);
    ppu.set_high_shift(0x0000);

    ppu.set_pal_state(0x00);
    ppu.db_write_pal_ram(0x3F01,0x21);
    ppu.db_write_pal_ram(0x3F11,0x25);

    ppu.set_scan_ln_count(0x21);
    ppu.set_dot_count(17);

    ppu.tick();

    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "0 HIT: TRUE" << std::endl;
        std::cout << "EXPECTED: FALSE" << std::endl;
        std::cout << "FAIL" << std::endl;       
    }
    if(ppu.get_frame_buffer()[0x21][16] == 0x25){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FRAME BUFFER: " << static_cast<int>(ppu.get_frame_buffer()[0x21][16]) << std::endl;
        std::cout << "EXPECTED: " << 0x25 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    if(ppu.get_original_oam_index()[0] == 1){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_original_oam_index()[1] == 255){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    return 0;
}