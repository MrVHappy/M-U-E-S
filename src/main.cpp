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
    // reset t, v, and clear write toggle
    // std::cout << "TEST 1 A" << std::endl;
    // ppu.clear_write_toggle();
    // ppu.set_t(0);
    // ppu.set_v(0);
    // // test 0x2006 write 0x20
    // bus.write(0x2006,0x20);
    // // validate t and v and write toggle
    // if((ppu.get_t() == 0x2000) && (ppu.get_v() == 0) && (ppu.get_write_toggle() == true)){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 1 B" << std::endl;
    // // test 0x2006 write 0x0
    // bus.write(0x2006,0x0);
    // // validate t and v and write toggle
    // if((ppu.get_t() == 0x2000) && (ppu.get_v() == 0x2000) && (ppu.get_write_toggle() == false)){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }


    // std::cout << "TEST 2" << std::endl;
    // // reset t, v, and clear write toggle
    // ppu.clear_write_toggle();
    // ppu.set_t(0);
    // ppu.set_v(0);
    // // test 0x2006 write 0x3F
    // bus.write(0x2006,0x3F);
    // // test 0x2006 write 0x0
    // bus.write(0x2006,0x0);
    // // validate t and v and write toggle
    // if((ppu.get_t() == 0x3F00) && (ppu.get_v() == 0x3F00) && (ppu.get_write_toggle() == false)){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 3" << std::endl;
    // // test 0x2006 write 0x21
    // bus.write(0x2006,0x21);
    // // test 0x2006 write 0x0
    // bus.write(0x2006,0x65);
    // // validate t and v and write toggle
    // if((ppu.get_t() == 0x2165) && (ppu.get_v() == 0x2165) && (ppu.get_write_toggle() == false)){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 4" << std::endl;
    
    // // reset t, v, and clear write toggle
    // ppu.clear_write_toggle();
    // ppu.set_t(0x0000);
    // ppu.set_v(0x1234);
    // // test 0x2006 write 0x25
    // bus.write(0x2006,0x25);
    // // validate t and v and write toggle
    // if((ppu.get_t() == 0x2500) && (ppu.get_v() == 0x1234) && (ppu.get_write_toggle() == true)){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 5" << std::endl;
    // ppu.clear_write_toggle();
    // bus.write(0x2006,0x20);
    // bus.read(0x2002);
    // if(ppu.get_write_toggle() == false){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2006,0x3F);
    // bus.write(0x2006,0x00);
    // if(ppu.get_v() == 0x3F00){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // std::cout << "TEST 6" << std::endl;
    // ppu.clear_write_toggle();
    // bus.write(0x2005,0x12);
    // if(ppu.get_write_toggle() == true){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2006,0x34);
    // if(ppu.get_write_toggle() == false){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // std::cout << "TEST 7" << std::endl;
    // ppu.clear_write_toggle();
    // bus.write(0x2006,0x20);
    // if(ppu.get_write_toggle() == true){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2005,0x40);
    // if(ppu.get_write_toggle() == false){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // std::cout << "TEST 8" << std::endl;
    // ppu.clear_write_toggle();
    // ppu.set_t(0);
    // ppu.set_v(0);
    // bus.write(0x2006,0x20);
    
    // bus.write(0x2006,0x00);
    // if(ppu.get_v() == 0x2000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.read(0x2007);
    // if(ppu.get_v() == 0x2001){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.read(0x2007);
    // if(ppu.get_v() == 0x2002){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // std::cout << "TEST 9" << std::endl;
    // ppu.set_v(0x2000);
    // ppu.set_ctrl(0b100);
    // bus.read(0x2007);
    // if(ppu.get_v() == 0x2020){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.read(0x2007);
    // if(ppu.get_v() == 0x2040){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // std::cout << "TEST 10" << std::endl;
    // rom.set_header(0);
    // ppu.clear_write_toggle();
    // ppu.set_v(0);
    // ppu.set_t(0);
    
    // bus.write(0x2006, 0x20);
    // bus.write(0x2006, 0x00);
    
    // bus.write(0x2007, 0xBB);

    // bus.write(0x2006, 0x20);
    // bus.write(0x2006, 0x00);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x24);
    // bus.write(0x2006, 0x00);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x28);
    // bus.write(0x2006, 0x00);

    // bus.write(0x2007, 0xCC);

    // bus.write(0x2006, 0x28);
    // bus.write(0x2006, 0x00);
    
    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xCC){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x2C);
    // bus.write(0x2006, 0x00);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xCC){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x20);
    // bus.write(0x2006, 0x00);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 11" << std::endl;
    
    // ppu.clear_write_toggle();
    // ppu.set_v(0);
    // ppu.set_t(0);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x00);

    // bus.write(0x2007, 0x11);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x00);

    // if(bus.read(0x2007) == 0x11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x04);

    // bus.write(0x2007, 0x22);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x04);

    // if(bus.read(0x2007) == 0x22){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x14);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x14);

    // if(bus.read(0x2007) == 0x22){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x08);

    // bus.write(0x2007, 0x33);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x08);

    // if(bus.read(0x2007) == 0x33){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x18);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x18);

    // if(bus.read(0x2007) == 0x33){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x0C);

    // bus.write(0x2007, 0x44);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x0C);

    // if(bus.read(0x2007) == 0x44){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x1C);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x1C);

    // if(bus.read(0x2007) == 0x44){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 12" << std::endl;
    // rom.set_CHR(0);
    // ppu.clear_write_toggle();
    // ppu.set_t(0);
    // ppu.set_v(0);

    // bus.write(0x2006,0x00);
    // bus.write(0x2006,0x00);

    // bus.write(0x2007, 0xAA);

    // bus.write(0x2006,0x00);
    // bus.write(0x2006,0x00);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006,0x00);
    // bus.write(0x2006,0x01);

    // bus.write(0x2007, 0xBB);

    // bus.write(0x2006,0x00);
    // bus.write(0x2006,0x01);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006,0x00);
    // bus.write(0x2006,0x00);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006,0x1F);
    // bus.write(0x2006,0xFF);

    // bus.write(0x2007, 0xCC);

    // bus.write(0x2006,0x1F);
    // bus.write(0x2006,0xFF);

    // bus.read(0x2007);

    // if(bus.read(0x2007) == 0xCC){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 13" << std::endl;
    // ppu.set_t(0);
    // ppu.set_v(0);
    // ppu.clear_write_toggle();

    // bus.write(0x2006,0x00);
    // bus.write(0x2006,0x00);

    // ppu.set_ctrl(0);

    // bus.write(0x2006,0x2F);
    // bus.write(0x2006,0xFE);

    // bus.read(0x2007);

    // if(ppu.get_v() == 0x2FFF){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.read(0x2007);

    // if(ppu.get_v() == 0x3000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006,0x00);
    // bus.write(0x2006,0x00);

    // ppu.set_ctrl(0b00000100);

    // bus.write(0x2006,0x20);
    // bus.write(0x2006,0x00);

    // bus.read(0x2007);

    // if(ppu.get_v() == 0x2020){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.read(0x2007);

    // if(ppu.get_v() == 0x2040){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_ctrl(0);
    // bus.write(0x2006,0x7F);
    // bus.write(0x2006,0xFF);

    // bus.read(0x2007);

    // if(ppu.get_v() == 0x4000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << ppu.get_v() << std::endl;
    // }

    // std::cout << "TEST 14" << std::endl;
    // ppu.set_t(0);
    // ppu.set_v(0);
    // ppu.clear_write_toggle();
    // ppu.set_ctrl(0);

    // bus.write(0x2000,0x80);

    // if(ppu.get_ctrl() == 0x80){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "PPU CTRL TESTS" << std::endl;
    // std::cout << "TEST 15" << std::endl;
    // ppu.set_t(0);
    // ppu.set_v(0);
    // ppu.clear_write_toggle();
    // ppu.set_ctrl(0);

    // bus.write(0x2000,0x01);

    // if(ppu.get_ctrl() == 0x01){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 16" << std::endl;
    // ppu.set_t(0);
    // ppu.set_v(0);
    // ppu.clear_write_toggle();
    // ppu.set_ctrl(0);

    // bus.write(0x2000,0x00);

    // uint8_t test_bits = ppu.get_t() >> 10;
    // if(test_bits == 0b00){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2000,0x01);

    // test_bits = ppu.get_t() >> 10;
    // if(test_bits == 0b01){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2000,0x02);

    // test_bits = ppu.get_t() >> 10;
    // if(test_bits == 0b10){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2000,0x03);

    // test_bits = ppu.get_t() >> 10;
    // if(test_bits == 0b11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    
    // std::cout << "TEST 17" << std::endl;
    // ppu.set_t(0b00000110000101);
    // ppu.set_v(0);
    // ppu.clear_write_toggle();
    // ppu.set_ctrl(0);

    // bus.write(0x2000,0x03);
    // if(ppu.get_t() == 0b00110110000101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 18 A" << std::endl;
    // ppu.set_t(0);
    // ppu.set_v(0);
    // ppu.clear_write_toggle();
    // ppu.set_ctrl(0);

    // bus.write(0x2000,0x00);
    // ppu.set_v(0x2000);

    // bus.write(0x2007, 0x10);

    // if(ppu.get_v() == 0x2001){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 18 B" << std::endl;

    // bus.write(0x2000,0x04);
    // ppu.set_v(0x2000);

    // bus.write(0x2007, 0x10);

    // if(ppu.get_v() == 0x2020){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    
    // std::cout << "TEST 19 A" << std::endl;

    // bus.write(0x2000,0x00);

    // if(ppu.get_nmi() == false){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 19 B" << std::endl;

    // bus.write(0x2000,0x80);

    // if(ppu.get_nmi() == true){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 20" << std::endl;
    // ppu.set_t(0);
    // ppu.set_v(0);
    // ppu.clear_write_toggle();
    // ppu.set_ctrl(0);
    // ppu.clear_nmi();

    // ppu.set_v(0x2000);

    // bus.write(0x2000,0x00);
    // bus.write(0x2007,0x00);

    // if(ppu.get_v() == 0x2001){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_v(0x2000);

    // bus.write(0x2000,0x04);
    // bus.write(0x2007,0x00);

    // if(ppu.get_v() == 0x2020){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "PPU STATUS TESTS" << std::endl;
    // std::cout << "TEST 21" << std::endl;
    // ppu.clear_sprite_0_hit();
    // ppu.clear_sprite_overflow();
    // ppu.clear_v_blank();
    // ppu.clear_write_toggle();

    // bus.read(0x2002);

    // if(ppu.get_status() == 0x0000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 22" << std::endl;
    // ppu.clear_sprite_0_hit();
    // ppu.clear_sprite_overflow();
    // ppu.update_v_blank();
    // ppu.clear_write_toggle();

    // bus.read(0x2002);

    // if(ppu.get_status() == 0b10000000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_v_blank() == false){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 23" << std::endl;
    // ppu.update_sprite_0_hit();
    // ppu.clear_sprite_overflow();
    // ppu.clear_v_blank();
    // ppu.clear_write_toggle();

    // bus.read(0x2002);
    // if(ppu.get_status() == 0b01000000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_sprite_0_hit()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 24" << std::endl;
    // ppu.clear_sprite_0_hit();
    // ppu.update_sprite_overflow();
    // ppu.clear_v_blank();
    // ppu.clear_write_toggle();

    // bus.read(0x2002);

    // if(ppu.get_status() == 0b00100000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_sprite_overflow()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 25" << std::endl;
    // ppu.update_sprite_0_hit();
    // ppu.update_v_blank();
    // ppu.update_sprite_overflow();

    // bus.read(0x2002);
    // if(ppu.get_status() == 0b11100000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_sprite_overflow()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_sprite_0_hit()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_v_blank() == false){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 26" << std::endl;
    // ppu.clear_sprite_0_hit();
    // ppu.clear_sprite_overflow();
    // ppu.clear_v_blank();
    // ppu.update_write_toggle();

    // bus.read(0x2002);
    // if(ppu.get_write_toggle() == false){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 27" << std::endl;
    // ppu.update_sprite_0_hit();
    // ppu.update_v_blank();
    // ppu.update_sprite_overflow();

    // bus.read(0x2002);
    // if(ppu.get_status() == 0b11100000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.clear_sprite_0_hit();
    // ppu.clear_v_blank();
    // ppu.clear_sprite_overflow();

    // bus.read(0x2002);
    // if(ppu.get_status() == 0){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "PPU OAM ADDR TEST" << std::endl;
    // std::cout << "TEST 28" << std::endl;

    // bus.write(0x2003, 0x00);

    // if(ppu.get_oam_adrr() == 0x00){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2003, 0x01);

    // if(ppu.get_oam_adrr() == 0x01){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2003, 0x80);

    // if(ppu.get_oam_adrr() == 0x80){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2003, 0xFF);

    // if(ppu.get_oam_adrr() == 0xFF){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "PPU OAM DATA TEST" << std::endl;
    // std::cout << "TEST 29" << std::endl;
    
    // bus.write(0x2003, 0x00);
    // bus.write(0x2004, 0xAB);

    // if(ppu.read_oam_ram(0x00) == 0xAB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_oam_adrr() == 0x01){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 30" << std::endl;
    // bus.write(0x2003, 0x80);
    // bus.write(0x2004, 0x42);

    // if(ppu.read_oam_ram(0x80) == 0x42){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_oam_adrr() == 0x81){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 31" << std::endl;
    
    // bus.write(0x2003, 0xFF);
    // bus.write(0x2004, 0x55);

    // if(ppu.read_oam_ram(0xFF) == 0x55){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_oam_adrr() == 0x00){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "PPU OAM DATA" << std::endl;
    // std::cout << "TEST 32" << std::endl;

    // bus.write(0x2003, 0x10);
    // bus.write(0x2004, 0xAB);

    // if(ppu.get_oam_adrr() == 0x11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2003, 0x10);

    // if(bus.read(0x2004) == 0xAB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    

    // std::cout << "TEST 33" << std::endl;
    // bus.write(0x2003, 0x80);
    // bus.write(0x2004, 0x42);

    // if(ppu.get_oam_adrr() == 0x81){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2003, 0x80);
    // if(bus.read(0x2004) == 0x42){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    

    // std::cout << "TEST 34" << std::endl;
    // bus.write(0x2003, 0xFF);
    // bus.write(0x2004, 0x55);
    
    // if(ppu.get_oam_adrr() == 0x00){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2003, 0xFF);

    // if(bus.read(0x2004) == 0x55){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "PPU SCROLL TEST" << std::endl;
    // std::cout << "TEST 35" << std::endl;
    // ppu.clear_write_toggle();
    // ppu.set_t(0);
    // ppu.set_fine_x(0);

    // bus.write(0x2005, 0b10101010);

    // if(ppu.get_t() == 0b000000000010101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_fine_x() == 0b010){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "Test 36" << std::endl;
    // ppu.clear_write_toggle();
    // ppu.set_t(0b010110101010101);
    // ppu.set_fine_x(0);

    // bus.write(0x2005,0b11001101);
    
    // if(ppu.get_t() == 0b010110101011001){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_fine_x() == 0b101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 37" << std::endl;

    // ppu.update_write_toggle();
    // ppu.set_t(0);
    // ppu.set_fine_x(0);

    // bus.write(0x2005,0b10101010);
    
    // if(ppu.get_t() == 0b010001010100000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << ppu.get_t() << std::endl;
    // }

    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // std::cout << "TEST 38" << std::endl;

    // ppu.update_write_toggle();
    // ppu.set_t(0b000000000010101);
    // ppu.set_fine_x(0);

    // bus.write(0x2005,0b10101010);
    
    // if(ppu.get_t() == 0b010001010110101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << ppu.get_t() << std::endl;
    // }

    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 39" << std::endl;

    // ppu.clear_write_toggle();
    // ppu.set_t(0);
    // ppu.set_fine_x(0);

    // bus.write(0x2005,0b10101010);
    // bus.write(0x2005, 0b11001101);
    
    // if(ppu.get_t() == 0b0101001100110101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << ppu.get_t() << std::endl;
    // }

    // if(ppu.get_fine_x() == 0b010){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 40" << std::endl;

    // ppu.clear_write_toggle();
    // ppu.set_t(0);

    // bus.write(0x2005, 0b10101010);

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.read(0x2002);

    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2005, 0b10101010);

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "PPU DATA" << std::endl;
    // std::cout << "TEST 41" << std::endl;
    // ppu.clear_write_toggle();
    // ppu.set_t(0);
    // ppu.set_v(0);

    // bus.write(0x2006, 0x20);
    // bus.write(0x2006, 0x00);

    // bus.write(0x2007,0xAB);

    // bus.write(0x2006, 0x20);
    // bus.write(0x2006, 0x00);
    // bus.read(0x2007);
    // if(bus.read(0x2007) == 0xAB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 42" << std::endl;
    // ppu.clear_write_toggle();
    // ppu.set_ctrl(0);
    // ppu.set_t(0);
    // ppu.set_v(0x2000);

    // bus.write(0x2007, 0xAA);
    // bus.write(0x2007, 0xBB);
    

    // if(ppu.read_vram(0x0000) == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << "VRAM 0x100 = " << ppu.read_vram(0x0000) << std::endl;
    // }
    // if(ppu.read_vram(0x0001) == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << "VRAM 0x2001 MOD 0x0800 = " << static_cast<int>(ppu.read_vram(0x0001)) << std::endl;
    // }

    // std::cout << "TEST 43" << std::endl;
    // ppu.set_ctrl(0);
    // ppu.set_v(0x2000);

    // bus.write(0x2007,0);
    // if(ppu.get_v() == 0x2001){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2007,0);
    // if(ppu.get_v() == 0x2002){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    
    // std::cout << "TEST 44" << std::endl;
    // ppu.set_ctrl(0b100);
    // ppu.set_v(0x2000);

    // bus.write(0x2007,0);
    // if(ppu.get_v() == 0x2020){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // bus.write(0x2007,0);
    // if(ppu.get_v() == 0x2040){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 45" << std::endl;
    // ppu.set_ctrl(0b0);
    // ppu.set_v(0x2000);

    // bus.write(0x2007, 0x11);
    // bus.write(0x2007, 0x22);
    // bus.write(0x2007, 0x33);

    // if(ppu.get_v() == 0x2003){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.read(0x2007);
    // bus.read(0x2007);
    // bus.read(0x2007);

    // if(ppu.get_v() == 0x2006){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 46" << std::endl;
    // ppu.clear_write_toggle();
    
    // bus.write(0x2006, 0x21);

    // if(ppu.get_t() == 0x2100){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006,0x00);

    // if(ppu.get_t() == 0x2100){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2100){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2007, 0x55);

    // if(ppu.read_vram(0x0100) == 0x55){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 47" << std::endl;
    // ppu.clear_write_toggle();
    
    // bus.write(0x2006,0x23);

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006,0x45);
    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2345){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x67);

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2345){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006, 0x89);

    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2789){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << ppu.get_v() << std::endl;
    // }

    // std::cout << "TEST 48" << std::endl;

    // ppu.set_v(0x2000);
    // ppu.set_vram_data(0);

    // bus.write(0x2007, 0xAA);
    // bus.write(0x2007, 0xBB);

    // ppu.set_v(0x2000);

    // if(bus.read(0x2007) == 0){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_vram_data() == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2001){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(bus.read(0x2007) == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_vram_data() == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2002){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(bus.read(0x2007) == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2003){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 49" << std::endl;
    // ppu.set_vram_data(0);
    // ppu.set_ctrl(0b100);
    // ppu.set_v(0x2000);

    // bus.write(0x2007, 0xAA);
    // bus.write(0x2007, 0xBB);

    // ppu.set_v(0x2000);
    // uint16_t result = bus.read(0x2007);
    // if( result == 0){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << result << std::endl;
    // }

    // if(ppu.get_vram_data() == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2020){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(bus.read(0x2007) == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_vram_data() == 0xBB){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x2040){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 50" << std::endl;
    // ppu.set_ctrl(0);
    // ppu.clear_write_toggle();

    // bus.write(0x2006,0x21);

    // if(ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // bus.write(0x2006,0x00);

    // if(!ppu.get_write_toggle()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_v() == 0x2100){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << static_cast<int>(ppu.get_v()) << std::endl;
    // }

    // ppu.set_vram_data(0);

    // bus.write(0x2007,0xAA);

    // bus.write(0x2006,0x21);
    // bus.write(0x2006,0x00);

    // if(bus.read(0x2007) == 0){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_vram_data() == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << static_cast<int>(ppu.get_vram_data()) << std::endl;
    // }

    // if(ppu.get_v() == 0x2101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    //     std::cout << static_cast<int>(ppu.get_v()) << std::endl;

    // }
    // std::cout << "TEST 51" << std::endl;
    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x00);

    // bus.write(0x2007, 0x3F);

    // ppu.set_v(0x3F00);

    // if(bus.read(0x2007) == 0x3F){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x3F01){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // ppu.set_v(0x3F00);

    // if(bus.read(0x2007) == 0x3F){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 52" << std::endl;
    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x00);

    // bus.write(0x2007, 0x3F);

    // ppu.set_v(0x3F00);

    // if(bus.read(0x2007) == 0x3F){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_v(0x3F20);

    // if(bus.read(0x2007) == 0x3F){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    
    // ppu.set_v(0x3F40);

    // if(bus.read(0x2007) == 0x3F){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_v(0x3F60);

    // if(bus.read(0x2007) == 0x3F){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 53" << std::endl;

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x00);

    // bus.write(0x2007, 0x11);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x04);

    // bus.write(0x2007, 0x22);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x08);

    // bus.write(0x2007, 0x33);

    // bus.write(0x2006, 0x3F);
    // bus.write(0x2006, 0x0C);

    // bus.write(0x2007, 0x44);

    // ppu.set_v(0x3F10);

    // if(bus.read(0x2007) == 0x11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_v(0x3F14);

    // if(bus.read(0x2007) == 0x22){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_v(0x3F18);

    // if(bus.read(0x2007) == 0x33){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_v(0x3F1C);

    // if(bus.read(0x2007) == 0x44){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 54" << std::endl;
    // ppu.set_ctrl(0);
    // ppu.set_v(0x3FFF);

    // bus.read(0x2007);
    // if(ppu.get_v() == 0x4000){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 55" << std::endl;

    // bus.write(0x2006,0x20);
    // bus.write(0x2006,0x00);

    // bus.write(0x2007,0xAA);

    // ppu.set_v(0x3000);
    // ppu.set_vram_data(0x00);

    // if(bus.read(0x2007) == 0x00){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_vram_data() == 0xAA){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // if(ppu.get_v() == 0x3001){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "NMI CPU EXECUTION TESTS" << std::endl;
    // std::cout << "TEST 1" << std::endl;
    // bus.set_NOP_sys_ram();
    // nes.set_pc(0x0100);
    // nes.set_stack_ptr(0xFF);
    // ppu.set_nmi_output(false);
    // nes.set_prev_nmi_line(false);
    // ppu.clear_v_blank();
    // rom.set_FA_FB(0x78,0x56);
    // nes.execute();

    // if(nes.get_pc() == 0x0101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(!nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 2" << std::endl;
    // nes.set_pc(0x0100);
    // nes.set_stack_ptr(0xFF);
    // ppu.set_nmi_output(true);
    // nes.set_prev_nmi_line(false);
    // ppu.update_v_blank();
    // rom.set_FA_FB(0x78,0x56);
    // nes.execute();

    // if(nes.get_pc() == 0x5678){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_stack_ptr() == 0xFC){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 3" << std::endl;
    // nes.set_pc(0x0100);
    // nes.set_stack_ptr(0xFF);
    // ppu.set_nmi_output(true);
    // nes.set_prev_nmi_line(true);
    // ppu.update_v_blank();
    // rom.set_FA_FB(0x78,0x56);
    // nes.execute();

    // if(nes.get_pc() == 0x0101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 4" << std::endl;
    // nes.set_pc(0x0100);
    // nes.set_stack_ptr(0xFF);
    // ppu.set_nmi_output(true);
    // nes.set_prev_nmi_line(true);
    // ppu.clear_v_blank();
    // rom.set_FA_FB(0x78,0x56);
    // nes.execute();

    // if(nes.get_pc() == 0x0101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(!nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 5" << std::endl;
    // nes.set_pc(0x0100);
    // nes.set_stack_ptr(0xFF);
    // ppu.set_nmi_output(false);
    // nes.set_prev_nmi_line(false);
    // ppu.update_v_blank();
    // rom.set_FA_FB(0x78,0x56);
    // nes.execute();

    // if(nes.get_pc() == 0x0101){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(!nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 6" << std::endl;
    // nes.set_pc(0x0100);
    // nes.set_stack_ptr(0xFF);
    // ppu.set_nmi_output(false);
    // nes.set_prev_nmi_line(false);
    // ppu.update_v_blank();
    // rom.set_FA_FB(0x78,0x56);

    // bus.write(0x2000, 128);

    // nes.execute();

    // if(nes.get_pc() == 0x5678){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_stack_ptr() == 0xFC){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // std::cout << "TEST 7" << std::endl;
    // nes.set_pc(0x0100);
    // nes.set_stack_ptr(0xFF);
    // ppu.set_nmi_output(true);
    // nes.set_prev_nmi_line(false);
    // ppu.clear_v_blank();
    // ppu.set_scan_ln_count(240);
    // ppu.set_cycle_count(340);
    // rom.set_FA_FB(0x78,0x56);

    // ppu.tick();
    // nes.execute();

    // if(nes.get_pc() == 0x5678){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "PC: " << static_cast<int>(nes.get_pc()) << std::endl;
    //     std::cout << "Expected PC: " << 0x5678 << std::endl;
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Prev NMI Line: " << nes.get_prev_nmi_line() << std::endl;
    //     std::cout << "Expected Prev NMI Line: TRUE" << std::endl;
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_stack_ptr() == 0xFC){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Stack Ptr: " << static_cast<int>(nes.get_stack_ptr()) << std::endl;
    //     std::cout << "Expected Stack Ptr: TRUE" << std::endl;
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_scan_ln_count(260);
    // ppu.set_cycle_count(340);

    // ppu.tick();

    // if(!ppu.get_v_blank()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // nes.execute();

    // if(!nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }

    // ppu.set_scan_ln_count(240);
    // ppu.set_cycle_count(340);

    // ppu.tick();
    // nes.execute();
    
    // if(nes.get_pc() == 0x5678){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "PC: " << static_cast<int>(nes.get_pc()) << std::endl;
    //     std::cout << "Expected PC: " << static_cast<int>(nes.get_pc()) << std::endl;
    //     std::cout << "Fail" << std::endl;
    // }
    // if(nes.get_prev_nmi_line()){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Prev NMI Line: " << nes.get_prev_nmi_line() << std::endl;
    //     std::cout << "Expected Prev NMI Line: TRUE" << std::endl;
    //     std::cout << "Fail" << std::endl;
    // }

    std::cout << "PPU RENDERING TESTS" << std::endl;
    std::cout << "TEST 1" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);
    ppu.set_v(0);
    ppu.set_low_shift(0);
    ppu.set_high_shift(0);
    ppu.set_pal_state(0);
    ppu.set_pattern_high(0);
    ppu.set_pattern_low(0);
    while(ppu.get_dot_count() < 340){
        ppu.tick();
    }

    ppu.tick();
    std::cout << "Dot Count After loop: " << ppu.get_dot_count() << std::endl; 
    if(ppu.get_scan_ln_count() == 1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
        std::cout << "Scan Line Count: " << ppu.get_scan_ln_count() << std::endl; 
    }
    if(ppu.get_dot_count() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
        std::cout << "Dot Count: " << ppu.get_dot_count() << std::endl; 
    }
    std::cout << "TEST 2" << std::endl;
    ppu.set_t(0b101010101010101);
    ppu.set_v(0b010101010101010);
    ppu.set_dot_count(256);
    ppu.set_scan_ln_count(0);
    std::cout << "register V: " << ppu.get_v() << std::endl;
    ppu.tick();

    if(ppu.get_t() == 0b101010101010101){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0b0011101010101011){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
        std::cout << "register V: " << ppu.get_v() << 
        " VS Expected value: " << 0b0011101010101011 << std::endl;

    }

    std::cout << "TEST 3 A" << std::endl;
    ppu.set_v(0b010001010101010);
    std::cout << "register V: " << ppu.get_v() << std::endl;
    ppu.set_dot_count(256);
    ppu.set_scan_ln_count(0);
    ppu.tick();

    if(ppu.get_v() == 0b011001010101011){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "register V: " << ppu.get_v() << std::endl;
        std::cout << "expected V: " << 0b010101010101010 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 3 B" << std::endl;
    ppu.set_v(0b010001111101010);
    ppu.set_dot_count(256);
    ppu.set_scan_ln_count(0);
    ppu.tick();

    if(ppu.get_v() == 0b011001111101011){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "register V: " << ppu.get_v() << std::endl;
        std::cout << "expected V: " << 0b011001111101011 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 3 C" << std::endl;
    ppu.set_v(0b000111111101010);
    ppu.set_dot_count(256);
    ppu.set_scan_ln_count(0);
    ppu.tick();

    if(ppu.get_v() == 0b001111111101011){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    ppu.write_vram(0,0x10);
    ppu.write_vram(1,0x20);
    ppu.write_vram(2,0x30);
    ppu.write_vram(3,0x40);
    ppu.write_vram(4,0x50);
    ppu.write_vram(5,0x60);
    ppu.write_vram(6,0x70);
    ppu.write_vram(7,0x80);
    ppu.set_v(0b001000000000000);
    ppu.set_t(0b001000000000000);
    ppu.clear_tile_buffer();
    ppu.set_dot_count(257);
    ppu.set_scan_ln_count(0);
    for(int i = 257; i <= 320; i++){
        ppu.tick();
    }
    if(ppu.get_tile_buffer()[0] == 0x10){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "tile buffer index 0: " << static_cast<int>(ppu.get_tile_buffer()[0]) << std::endl;
        std::cout << "VRAM at index 0: " << static_cast<int>(ppu.read_vram(0)) << std::endl;
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[1] == 0x20){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "tile buffer index 1: " << static_cast<int>(ppu.get_tile_buffer()[1]) << std::endl;
        std::cout << "VRAM at index 1: " << static_cast<int>(ppu.read_vram(1)) << std::endl;
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[2] == 0x30){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "tile buffer index 2: " << static_cast<int>(ppu.get_tile_buffer()[2]) << std::endl;
        std::cout << "VRAM at index 2: " << static_cast<int>(ppu.read_vram(2)) << std::endl;
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[3] == 0x40){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[4] == 0x50){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[5] == 0x60){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[6] == 0x70){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[7] == 0x80){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    ppu.set_v(0b001000000000000);
    ppu.set_t(0b001000000000000);
    ppu.set_dot_count(257);
    ppu.set_ctrl(0);
    ppu.set_scan_ln_count(0);
    ppu.write_vram(0,0x12);
    ppu.write_vram(0x3C0,0x34);
    rom.set_CHR(0);
    bus.get_rom().get_mapper_info().write_CHR(0x0121,0x56);
    bus.get_rom().get_mapper_info().write_CHR(0x0129,0x78);

    ppu.clear_tile_buffer();
    ppu.clear_attribute_buffer();
    
    ppu.tick(); // 257
    if(ppu.get_dot_count() == 258){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[0] == 0x12){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Tile Buffer at index 0: " << static_cast<int>(ppu.get_tile_buffer()[0]) << std::endl;
        std::cout << "Expected Value: " << 0x12 << std::endl;
        std::cout << "Fail" << std::endl;
    }
    ppu.tick(); // 259
    ppu.tick(); // 260
    
    if(ppu.get_dot_count() == 260){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_attribute_buffer()[0] == 0x34){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    ppu.tick(); // 261
    ppu.tick(); // 262
    
    if(ppu.get_dot_count() == 262){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_pattern_low() == 0x56){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Pattern Low: " << static_cast<int>(ppu.get_pattern_low()) << std::endl;
        std::cout << "Expected: " << 0x56 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    ppu.tick(); // 263
    ppu.tick(); // 264

    if(ppu.get_dot_count() == 264){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_pattern_low() == 0x56){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_pattern_high() == 0x78){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.tick(); // 265

    if(ppu.get_dot_count() == 265){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_low_shift() == 0x56){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0x78){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.tick(); // 266

    if(ppu.get_dot_count() == 266){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 6" << std::endl;
    ppu.set_v(0b001000000000000);
    ppu.set_dot_count(257);
    ppu.set_scan_ln_count(0);
    ppu.write_vram(0x0,0x10);
    ppu.write_vram(0x1,0x20);
    ppu.write_vram(0x2,0x30);
    ppu.write_vram(0x3,0x40);
    ppu.write_vram(0x4,0x50);
    ppu.write_vram(0x5,0x60);
    ppu.write_vram(0x6,0x70);
    ppu.write_vram(0x7,0x80);
    ppu.clear_tile_buffer();
    for (int i = 257; i <= 320; i++){
        ppu.tick();
    }
    if(ppu.get_tile_buffer()[0] == 0x10){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[1] == 0x20){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[2] == 0x30){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[3] == 0x40){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[4] == 0x50){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[5] == 0x60){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[6] == 0x70){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_tile_buffer()[7] == 0x80){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v() == 0b001000000001000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Register V: " << ppu.get_v() << std::endl;
        std::cout << "Expected V: " << 0b001000000001000 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 7" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);
    ppu.set_v(0x2000);
    ppu.write_vram(0x0,0x42);

    ppu.tick();

    if(ppu.get_tile_buffer()[0] == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 8" << std::endl;
    ppu.set_v(0x2001);
    ppu.write_vram(0x1,0x37);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(9);

    ppu.tick();

    if(ppu.get_tile_buffer()[1] == 0x37){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 9" << std::endl;
    ppu.set_v(0x2000);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(3);
    ppu.write_vram(0x3C0,0xA5);
    ppu.clear_attribute_buffer();
    ppu.tick();
    if(ppu.get_attribute_buffer()[0] == 0xA5){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 10" << std::endl;
    ppu.set_v(0x2141);
    ppu.write_vram(0x3D0,0x96);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(3);

    ppu.tick();
    if(ppu.get_attribute_buffer()[0] == 0x96){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 11 A" << std::endl;
    ppu.set_ctrl(0);
    ppu.clear_tile_buffer();
    ppu.write_tile_buffer(0,0x02);
    ppu.set_v(0);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(5);
    bus.get_rom().get_mapper_info().write_CHR(0x0020,0xAB);
    bus.get_rom().get_mapper_info().write_CHR(0x1020,0xCD);
    ppu.tick();
    if(ppu.get_pattern_low() == 0xAB){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 11 B" << std::endl;
    ppu.set_ctrl(0x10);
    ppu.clear_tile_buffer();
    ppu.write_tile_buffer(0,0x02);
    ppu.set_v(0);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(5);
    bus.get_rom().get_mapper_info().write_CHR(0x0020,0xAB);
    bus.get_rom().get_mapper_info().write_CHR(0x1020,0xCD);
    ppu.tick();
    if(ppu.get_pattern_low() == 0xCD){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    
    std::cout << "TEST 12" << std::endl;
    ppu.set_ctrl(0);
    ppu.clear_tile_buffer();
    ppu.write_tile_buffer(0,0x03);
    ppu.set_v(0x3000);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(5);
    bus.get_rom().get_mapper_info().write_CHR(0x0033, 0x71);

    ppu.tick();

    if(ppu.get_pattern_low() == 0x71){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 13" << std::endl;
    ppu.set_ctrl(0);
    ppu.clear_tile_buffer();
    ppu.write_tile_buffer(0,0x03);
    ppu.set_v(0x3000);
    ppu.set_dot_count(7);
    ppu.set_scan_ln_count(0);
    bus.get_rom().get_mapper_info().write_CHR(0x0033, 0x71);
    bus.get_rom().get_mapper_info().write_CHR(0x003B, 0xD2);

    ppu.tick();

    if(ppu.get_pattern_high() == 0xD2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 14" << std::endl;
    ppu.set_v(0);
    ppu.set_ctrl(0);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.write_vram(0x0,0x04);
    ppu.write_vram(0x3C0,0x55);

    bus.get_rom().get_mapper_info().write_CHR(0x0040,0x12);
    bus.get_rom().get_mapper_info().write_CHR(0x0048,0x34);

    ppu.tick();

    if(ppu.get_tile_buffer()[0] == 0x04){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.tick();
    ppu.tick();

    if(ppu.get_attribute_buffer()[0] == 0x55){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.tick();
    ppu.tick();

    if(ppu.get_pattern_low() == 0x12){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.tick();
    ppu.tick();

    if(ppu.get_pattern_high() == 0x34){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 15 A" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);
    ppu.set_pal_state(0);

    ppu.write_pal_ram(0x00,0x10);
    ppu.write_pal_ram(0x01,0x20);
    ppu.write_pal_ram(0x02,0x30);
    ppu.write_pal_ram(0x03,0x40);

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
    if(ppu.get_frame_buffer()[0][0] == 0x20){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 15B" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0);
    ppu.set_high_shift(0x4000);
    ppu.set_pal_state(0);

    ppu.write_pal_ram(0x00,0x10);
    ppu.write_pal_ram(0x01,0x20);
    ppu.write_pal_ram(0x02,0x30);
    ppu.write_pal_ram(0x03,0x40);

    ppu.tick();

    if(ppu.get_low_shift() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0x8000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_frame_buffer()[0][0] == 0x30){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 15C" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x4000);
    ppu.set_pal_state(0);

    ppu.write_pal_ram(0x00,0x10);
    ppu.write_pal_ram(0x01,0x20);
    ppu.write_pal_ram(0x02,0x30);
    ppu.write_pal_ram(0x03,0x40);

    ppu.tick();

    if(ppu.get_low_shift() == 0x8000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0x8000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_frame_buffer()[0][0] == 0x40){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 15D" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0);
    ppu.set_high_shift(0);
    ppu.set_pal_state(0);

    ppu.write_pal_ram(0x00,0x10);
    ppu.write_pal_ram(0x01,0x20);
    ppu.write_pal_ram(0x02,0x30);
    ppu.write_pal_ram(0x03,0x40);

    ppu.tick();

    if(ppu.get_low_shift() == 0){
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
    if(ppu.get_frame_buffer()[0][0] == 0x10){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 16" << std::endl;
    std::cout << "ALL PASSED IN ISOLATION WITH OTHER TESTS" << std::endl;
    // std::cout << "1" << std::endl;
    // ppu.set_scan_ln_count(0);
    // std::cout << "2" << std::endl;
    // ppu.set_dot_count(1);
    // std::cout << "3" << std::endl;
    // ppu.set_pal_state(0);

    // std::cout << "4" << std::endl;
    // ppu.set_low_shift(0x5500);
    // std::cout << "5" << std::endl;
    // ppu.set_high_shift(0xAA00);

    // std::cout << "6" << std::endl;
    // ppu.write_pal_ram(0x00,0x10);
    // std::cout << "7" << std::endl;
    // ppu.write_pal_ram(0x01,0x11);
    // std::cout << "8" << std::endl;
    // ppu.write_pal_ram(0x02,0x12);
    // std::cout << "9" << std::endl;
    // ppu.write_pal_ram(0x03,0x13);

    // std::cout << "10" << std::endl;
    // std::cout << "BEFORE FB CHECK" << std::endl;

    // std::cout << "11" << std::endl;
    // uint8_t value = ppu.get_frame_buffer()[0][0];

    // std::cout << "12" << std::endl;
    // std::cout << "VALUE: " << static_cast<int>(value) << std::endl;

    // std::cout << "BEFORE VALUE 7" << std::endl;

    // uint8_t value4 = ppu.get_frame_buffer()[0][6];

    // std::cout << "13" << std::endl;
    // for (int i = 0; i < 8; i++){
    //     ppu.tick();
    // }

    // std::cout << "14" << std::endl;
    // value = ppu.get_frame_buffer()[0][0];

    // std::cout << "15" << std::endl;
    // std::cout << "AFTER FB CHECK" << std::endl;
    // std::cout << "16" << std::endl;
    // std::cout << "VALUE 0: " << static_cast<int>(value) << std::endl;
    // std::cout << "17" << std::endl;
    // value = ppu.get_frame_buffer()[0][1];
    // std::cout << "18" << std::endl;
    // std::cout << "VALUE 1: " << static_cast<int>(value) << std::endl;
    // std::cout << "19" << std::endl;
    // value = ppu.get_frame_buffer()[0][2];
    // std::cout << "20" << std::endl;
    // std::cout << "VALUE 2: " << static_cast<int>(value) << std::endl;
    // std::cout << "21" << std::endl;
    // value = ppu.get_frame_buffer()[0][3];
    // std::cout << "22" << std::endl;
    // std::cout << "VALUE 3: " << static_cast<int>(value) << std::endl;
    // std::cout << "23" << std::endl;
    // value = ppu.get_frame_buffer()[0][4];
    // std::cout << "24" << std::endl;
    // std::cout << "VALUE 4: "<< static_cast<int>(value) << std::endl;
    // value = ppu.get_frame_buffer()[0][5];
    // std::cout << "25" << std::endl;
    // std::cout << "VALUE 5: " << static_cast<int>(value) << std::endl;
    // value = ppu.get_frame_buffer()[0][6];
    // std::cout << "26" << std::endl;
    // std::cout << "VALUE 6: " << static_cast<int>(value) << std::endl;
    // value = ppu.get_frame_buffer()[0][7];
    // std::cout << "26" << std::endl;
    // std::cout << "VALUE 7: " << static_cast<int>(value) << std::endl;


    
    // if(ppu.get_frame_buffer()[0][0] == 0x11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_frame_buffer()[0][1] == 0x12){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_frame_buffer()[0][2] == 0x11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_frame_buffer()[0][3] == 0x12){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_frame_buffer()[0][4] == 0x11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_frame_buffer()[0][5] == 0x12){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    // if(ppu.get_frame_buffer()[0][6] == 0x11){
    //     std::cout << "Pass" << std::endl;
    // }
    // else{
    //     std::cout << "Fail" << std::endl;
    // }
    uint8_t value;
    std::cout << "TEST 17 A" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);
    
    ppu.set_pal_state(0);

    ppu.write_pal_ram(0x01,0x11);

    ppu.tick();
    value = ppu.get_frame_buffer()[0][0];

    if(value == 0x11){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 17 B" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);
    
    ppu.set_pal_state(1);

    ppu.write_pal_ram(0x05,0x21);

    ppu.tick();

    value = ppu.get_frame_buffer()[0][0];

    if(value == 0x21){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 17 C" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);
    
    ppu.set_pal_state(2);

    ppu.write_pal_ram(0x09,0x31);

    ppu.tick();

    value = ppu.get_frame_buffer()[0][0];

    if(value == 0x31){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 17 D" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);
    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);
    
    ppu.set_pal_state(3);

    ppu.write_pal_ram(0x0D,0x41);

    ppu.tick();

    value = ppu.get_frame_buffer()[0][0];

    if(value == 0x41){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_frame_buffer()[0][0] == 0x41){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 18" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0);
    ppu.set_high_shift(0);

    ppu.set_pal_state(3);

    ppu.write_pal_ram(0x00,0x55);
    ppu.write_pal_ram(0x0D,0x66);

    ppu.tick();

    if(ppu.get_frame_buffer()[0][0] == 0x55){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 19A" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(8);

    ppu.set_v(0x0005);

    ppu.tick();

    if(ppu.get_v() == 0x0006){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 19B" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(8);

    ppu.set_v(0x001F);

    ppu.tick();

    if(ppu.get_v() == 0x0400){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 19C" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(8);

    ppu.set_v(0x041F);

    ppu.tick();

    if(ppu.get_v() == 0x0000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 20 A" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.set_v(0);

    ppu.tick();

    if(ppu.get_v() == 0x1001){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Register V: " << ppu.get_v() << std::endl;
        std::cout << "Expected V: " << 0x1000 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 20 B" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.set_v(0x6000);

    ppu.tick();

    if(ppu.get_v() == 0x7001){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 20 C" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.set_v(0x70A0);

    ppu.tick();

    if(ppu.get_v() == 0x00C1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 21 A" << std::endl;
    ppu.set_v(0x71A0);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.tick();

    if(ppu.get_v() == 0x01C1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Register V: " << ppu.get_v() << std::endl;
        std::cout << "Expected V: " << 0x0800 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 21 B" << std::endl;
    ppu.set_v(0x79A0);
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);
    
    ppu.tick();
    
    if(ppu.get_v() == 0x09C1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 22 A" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.set_v(0x73C0);
    ppu.tick();

    if(ppu.get_v() == 1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Register V: " << ppu.get_v() << std::endl;
        std::cout << "Expected V: " << 0 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 22 B" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.set_v(0x7BC0);
    ppu.tick();

    if(ppu.get_v() == 0x0801){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Register V: " << ppu.get_v() << std::endl;
        std::cout << "Expected V: " << 0x800 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 22 C" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(256);

    ppu.set_v(0x7BE0);
    ppu.tick();

    if(ppu.get_v() == 0x0801){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Register V: " << ppu.get_v() << std::endl;
        std::cout << "Expected V: " << 0x800 << std::endl;
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 23" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x1234);
    ppu.set_high_shift(0);

    ppu.tick();

    if(ppu.get_low_shift() == 0x2468){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_scan_ln_count() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_dot_count() == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 24" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0);
    ppu.set_high_shift(0x1234);

    ppu.tick();

    if(ppu.get_high_shift() == 0x2468){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_scan_ln_count() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_dot_count() == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 25" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x1234);
    ppu.set_high_shift(0xABCD);

    ppu.tick();

    if(ppu.get_low_shift() == 0x2468){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_high_shift() == 0x579A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_scan_ln_count() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_dot_count() == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 26" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);

    ppu.set_low_shift(0x8001);
    ppu.set_high_shift(0x8000);

    ppu.tick();

    if(ppu.get_low_shift() == 2){
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
    if(ppu.get_scan_ln_count() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_dot_count() == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 27" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(1);
    
    ppu.set_low_shift(0x1234);
    ppu.set_high_shift(0x5678);

    ppu.tick();

    if(ppu.get_low_shift() == 0x2468){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0xACF0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_dot_count() == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.tick();

    if(ppu.get_low_shift() == 0x48D0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0x59E0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_dot_count() == 3){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.tick();

    if(ppu.get_low_shift() == 0x91A0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0xB3C0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_dot_count() == 4){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 28" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x1234);
    std::cout << "low shift: " << static_cast<int>(ppu.get_low_shift()) << std::endl;
    ppu.set_high_shift(0xABCD);
    std::cout << "high shift: " << static_cast<int>(ppu.get_high_shift()) << std::endl;
    
    ppu.tick();

    if(ppu.get_low_shift() == 0x2468){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "low shift: " << static_cast<int>(ppu.get_low_shift()) << std::endl;
        std::cout << "expected: " << 0x2468 << std::endl;
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_high_shift() == 0x579A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "high shift: " << static_cast<int>(ppu.get_high_shift()) << std::endl;
        std::cout << "expected: " << 0x579A << std::endl;
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_scan_ln_count() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_dot_count() == 0x12){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 29 A" << std::endl;
    uint8_t low_bit, high_bit, pattern_val;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 0x01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 29 B" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0);
    ppu.set_high_shift(0x4000);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 0x02){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 29 C" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x4000);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 0x03){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 29 D" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0);
    ppu.set_high_shift(0);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 0x00){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 30 A" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);

    ppu.set_pal_state(0);

    ppu.write_pal_ram(0x01,0x2A);

    ppu.tick();

    if(ppu.get_frame_buffer()[0][0x10] == 0x2A){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 30 B" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0);
    ppu.set_high_shift(0);

    ppu.set_pal_state(0x03);

    ppu.write_pal_ram(0x00,0x15);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 0x00){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_frame_buffer()[0][0x10] == 0x15){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 30 C" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0x4000);

    ppu.set_pal_state(0x02);

    ppu.write_pal_ram(0x0B,0x37);

    ppu.tick();

    if(ppu.get_frame_buffer()[0][0x10] == 0x37){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 31" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x11);

    ppu.set_low_shift(0b0101101001011010);
    ppu.set_high_shift(0b1010010110100101);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    // ppu.set_dot_count(0x11);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    // ppu.set_dot_count(0x11);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    // ppu.set_dot_count(0x11);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    // ppu.set_dot_count(0x11);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    // ppu.set_dot_count(0x11);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 1){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.set_dot_count(0x11);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.set_dot_count(0x11);

    ppu.tick();

    low_bit = ppu.get_low_shift() >> 15;
    high_bit = ppu.get_high_shift() >> 15;

    pattern_val = low_bit + (2 * high_bit);

    if(pattern_val == 2){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 32 A" << std::endl;
    ppu.set_scan_ln_count(0);
    ppu.set_dot_count(0x01);
    ppu.set_mask(0);

    ppu.set_low_shift(0x4000);
    ppu.set_high_shift(0);
    ppu.set_pal_state(0);
    return 0;
}