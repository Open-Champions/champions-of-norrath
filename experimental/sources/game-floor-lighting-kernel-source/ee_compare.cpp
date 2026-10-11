#include "ee_compare.hpp"
static_assert(sizeof(float) == 4 && sizeof(unsigned) == 4,
              "requires the EE scalar word model");
extern "C" unsigned ee_c_lt_s_word(float left, float right) {
    unsigned result;
    // Keep the producer and consumer of the native condition bit in one block.
    // The false branch always executes the zero-result delay instruction.
    __asm__ volatile (
        ".set push\n\t"
        ".set noreorder\n\t"
        "c.lt.s %1, %2\n\t"
        "bc1f 1f\n\t"
        "addu %0, $zero, $zero\n\t"
        "addiu %0, $zero, 1\n\t"
        "1:\n\t"
        ".set pop"
        : "=r" (result)
        : "f" (left), "f" (right)
        : "cc");
    return result;
}
