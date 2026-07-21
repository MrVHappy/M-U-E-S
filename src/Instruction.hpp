// references:
// https://stackoverflow.com/questions/1350376/function-pointer-as-a-member-of-a-c-struct

#ifndef INSTRUCTION_H

#define INSTRUCTION_H
#include <bitset>
class NES;
typedef struct Instruction
{
    // function pointer to operation
    void (NES::*operation)();
    // function pointer to addressing mode
    uint16_t (NES::*addressing_mode)();
    // number of cycles
    uint8_t cycles;
} Instruction;


#endif