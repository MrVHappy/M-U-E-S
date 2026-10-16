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
const char* opcode_names[256] = {
    "BRK",    "ORA",    "NOP",    "SLO",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x00-0x07
    "PHP",    "ORA",    "ASL",    "NOP",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x08-0x0F
    "BPL",    "ORA",    "NOP",    "SLO",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x10-0x17
    "CLC",    "ORA",    "NOP",    "SLO",    "NOP",    "ORA",    "ASL",    "SLO",    // 0x18-0x1F

    "JSR",    "AND",    "NOP",    "RLA",    "BIT",    "AND",    "ROL",    "RLA",    // 0x20-0x27
    "PLP",    "AND",    "ROL",    "NOP",    "BIT",    "AND",    "ROL",    "RLA",    // 0x28-0x2F
    "BMI",    "AND",    "NOP",    "RLA",    "NOP",    "AND",    "ROL",    "RLA",    // 0x30-0x37
    "SEC",    "AND",    "NOP",    "RLA",    "NOP",    "AND",    "ROL",    "RLA",    // 0x38-0x3F

    "RTI",    "EOR",    "NOP",    "SRE",    "NOP",    "EOR",    "LSR",    "SRE",    // 0x40-0x47
    "PHA",    "EOR",    "LSR",    "NOP",    "JMP",    "EOR",    "LSR",    "SRE",    // 0x48-0x4F
    "BVC",    "EOR",    "NOP",    "SRE",    "NOP",    "EOR",    "LSR",    "SRE",    // 0x50-0x57
    "CLI",    "EOR",    "NOP",    "SRE",    "NOP",    "EOR",    "LSR",    "SRE",    // 0x58-0x5F

    "RTS",    "ADC",    "NOP",    "RRA",    "NOP",    "ADC",    "ROR",    "RRA",    // 0x60-0x67
    "PLA",    "ADC",    "ROR",    "NOP",    "JMP",    "ADC",    "ROR",    "RRA",    // 0x68-0x6F
    "BVS",    "ADC",    "NOP",    "RRA",    "NOP",    "ADC",    "ROR",    "RRA",    // 0x70-0x77
    "SEI",    "ADC",    "NOP",    "RRA",    "NOP",    "ADC",    "ROR",    "RRA",    // 0x78-0x7F

    "NOP",    "STA",    "NOP",    "SAX",    "STY",    "STA",    "STX",    "SAX",    // 0x80-0x87
    "DEY",    "NOP",    "TXA",    "NOP",    "STY",    "STA",    "STX",    "SAX",    // 0x88-0x8F
    "BCC",    "STA",    "NOP",    "NOP",    "STY",    "STA",    "STX",    "SAX",    // 0x90-0x97
    "TYA",    "STA",    "TXS",    "NOP",    "NOP",    "STA",    "NOP",    "NOP",    // 0x98-0x9F

    "LDY",    "LDA",    "LDX",    "LAX",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xA0-0xA7
    "TAY",    "LDA",    "TAX",    "LAX",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xA8-0xAF
    "BCS",    "LDA",    "NOP",    "LAX",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xB0-0xB7
    "CLV",    "LDA",    "TSX",    "NOP",    "LDY",    "LDA",    "LDX",    "LAX",    // 0xB8-0xBF

    "CPY",    "CMP",    "NOP",    "DCP",    "CPY",    "CMP",    "DEC",    "DCP",    // 0xC0-0xC7
    "INY",    "CMP",    "DEX",    "NOP",    "CPY",    "CMP",    "DEC",    "DCP",    // 0xC8-0xCF
    "BNE",    "CMP",    "NOP",    "DCP",    "NOP",    "CMP",    "DEC",    "DCP",    // 0xD0-0xD7
    "CLD",    "CMP",    "NOP",    "DCP",    "NOP",    "CMP",    "DEC",    "DCP",    // 0xD8-0xDF

    "CPX",    "SBC",    "NOP",    "ISB",    "CPX",    "SBC",    "INC",    "ISB",    // 0xE0-0xE7
    "INX",    "SBC",    "NOP",    "SBC",    "CPX",    "SBC",    "INC",    "ISB",    // 0xE8-0xEF
    "BEQ",    "SBC",    "NOP",    "ISB",    "NOP",    "SBC",    "INC",    "ISB",    // 0xF0-0xF7
    "SED",    "SBC",    "NOP",    "ISB",    "NOP",    "SBC",    "INC",    "ISB"     // 0xF8-0xFF
};




