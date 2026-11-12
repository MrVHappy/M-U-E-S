#include "NES.hpp"
#include "Mapper.hpp"
#include "BUS.hpp"
#include "Cartridge.hpp"
#include "PPU.hpp"
#include "register_bit.hpp"
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

    std::cout << "PPU CPU BUS INTERGRATION TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    bus.reset_state();
    ppu.reset_state();

    ppu.set_mask(0x00);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(64);

    ppu.set_low_shift(0x00);
    ppu.set_high_shift(0x00);
    ppu.set_fine_x(0);
    ppu.set_pal_state(0);
    ppu.set_oam_ram(0xFF);

    ppu.write_oam_ram(0,0x00);
    ppu.write_oam_ram(1,0x02);
    ppu.write_oam_ram(2,0x00);
    ppu.write_oam_ram(3,0x08);

    ppu.tick();
    ppu.tick();

    if(ppu.get_sprite_oam()[0] == 0x00){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_original_oam_index()[0] == 0x00){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    ppu.write_sprite_pattern_low(0,0xFF);
    ppu.write_sprite_pattern_high(0,0x00);

    ppu.set_mask(0x1E);
    ppu.set_dot_count(9);
    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x0000);
    ppu.set_fine_x(0);

    ppu.tick();

    if(ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0x40){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "STATUS: " << static_cast<int>(ppu.get_status()) << std::endl;
        std::cout << "EXPECTED: " << 0x40 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    int status = bus.read(0x2002);

    if(status == 0x40){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "STATUS: " << status << std::endl;
        std::cout << "EXPECTED: " << 0x40 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "STATUS: " << static_cast<int>(ppu.get_status()) << std::endl;
        std::cout << "EXPECTED: " << 0 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(status == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "STATUS: " << status << std::endl;
        std::cout << "EXPECTED: " << 0 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    
    std::cout << "TEST 2" << std::endl;
    ppu.reset_state();
    bus.reset_state();

    ppu.update_v_blank();

    if(ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(status == 0x80){
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
    if(ppu.get_status() == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(status == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    ppu.reset_state();
    bus.reset_state();

    ppu.update_sprite_overflow();

    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(status == 0x20){
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
    if(ppu.get_status() == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(status == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    ppu.reset_state();
    bus.reset_state();

    ppu.update_sprite_overflow();
    ppu.update_sprite_0_hit();
    ppu.update_v_blank();

    if(ppu.get_sprite_overflow()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_sprite_0_hit()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl; 
    }
    if(ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0xE0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(status == 0xE0){
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
    if(ppu.get_status() == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL:" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    ppu.reset_state();
    bus.reset_state();

    ppu.set_v(0x2000);
    ppu.set_t(0x0000);

    bus.write(0x2005,0x12);

    if(ppu.get_write_toggle()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(!ppu.get_write_toggle()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.reset_state();
    bus.reset_state();

    bus.write(0x2000,0x80);
    
    if(ppu.get_nmi_output()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    ppu.update_v_blank();
    ppu.update_nmi();

    if(ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_nmi()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "STATUS: " << static_cast<int>(ppu.get_status()) << std::endl;
        std::cout << "EXPECTED: " << 0 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    status = bus.read(0x2002);

    if(status == 0x80){
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
    if(!ppu.get_nmi()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "NMI: TRUE" << std::endl;
        std::cout << "EXPECTED: FALSE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "STATUS: " << static_cast<int>(ppu.get_status()) << std::endl;
        std::cout << "EXPECTED: " << 0x80 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "NMI INTEGRATION TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    nes.reset_state();
    ppu.reset_state();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_prev_nmi_line(false);

    ppu.set_scan_ln_count(240);
    ppu.set_dot_count(340);
    ppu.update_nmi_output();
    ppu.clear_v_blank();
    ppu.clear_nmi();

    bus.set_NOP_sys_ram();
    rom.set_PRG_NOP();
    rom.set_FA_FB(0x00,0x80);

    nes.execute();
    
    if(ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_nmi_output()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_pc() == 0x8000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x1FD) == 0xC0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x1FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    ppu.reset_state();
    nes.reset_state();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_prev_nmi_line(false);

    ppu.set_scan_ln_count(240);
    ppu.set_dot_count(340);
    ppu.clear_nmi_output();
    ppu.clear_v_blank();
    ppu.clear_nmi();

    rom.set_PRG_NOP();
    bus.set_NOP_sys_ram();

    nes.execute();

    if(ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_nmi_output()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_pc() == 0xC001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_prev_nmi_line()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    ppu.reset_state();
    nes.reset_state();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_prev_nmi_line(false);

    ppu.set_scan_ln_count(241);
    ppu.set_dot_count(10);
    ppu.update_v_blank();
    ppu.update_nmi_output();
    ppu.clear_nmi();

    bus.write(0x2000,0x80);

    rom.set_PRG_NOP();
    rom.set_FA_FB(0x00,0x80);

    nes.execute();

    if(nes.get_pc() == 0x8000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_prev_nmi_line()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0xC0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    ppu.reset_state();
    nes.reset_state();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_prev_nmi_line(true);

    ppu.set_scan_ln_count(241);
    ppu.set_dot_count(10);
    ppu.update_v_blank();
    ppu.update_nmi_output();
    ppu.update_nmi();

    bus.write(0x2000,0x00);

    if(ppu.get_v_blank()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(ppu.get_status() == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_nmi_output()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!ppu.get_nmi()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    ppu.reset_state();
    nes.reset_state();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_prev_nmi_line(false);

    ppu.update_v_blank();
    ppu.update_nmi_output();

    rom.set_PRG_NOP();
    rom.set_FA_FB(0x34,0x12);

    nes.execute();

    if(nes.get_pc() == 0x1234){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0xC0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.reset_state();
    nes.reset_state();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_prev_nmi_line(false);

    ppu.update_v_blank();
    ppu.set_nmi_output(true);

    rom.set_PRG_NOP();
    rom.set_FA_FB(0x00,0x80);

    nes.execute();

    if(nes.get_pc() == 0x8000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_prev_nmi_line()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.execute();

    if(nes.get_pc() == 0x8001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_prev_nmi_line()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 7" << std::endl;
    ppu.reset_state();
    nes.reset_state();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_prev_nmi_line(false);

    ppu.update_v_blank();
    ppu.update_nmi_output();

    rom.set_PRG_NOP();
    rom.set_FA_FB(0x00,0x80);

    nes.execute();

    if(nes.get_pc() == 0x8000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    ppu.clear_v_blank();
    ppu.update_nmi();

    nes.execute();

    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_prev_nmi_line()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    ppu.update_v_blank();
    ppu.update_nmi();

    nes.execute();

    if(nes.get_pc() == 0x8000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xF7){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_prev_nmi_line()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "BRK AND RTI TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl; 
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0xC0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x02){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();
    
    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(bus.read(0x01FB) == 0x34){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFA);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x40);

    bus.write_sys_ram(0x01FB,0xFF);
    bus.write_sys_ram(0x01FC,0x34);
    bus.write_sys_ram(0x01FD,0x92);

    nes.execute();

    if(nes.get_pc() == 0x9234){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag() == 0xEF){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0xA1);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x1000,0x40);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0xC0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x02){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0xB1){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.execute();
    if(nes.get_pc() == 0xC002){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag() == 0xA1){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 5 A" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();

    nes.set_pc(0xC000);
    nes.set_status_flag(0x24);

    rom.db_write_PRG(0x4000,0x58);

    nes.execute();

    if(nes.get_pc() == 0xC001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 5 B" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();

    nes.set_pc(0xC000);
    nes.set_status_flag(0x20);

    rom.db_write_PRG(0x4000,0x78);

    nes.execute();

    if(nes.get_pc() == 0xC001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "PASS" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0x00);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x0100) == 0xC0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FF) == 0x02){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FE) == 0x34){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 7" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFC);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x40);
    
    bus.write_sys_ram(0x01FD,0xFF);
    bus.write_sys_ram(0x01FE,0x34);
    bus.write_sys_ram(0x01FF,0x92);

    nes.execute();

    if(nes.get_pc() == 0x9234){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFF){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag() == 0xEF){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 8 A" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC0FE);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x40FE,0x00);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0xC1){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x00){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 8 B" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC0FF);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x40FF,0x00);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0xC1){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 9 A" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x00);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x1000,0x40);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.execute();

    if(nes.get_pc() == 0xC002){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag() == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 9 B" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0xFF);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x1000,0x40);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();
    
    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.execute();

    if(nes.get_pc() == 0xC002){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag() == 0xEF){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 9 C" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x41);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x1000,0x40);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.execute();

    if(nes.get_pc() == 0xC002){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag() == 0x61){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 9 D" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x8A);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x1000,0x40);
    rom.db_write_PRG(0x7FFE,0x00);
    rom.db_write_PRG(0x7FFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.execute();

    if(nes.get_pc() == 0xC002){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag() == 0xAA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 10 A" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x7FFE,0x23);
    rom.db_write_PRG(0x7FFF,0x81);

    nes.execute();

    if(nes.get_pc() == 0x8123){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 10 B" << std::endl;
    ppu.reset_state();
    nes.reset_state();
    rom.reset_cart();
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    nes.set_pc(0xC000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_prev_nmi_line(false);

    rom.db_write_PRG(0x4000,0x00);
    rom.db_write_PRG(0x7FFE,0xEF);
    rom.db_write_PRG(0x7FFF,0xBE);

    nes.execute();

    if(nes.get_pc() == 0xBEEF){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "IRQ REQUEST TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    nes.reset_state();

    if(!nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    nes.reset_state();
    nes.set_irq_request(true);

    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    nes.reset_state();
    nes.set_irq_request(true);
    
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.set_irq_request(false);

    if(!nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    nes.set_irq_request(true);

    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.reset_state();

    if(!nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "IRQ SERVICING TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    nes.reset_state();
    rom.reset_cart();
    bus.reset_state();
    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);
    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();
    
    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    nes.reset_state();
    rom.reset_cart();
    bus.reset_state();
    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_irq_request(true);
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    rom.db_write_PRG(0x8000,0xEA);

    nes.execute();

    if(nes.get_pc() == 0x8001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();
    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0xE9);
    nes.set_irq_request(true);
    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    rom.db_write_PRG(0x8000,0xEA);

    nes.execute();

    if(bus.read(0x01FB) == 0xE9){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_status_flag()[bit_index(register_bit::B)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::DASH)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();
    nes.set_pc(0x8000);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);
    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);
    rom.db_write_PRG(0xFFFE,0x34);
    rom.db_write_PRG(0xFFFF,0x12);

    nes.execute();

    if(nes.get_pc() == 0x1234){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();
    nes.set_pc(0x8000);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);
    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);

    int start_cycle = bus.get_cpu_cycles();
    nes.execute();

    if(bus.get_cpu_cycles() == (start_cycle + 9)){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();
    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(false);

    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);

    start_cycle = bus.get_cpu_cycles();

    nes.execute();

    if(nes.get_pc() == 0x8001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.get_cpu_cycles() == (start_cycle + 2)){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 7" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);

    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 8" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);

    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);
    rom.db_write_PRG(0x9000,0x40);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    start_cycle = bus.get_cpu_cycles();

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.get_cpu_cycles() == (start_cycle + 13)){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 9" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);

    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);
    rom.db_write_PRG(0x9000,0x40);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    nes.set_irq_request(false);

    nes.execute();

    if(nes.get_pc() == 0x8001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 10" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);
    nes.set_prev_nmi_line(false);

    ppu.update_v_blank();
    ppu.update_nmi_output();

    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    rom.db_write_PRG(0x8000,0xEA);
    rom.db_write_PRG(0xFFFA,0x34);
    rom.db_write_PRG(0xFFFB,0x12);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x1234){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "IRQ POLLING TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_irq_request(true);

    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    rom.db_write_PRG(0x8000,0x58);
    rom.db_write_PRG(0x8001,0xEA);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);
    
    nes.execute();

    if(nes.get_pc() == 0x8001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "PC: " << static_cast<int>(nes.get_pc()) << std::endl;
        std::cout << "EXPECTED: " << 0x8001 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "SP: " << static_cast<int>(nes.get_stack_ptr()) << std::endl;
        std::cout << "EXPECTED: " << 0xFD << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "I FLAG: TRUE" << std::endl;
        std::cout << "EXPECTED: FALSE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "IRQ: TRUE" << std::endl;
        std::cout << "EXPECTED: FALSE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "PC: " << static_cast<int>(nes.get_pc()) << std::endl;
        std::cout << "EXPECTED: " << 0x9000 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "SP: " << static_cast<int>(nes.get_stack_ptr()) << std::endl;
        std::cout << "EXPECTED: " << 0xFA << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "I FLAG: FALSE" << std::endl;
        std::cout << "EXPECTED: TRUE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "IRQ: FALSE" << std::endl;
        std::cout << "EXPETED: TRUE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FD: " << static_cast<int>(bus.read(0x01FD)) << std::endl;
        std::cout << "EXPECTED: " << 0x80 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x02){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FC: " << static_cast<int>(bus.read(0x01FC)) << std::endl;
        std::cout << "EXPECTED: " << 0x02 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FB: " << static_cast<int>(bus.read(0x01FB)) << std::endl;
        std::cout << "EXPECTED: " << 0x20 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 2" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);

    rom.set_PRG_NOP();
    bus.clear_sys_ram();

    rom.db_write_PRG(0x8000,0x58);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "PC: " << static_cast<int>(nes.get_pc()) << std::endl;
        std::cout << "EXPECTED: " << 0x9000 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "SP: " << static_cast<int>(nes.get_stack_ptr()) << std::endl;
        std::cout << "EXPECTED: " << 0xFA << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(!nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "I FLAG: FALSE" << std::endl;
        std::cout << "EXPECTED: TRUE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "IRQ: FALSE" << std::endl;
        std::cout << "EXPECTED: TRUE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FD: " << static_cast<int>(bus.read(0x01FD)) << std::endl;
        std::cout << "EXPECTED: " << 0x80 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FC: " << static_cast<int>(bus.read(0x01FC)) << std::endl;
        std::cout << "EXPECTED: " << 0x01 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0x20){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FB: " << static_cast<int>(bus.read(0x01FB)) << std::endl;
        std::cout << "EXPECTED: " << 0x20 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x20);
    nes.set_irq_request(true);

    rom.db_write_PRG(0x8000,0x78);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x9000){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "PC: " << static_cast<int>(nes.get_pc()) << std::endl;
        std::cout << "EXPECTED: " << 0x9000 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFA){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "SP: " << static_cast<int>(nes.get_stack_ptr()) << std::endl;
        std::cout << "EXPECTED: " << 0xFA << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "I FLAG: FALSE" << std::endl;
        std::cout << "EXPECTED: TRUE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_irq_requested()){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "IRQ: TRUE" << std::endl;
        std::cout << "EXPECTED: FALSE" << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FD) == 0x80){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FD: " << static_cast<int>(bus.read(0x01FD)) << std::endl;
        std::cout << "EXPECTED: " << 0x80 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FC) == 0x01){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FC: " << static_cast<int>(bus.read(0x01FC)) << std::endl;
        std::cout << "EXPECTED: " << 0x01 << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(bus.read(0x01FB) == 0x24){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "RAM 0x1FB: " << static_cast<int>(bus.read(0x01FB)) << std::endl;
        std::cout << "EXPECTED: " << 0x24 << std::endl;
        std::cout << "FAIL" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    nes.reset_state();
    bus.reset_state();
    rom.reset_cart();

    bus.clear_sys_ram();
    rom.set_PRG_NOP();

    nes.set_pc(0x8000);
    nes.set_stack_ptr(0xFD);
    nes.set_status_flag(0x24);
    nes.set_irq_request(true);

    rom.db_write_PRG(0x8000,0x78);
    rom.db_write_PRG(0xFFFE,0x00);
    rom.db_write_PRG(0xFFFF,0x90);

    nes.execute();

    if(nes.get_pc() == 0x8001){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "PC: " << static_cast<int>(nes.get_pc()) << std::endl;
        std::cout << "EXPECTED: " << 0x8001 << std::endl;
    }
    if(nes.get_stack_ptr() == 0xFD){
        std::cout << "PASS" << std::endl;
    }
    else{
        std::cout << "SP: " << static_cast<int>(nes.get_stack_ptr()) << std::endl;
        std::cout << "EXPECTED: " << 0xFD << std::endl;
        std::cout << "FAIL" << std::endl;
    }
    if(nes.get_status_flag()[bit_index(register_bit::I)]){
        std::cout << "PASS" << std::endl;
    }
    return 0;
}