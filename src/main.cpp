#include "NES.cpp"
#include <iomanip>
int main(int argc, char*argv[]){
    NES nes = NES();
    // check if the user has send the ROM as an arg
    // if(argc >= 2){
    //     if(!nes.load_ROM(argv[1])){
    //         std::cerr << "ERROR:\t Failed to load ROM" << std::endl;
    //         // return 1;
    //     }
    //     std::clog << std::hex << std::setfill('0');
    //     std::clog << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl;
    // }
    // else if(nes.load_ROM("TEST\\nestest.nes")){
    //     std::cout << "loading Test ROM" << std::endl;
    //     std::clog << std::hex << std::setfill('0');
    //     std::clog << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl;
    // }
    if(!nes.load_ROM("C:\\Users\\Sebastian\\OneDrive\\Documents\\GitHub\\M-U-E-S\\TEST\\nestest.nes")){
        std::cerr << "ERROR:\t Failed to load test ROM" << std::endl;
            return 1;
    }
    // else{
    //     std::cerr << "ERROR:\t No ROM detected" << std::endl;
    //         return 1;
    // }

    std::clog << std::hex << std::setfill('0');
    std::clog << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl;

    std::cout << "instructions" << std::endl;
    for (int i = 16384; i < 16394; i++){
        std::cout << std::hex << std::setfill('0');
        std::cout << "Byte:" << std::setw(2) << static_cast<int>(nes.get_prg_rom()[i]) << std::endl;
    }


    for (int i = 0; i < 152; i++){
        std::clog << "instruction " << i << ":" << std::endl;
        std::clog << std::hex << std::setfill('0');

        std::clog << "Before execution" << std::endl;
        std::clog << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl
        << "acc:" << std::setw(2) << static_cast<int>(nes.get_acc()) << std::endl
        << "x:" << std::setw(2) << static_cast<int>(nes.get_x()) << std::endl
        << "y:" << std::setw(2) << static_cast<int>(nes.get_y()) << std::endl
        << "status_flag:" << std::setw(2) << static_cast<int>(nes.get_status_flag().to_ulong()) << std::endl
        << "stack_ptr:" << std::setw(2) << static_cast<int>(nes.get_stack_ptr()) << std::endl;

        nes.execute();

        std::clog << "After execution" << std::endl;
        std::clog << "PC:" << std::setw(4) << static_cast<int>(nes.get_pc()) << std::endl
        << "acc:" << std::setw(2) << static_cast<int>(nes.get_acc()) << std::endl
        << "x:" << std::setw(2) << static_cast<int>(nes.get_x()) << std::endl
        << "y:" << std::setw(2) << static_cast<int>(nes.get_y()) << std::endl
        << "status_flag:" << std::setw(2) << static_cast<int>(nes.get_status_flag().to_ulong()) << std::endl
        << "stack_ptr:" << std::setw(2) << static_cast<int>(nes.get_stack_ptr()) << std::endl;
    }

    return 0;
}