#include "NES.cpp"

int main(int argc, char*argv[]){
    NES nes = NES();
    // check if the user has send the ROM as an arg
    if(argc >= 2){
        if(!nes.load_ROM(argv[1])){
            std::cerr << "ERROR:\t Failed to load ROM" << std::endl;
            return 1;
        }
    }
    else{
        std::cerr << "ERROR:\t No ROM detected" << std::endl;
            return 1;
    }
    return 0;
}