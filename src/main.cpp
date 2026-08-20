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
    return 0;
}