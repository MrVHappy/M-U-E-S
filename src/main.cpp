#include "NES.hpp"
#include "Mapper.hpp"
#include "BUS.hpp"
#include "Cartridge.hpp"
#include "PPU.hpp"
#include <iomanip>
#define TEST_ROM = "C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\TEST\\nestest.nes"
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
    std::cout << std::hex << std::setfill('0');
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

    
    // reset t, v, and clear write toggle
    std::cout << "TEST 1 A" << std::endl;
    ppu.clear_write_toggle();
    ppu.set_t(0);
    ppu.set_v(0);
    // test 0x2006 write 0x20
    bus.write(0x2006,0x20);
    // validate t and v and write toggle
    if((ppu.get_t() == 0x2000) && (ppu.get_v() == 0) && (ppu.get_write_toggle() == true)){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 1 B" << std::endl;
    // test 0x2006 write 0x0
    bus.write(0x2006,0x0);
    // validate t and v and write toggle
    if((ppu.get_t() == 0x2000) && (ppu.get_v() == 0x2000) && (ppu.get_write_toggle() == false)){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }


    std::cout << "TEST 2" << std::endl;
    // reset t, v, and clear write toggle
    ppu.clear_write_toggle();
    ppu.set_t(0);
    ppu.set_v(0);
    // test 0x2006 write 0x3F
    bus.write(0x2006,0x3F);
    // test 0x2006 write 0x0
    bus.write(0x2006,0x0);
    // validate t and v and write toggle
    if((ppu.get_t() == 0x3F00) && (ppu.get_v() == 0x3F00) && (ppu.get_write_toggle() == false)){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 3" << std::endl;
    // test 0x2006 write 0x21
    bus.write(0x2006,0x21);
    // test 0x2006 write 0x0
    bus.write(0x2006,0x65);
    // validate t and v and write toggle
    if((ppu.get_t() == 0x2165) && (ppu.get_v() == 0x2165) && (ppu.get_write_toggle() == false)){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 4" << std::endl;
    
    // reset t, v, and clear write toggle
    ppu.clear_write_toggle();
    ppu.set_t(0x0000);
    ppu.set_v(0x1234);
    // test 0x2006 write 0x25
    bus.write(0x2006,0x25);
    // validate t and v and write toggle
    if((ppu.get_t() == 0x2500) && (ppu.get_v() == 0x1234) && (ppu.get_write_toggle() == true)){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 5" << std::endl;
    ppu.clear_write_toggle();
    bus.write(0x2006,0x20);
    bus.read(0x2002);
    if(ppu.get_write_toggle() == false){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.write(0x2006,0x3F);
    bus.write(0x2006,0x00);
    if(ppu.get_v() == 0x3F00){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    std::cout << "TEST 6" << std::endl;
    ppu.clear_write_toggle();
    bus.write(0x2005,0x12);
    if(ppu.get_write_toggle() == true){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.write(0x2006,0x34);
    if(ppu.get_write_toggle() == false){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    std::cout << "TEST 7" << std::endl;
    ppu.clear_write_toggle();
    bus.write(0x2006,0x20);
    if(ppu.get_write_toggle() == true){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.write(0x2005,0x40);
    if(ppu.get_write_toggle() == false){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    std::cout << "TEST 8" << std::endl;
    ppu.clear_write_toggle();
    ppu.set_t(0);
    ppu.set_v(0);
    bus.write(0x2006,0x20);
    
    bus.write(0x2006,0x00);
    if(ppu.get_v() == 0x2000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.read(0x2007);
    if(ppu.get_v() == 0x2001){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.read(0x2007);
    if(ppu.get_v() == 0x2002){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    std::cout << "TEST 9" << std::endl;
    ppu.set_v(0x2000);
    ppu.set_ctrl(0b100);
    bus.read(0x2007);
    if(ppu.get_v() == 0x2020){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.read(0x2007);
    if(ppu.get_v() == 0x2040){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    std::cout << "TEST 10" << std::endl;
    rom.set_header(0);
    ppu.clear_write_toggle();
    ppu.set_v(0);
    ppu.set_t(0);
    
    bus.write(0x2006, 0x20);
    bus.write(0x2006, 0x00);
    
    bus.write(0x2007, 0xBB);

    bus.write(0x2006, 0x20);
    bus.write(0x2006, 0x00);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xBB){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x24);
    bus.write(0x2006, 0x00);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xBB){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x28);
    bus.write(0x2006, 0x00);

    bus.write(0x2007, 0xCC);

    bus.write(0x2006, 0x28);
    bus.write(0x2006, 0x00);
    
    bus.read(0x2007);

    if(bus.read(0x2007) == 0xCC){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x2C);
    bus.write(0x2006, 0x00);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xCC){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x20);
    bus.write(0x2006, 0x00);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xBB){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 11" << std::endl;
    
    ppu.clear_write_toggle();
    ppu.set_v(0);
    ppu.set_t(0);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x00);

    bus.write(0x2007, 0x11);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x00);

    if(bus.read(0x2007) == 0x11){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x04);

    bus.write(0x2007, 0x22);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x04);

    if(bus.read(0x2007) == 0x22){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x14);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x14);

    if(bus.read(0x2007) == 0x22){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x08);

    bus.write(0x2007, 0x33);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x08);

    if(bus.read(0x2007) == 0x33){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x18);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x18);

    if(bus.read(0x2007) == 0x33){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x0C);

    bus.write(0x2007, 0x44);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x0C);

    if(bus.read(0x2007) == 0x44){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x1C);

    bus.write(0x2006, 0x3F);
    bus.write(0x2006, 0x1C);

    if(bus.read(0x2007) == 0x44){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 12" << std::endl;
    rom.set_CHR(0);
    ppu.clear_write_toggle();
    ppu.set_t(0);
    ppu.set_v(0);

    bus.write(0x2006,0x00);
    bus.write(0x2006,0x00);

    bus.write(0x2007, 0xAA);

    bus.write(0x2006,0x00);
    bus.write(0x2006,0x00);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xAA){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006,0x00);
    bus.write(0x2006,0x01);

    bus.write(0x2007, 0xBB);

    bus.write(0x2006,0x00);
    bus.write(0x2006,0x01);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xBB){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006,0x00);
    bus.write(0x2006,0x00);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xAA){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006,0x1F);
    bus.write(0x2006,0xFF);

    bus.write(0x2007, 0xCC);

    bus.write(0x2006,0x1F);
    bus.write(0x2006,0xFF);

    bus.read(0x2007);

    if(bus.read(0x2007) == 0xCC){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 13" << std::endl;
    ppu.set_t(0);
    ppu.set_v(0);
    ppu.clear_write_toggle();

    bus.write(0x2006,0x00);
    bus.write(0x2006,0x00);

    ppu.set_ctrl(0);

    bus.write(0x2006,0x2F);
    bus.write(0x2006,0xFE);

    bus.read(0x2007);

    if(ppu.get_v() == 0x2FFF){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.read(0x2007);

    if(ppu.get_v() == 0x3000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2006,0x00);
    bus.write(0x2006,0x00);

    ppu.set_ctrl(0b00000100);

    bus.write(0x2006,0x20);
    bus.write(0x2006,0x00);

    bus.read(0x2007);

    if(ppu.get_v() == 0x2020){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.read(0x2007);

    if(ppu.get_v() == 0x2040){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.set_ctrl(0);
    bus.write(0x2006,0x3F);
    bus.write(0x2006,0xFF);

    bus.read(0x2007);

    if(ppu.get_v() == 0x0000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
        std::cout << ppu.get_v() << std::endl;
    }

    std::cout << "TEST 14" << std::endl;
    ppu.set_t(0);
    ppu.set_v(0);
    ppu.clear_write_toggle();
    ppu.set_ctrl(0);

    bus.write(0x2000,0x80);

    if(ppu.get_ctrl() == 0x80){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "PPU CTRL TESTS" << std::endl;
    std::cout << "TEST 15" << std::endl;
    ppu.set_t(0);
    ppu.set_v(0);
    ppu.clear_write_toggle();
    ppu.set_ctrl(0);

    bus.write(0x2000,0x01);

    if(ppu.get_ctrl() == 0x01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 16" << std::endl;
    ppu.set_t(0);
    ppu.set_v(0);
    ppu.clear_write_toggle();
    ppu.set_ctrl(0);

    bus.write(0x2000,0x00);

    uint8_t test_bits = ppu.get_t() >> 10;
    if(test_bits == 0b00){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2000,0x01);

    test_bits = ppu.get_t() >> 10;
    if(test_bits == 0b01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2000,0x02);

    test_bits = ppu.get_t() >> 10;
    if(test_bits == 0b10){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    bus.write(0x2000,0x03);

    test_bits = ppu.get_t() >> 10;
    if(test_bits == 0b11){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    
    std::cout << "TEST 17" << std::endl;
    ppu.set_t(0b00000110000101);
    ppu.set_v(0);
    ppu.clear_write_toggle();
    ppu.set_ctrl(0);

    bus.write(0x2000,0x03);
    if(ppu.get_t() == 0b00110110000101){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 18 A" << std::endl;
    ppu.set_t(0);
    ppu.set_v(0);
    ppu.clear_write_toggle();
    ppu.set_ctrl(0);

    bus.write(0x2000,0x00);
    ppu.set_v(0x2000);

    bus.write(0x2007, 0x10);

    if(ppu.get_v() == 0x2001){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 18 B" << std::endl;

    bus.write(0x2000,0x04);
    ppu.set_v(0x2000);

    bus.write(0x2007, 0x10);

    if(ppu.get_v() == 0x2020){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    
    std::cout << "TEST 19 A" << std::endl;

    bus.write(0x2000,0x00);

    if(ppu.get_nmi() == false){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 19 B" << std::endl;

    bus.write(0x2000,0x80);

    if(ppu.get_nmi() == true){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 20" << std::endl;
    ppu.set_t(0);
    ppu.set_v(0);
    ppu.clear_write_toggle();
    ppu.set_ctrl(0);
    ppu.clear_nmi();

    ppu.set_v(0x2000);

    bus.write(0x2000,0x00);
    bus.write(0x2007,0x00);

    if(ppu.get_v() == 0x2001){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.set_v(0x2000);

    bus.write(0x2000,0x04);
    bus.write(0x2007,0x00);

    if(ppu.get_v() == 0x2020){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "PPU STATUS TESTS" << std::endl;
    std::cout << "TEST 21" << std::endl;
    ppu.clear_sprite_0_hit();
    ppu.clear_sprite_overflow();
    ppu.clear_v_blank();
    ppu.clear_write_toggle();

    bus.read(0x2002);

    if(ppu.get_status() == 0x0000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 22" << std::endl;
    ppu.clear_sprite_0_hit();
    ppu.clear_sprite_overflow();
    ppu.update_v_blank();
    ppu.clear_write_toggle();

    bus.read(0x2002);

    if(ppu.get_status() == 0b10000000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_v_blank() == false){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 23" << std::endl;
    ppu.update_sprite_0_hit();
    ppu.clear_sprite_overflow();
    ppu.clear_v_blank();
    ppu.clear_write_toggle();

    bus.read(0x2002);
    if(ppu.get_status() == 0b01000000){
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

    std::cout << "TEST 24" << std::endl;
    ppu.clear_sprite_0_hit();
    ppu.update_sprite_overflow();
    ppu.clear_v_blank();
    ppu.clear_write_toggle();

    bus.read(0x2002);

    if(ppu.get_status() == 0b00100000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_sprite_overflow()){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 25" << std::endl;
    ppu.update_sprite_0_hit();
    ppu.update_v_blank();
    ppu.update_sprite_overflow();

    bus.read(0x2002);
    if(ppu.get_status() == 0b11100000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    if(ppu.get_sprite_overflow()){
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
    if(ppu.get_v_blank() == false){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 26" << std::endl;
    ppu.clear_sprite_0_hit();
    ppu.clear_sprite_overflow();
    ppu.clear_v_blank();
    ppu.update_write_toggle();

    bus.read(0x2002);
    if(ppu.get_write_toggle() == false){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 27" << std::endl;
    ppu.update_sprite_0_hit();
    ppu.update_v_blank();
    ppu.update_sprite_overflow();

    bus.read(0x2002);
    if(ppu.get_status() == 0b11100000){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    ppu.clear_sprite_0_hit();
    ppu.clear_v_blank();
    ppu.clear_sprite_overflow();

    bus.read(0x2002);
    if(ppu.get_status() == 0){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "PPU OAM ADDR TEST" << std::endl;
    std::cout << "TEST 28" << std::endl;

    bus.write(0x2003, 0x00);

    if(ppu.get_oam_adrr() == 0x00){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.write(0x2003, 0x01);

    if(ppu.get_oam_adrr() == 0x01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.write(0x2003, 0x80);

    if(ppu.get_oam_adrr() == 0x80){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    bus.write(0x2003, 0xFF);

    if(ppu.get_oam_adrr() == 0xFF){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "PPU OAM DATA TEST" << std::endl;
    std::cout << "TEST 29" << std::endl;
    
    bus.write(0x2003, 0x00);
    bus.write(0x2004, 0xAB);

    if(ppu.read_oam_ram(0x00) == 0xAB){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_oam_adrr() == 0x01){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 30" << std::endl;
    bus.write(0x2003, 0x80);
    bus.write(0x2004, 0x42);

    if(ppu.read_oam_ram(0x80) == 0x42){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_oam_adrr() == 0x81){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    std::cout << "TEST 31" << std::endl;
    
    bus.write(0x2003, 0xFF);
    bus.write(0x2004, 0x55);

    if(ppu.read_oam_ram(0xFF) == 0x55){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }

    if(ppu.get_oam_adrr() == 0x00){
        std::cout << "Pass" << std::endl;
    }
    else{
        std::cout << "Fail" << std::endl;
    }
    return 0;
}