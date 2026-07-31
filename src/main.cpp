#include "NES.cpp"
#include <iomanip>

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
    NES nes = NES();
    // check if the user has send the ROM as an arg
    // if(argc >= 2){
    //     if(!nes.load_ROM(argv[1])){
    //         std::cerr << "ERROR:\t Failed to load ROM" << std::endl;
    //         // return 1;
    //     }
    //     std::cout << std::hex << std::setfill('0');
    //     std::cout << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl;
    // }
    // else if(nes.load_ROM("TEST\\nestest.nes")){
    //     std::cout << "loading Test ROM" << std::endl;
    //     std::cout << std::hex << std::setfill('0');
    //     std::cout << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl;
    // }
    if(!nes.load_ROM("C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\TEST\\nestest.nes")){
        std::cerr << "ERROR:\t Failed to load test ROM" << std::endl;
            return 1;
    }
    // else{
    //     std::cerr << "ERROR:\t No ROM detected" << std::endl;
    //         return 1;
    // }

    // std::cout << std::hex << std::setfill('0');
    // std::cout << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl;

    // std::cout << "instructions" << std::endl;
    // for (int i = 16384; i < 16394; i++){
    //     std::cout << std::hex << std::setfill('0');
    //     std::cout << "Byte:" << std::setw(2) << static_cast<int>(nes.get_prg_rom()[i]) << std::endl;
    // }


    for (int i = 0; i < 8991; i++){
        // uint8_t current_opcode = nes.get_prg_rom()[nes.get_pc() - 0x8000]; // or a peek() method if you add one
        uint8_t current_opcode = nes.read(nes.get_pc());
        std::cout << opcode_names[current_opcode] << " ";
        std::cout << std::hex << std::setfill('0');

        // std::cout << "Before execution" << std::endl;
        std::cout << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << " "
        << "acc:" << std::setw(2) << static_cast<int>(nes.get_acc()) << " "
        << "x:" << std::setw(2) << static_cast<int>(nes.get_x()) << " "
        << "y:" << std::setw(2) << static_cast<int>(nes.get_y()) << " "
        << "status_flag:" << std::setw(2) << static_cast<int>(nes.get_status_flag().to_ulong()) << " "
        << "stack_ptr:" << std::setw(2) << static_cast<int>(nes.get_stack_ptr()) << std::endl;

        nes.execute();

        // std::cout << "After execution" << std::endl;
        // std::cout << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << " "
        // << "acc:" << std::setw(2) << static_cast<int>(nes.get_acc()) << " "
        // << "x:" << std::setw(2) << static_cast<int>(nes.get_x()) << " "
        // << "y:" << std::setw(2) << static_cast<int>(nes.get_y()) << " "
        // << "status_flag:" << std::setw(2) << static_cast<int>(nes.get_status_flag().to_ulong()) << " "
        // << "stack_ptr:" << std::setw(2) << static_cast<int>(nes.get_stack_ptr()) << " ";
    }

    return 0;
}