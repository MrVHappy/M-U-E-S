cmake_minimum_required(VERSION 3.10)
project(NES_Emulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(nes_emulator src/main.cpp)