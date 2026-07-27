#include "NES.cpp"
#include <iomanip>
int main(int argc, char*argv[]){
    NES nes = NES();
    // check if the user has send the ROM as an arg
    if(argc >= 2){
        if(!nes.load_ROM(argv[1])){
            std::cerr << "ERROR:\t Failed to load ROM" << std::endl;
            // return 1;
        }
    }
    else if(nes.load_ROM("TEST\\nestest.nes")){
        std::cout << "loading Test ROM" << std::endl;
    }
    else if(!nes.load_ROM("TEST\\nestest.nes")){
        std::cerr << "ERROR:\t Failed to load test ROM" << std::endl;
            return 1;
    }
    else{
        std::cerr << "ERROR:\t No ROM detected" << std::endl;
            return 1;
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