int main(int argc, char*argv[]){
    // initialisation of the emulator
    BUS bus = BUS();
    // connect the bus with the cartridge
    Cartridge rom = Cartridge(&bus);
    if(!rom.load_ROM("C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\TEST\\nestest.nes")){
        // if ROM is invalid load error
        std::cout << "Error:\t failed to load ROM" << std::endl;
        return 1;
    }
    // connect the cartridge with the bus
    bus.set_cartridge(&rom);
    // connect bus with PPU
    PPU ppu = PPU(&bus);
    ppu.set_ctrl(0);
    // connect PPU with BUS
    bus.set_ppu(&ppu);
    // connect the bus with the CPU
    NES nes = NES(&bus);
    // reset the CPU
    nes.reset();

    ppu.set_mask(0x08);

    std::cout << std::hex << std::setfill('0');

    std::cout << "SPRITE PIXEL RENDERING TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.write_pal_ram(0x3F11,0x2A);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x40] == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();
    
    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0);
    ppu.write_sprite_pattern_high(0,0);

    ppu.write_frame_buffer(0x20,0x40,0x55);

    ppu.tick();
    if(ppu.get_frame_buffer()[0x20][0x40] == 0x55){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 3 A" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x1E);
    ppu.set_ctrl(0x00);
    ppu.set_fine_x(0);
    ppu.set_v(0);
    ppu.clear_pal_ram();

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);

    ppu.clear_sprite_0_hit();
    ppu.set_status(0);

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.write_pal_ram(0x3F01,0x11);
    ppu.write_pal_ram(0x3F11,0x2A);

    ppu.write_frame_buffer(0x20,0x40,0x11);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_status() == 0b1000000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_frame_buffer()[0x20][0x40] == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 3 B" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x1E);
    ppu.set_ctrl(0x00);
    ppu.set_fine_x(0);
    ppu.set_v(0);
    ppu.clear_pal_ram();
    ppu.set_pal_state(0);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x20);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.write_pal_ram(0x3F01,0x11);
    ppu.write_pal_ram(0x3F11,0x2A);

    ppu.clear_frame_buffer();
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);

    ppu.tick();

    if(ppu.get_low_shift() == 0x8000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_sprite_0_hit()){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_status() == 0b1000000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_frame_buffer()[0x20][0x40] == 0x11){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();
    
    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x20);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_oam(4,0x20);
    ppu.write_sprite_oam(5,0x01);
    ppu.write_sprite_oam(6,0x01);
    ppu.write_sprite_oam(7,0x40);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.write_sprite_pattern_low(1,0x00);
    ppu.write_sprite_pattern_high(1,0x80);

    ppu.write_pal_ram(0x3F11,0x2A);
    ppu.write_pal_ram(0x3F16,0x35);

    ppu.clear_frame_buffer();

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x40] == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 5" <<std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x1E);
    ppu.set_ctrl(0x00);
    ppu.set_fine_x(0);
    ppu.set_v(0);
    ppu.clear_pal_ram();

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.clear_sprite_0_hit();
    ppu.set_status(0);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_status() == 0b1000000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();

    ppu.set_sprite_patterns(0);

    ppu.write_frame_buffer(0x20,0x40,0x11);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x40] == 0x11){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 7 A" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x40);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();
    ppu.clear_frame_buffer();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);
    ppu.write_pal_ram(0x3F11,0x2A);

    ppu.write_frame_buffer(0x20,0x3F,0x10);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x3F] == 0x10){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 7 B" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);
    ppu.write_pal_ram(0x3F11,0x2A);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x40] == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 7 C" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x48);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);
    ppu.write_pal_ram(0x3F11,0x2A);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x47] == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 7D" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x49);
    ppu.set_mask(0x10);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);
    ppu.write_pal_ram(0x3F11,0x2A);

    ppu.write_frame_buffer(0x20,0x48,0x22);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x48] == 0x22){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    std::cout << "TEST 8" << std::endl;
    ppu.set_scan_ln_count(0x20);
    ppu.set_dot_count(0x41);
    ppu.set_mask(0x00);
    ppu.set_ctrl(0x00);
    ppu.clear_sprite_0_hit();
    ppu.set_status(0);
    ppu.clear_pal_ram();

    ppu.clear_sprite_oam();

    ppu.write_sprite_oam(0,0x20);
    ppu.write_sprite_oam(1,0x00);
    ppu.write_sprite_oam(2,0x00);
    ppu.write_sprite_oam(3,0x40);

    ppu.write_sprite_pattern_low(0,0x80);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.write_frame_buffer(0x20,0x40,0x11);

    ppu.tick();

    if(ppu.get_frame_buffer()[0x20][0x40] == 0x11){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }


    std::cout << "UPDATED BUS PPU DATA READ AND WRITES" << std::endl;
    std::cout << "TEST 1" << std::endl;
    ppu.set_ctrl(0);
    ppu.set_v(0x2000);
    ppu.clear_vram();
    ppu.write_vram(0x2000,00);
    ppu.set_vram_data(0);

    bus.write(0x2007,0x42);
    ppu.set_v(0x2000);

    int result = bus.read(0x2007);
    result = bus.read(0x2007);

    if( result == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "RESULT: " << result << std::endl;
        std::cout << "EXPECTED: " << 0x42 << std::endl;
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x2002){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "V: " << static_cast<int>(ppu.get_v()) << std::endl;
        std::cout << "EXPECTED: " << 0x2001 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    ppu.clear_vram();
    ppu.set_ctrl(0);
    ppu.set_v(0x2000);
    bus.write(0x2007,0x42);
    bus.write(0x2007,0x56);
    ppu.set_vram_data(0);
    ppu.set_v(0x2000);

    if(bus.read(0x2007) == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x2001){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "VRAM DATA: " << static_cast<int>(ppu.get_vram_data()) << std::endl;
        std::cout << "EXPECTED: " << 0x42 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    if(bus.read(0x2007) == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x2002){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x56){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    std::cout << "TEST 4" << std::endl;
    ppu.set_ctrl(0x04);
    ppu.clear_vram();
    ppu.set_v(0x2000);
    bus.write(0x2007,0x42);
    ppu.set_vram_data(0);
    ppu.set_v(0x2000);

    if(bus.read(0x2007) == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x2020){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    ppu.clear_pal_ram();
    ppu.set_ctrl(0);
    ppu.set_v(0x3F00);
    bus.write(0x2007,0x2A);
    ppu.set_vram_data(0x55);
    ppu.set_v(0x3F00);

    if(bus.read(0x2007) == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x3F01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.set_ctrl(0x0);
    ppu.write_pal_ram(0x3F00,0x2A);
    ppu.set_v(0x3F10);

    if(bus.read(0x2007) == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x3F11){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 7" << std::endl;
    ppu.clear_vram();
    ppu.clear_pal_ram();
    ppu.set_ctrl(0);

    ppu.write_pal_ram(0x3F00,0x2A);

    ppu.write_vram(0x2F00,0x66);

    ppu.set_vram_data(0x00);
    ppu.set_v(0x3F00);

    result = bus.read(0x2007);

    if(result == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x66){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x3F01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 8" << std::endl;
    rom.set_CHR(0);
    rom.get_mapper_info().write_CHR(0,0x42);
    rom.get_mapper_info().write_CHR(1,0x56);

    ppu.set_ctrl(0);
    ppu.set_vram_data(0);
    ppu.set_v(0);

    result = bus.read(0x2007);

    if(result == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    result = bus.read(0x2007);

    if(result == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x56){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 9" << std::endl;
    ppu.clear_vram();
    ppu.write_vram(0x2000,0x42);

    ppu.set_ctrl(0);
    ppu.set_vram_data(0);
    ppu.set_v(0x3000);

    result = bus.read(0x2007);

    if(result == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x3001){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    result = bus.read(0x2007);

    if(result == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x3002){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 10" << std::endl;
    ppu.clear_vram();
    ppu.write_vram(0x2000,0x42);
    ppu.write_vram(0x2020,0x56);

    ppu.set_ctrl(0x04);
    ppu.set_vram_data(0);
    ppu.set_v(0x2000);

    result = bus.read(0x2007);

    if(result == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x2020){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    result = bus.read(0x2007);

    if(result == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_vram_data() == 0x56){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x2040){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 11 A" << std::endl;
    ppu.set_ctrl(0);
    ppu.set_v(0x7FFF);

    bus.read(0x2007);

    if(ppu.get_v() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 11 B" << std::endl;
    ppu.set_ctrl(0x04);
    ppu.set_v(0x7FE0);

    bus.read(0x2007);

    if(ppu.get_v() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 12" << std::endl;
    ppu.clear_pal_ram();
    ppu.write_pal_ram(0x3F00,0x2A);

    ppu.set_ctrl(0);
    ppu.set_vram_data(0x66);
    ppu.set_v(0x3F00);

    result = bus.read(0x2007);

    if(result == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0x3F01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    return 0;
}