// references
// https://www.w3schools.com/cpp/cpp_enum.asp

#ifndef REGISTER_BIT

#define REGISTER_BIT
enum class register_bit{
    N = 7, // Negative
    V = 6, // Overflow
    DASH = 5, // Read 1 (unused)
    B = 4, // Break
    D = 3, // Decimal
    I = 2, // Interrupt Disable
    Z = 1, // Zero
    C = 0 // Carry
};

// return size_t based on register bit
inline size_t bit_index(register_bit bit){
    return static_cast<size_t>(bit);
}
#endif