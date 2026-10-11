#pragma once
// Fixed scalar word operations for the 90-cell row stride. They affect no
// multiplication/division accumulator bank; all results wrap modulo 2^32.
namespace norrath_floor_grid_stride {
template<unsigned Shift> inline unsigned shift_left(unsigned value) {
    static_assert(Shift < 32, "word shift");
    unsigned result;
    __asm__ volatile ("sll %0, %1, %2" : "=r" (result) : "r" (value), "I" (Shift));
    return result;
}
inline unsigned add(unsigned left, unsigned right) {
    unsigned result;
    __asm__ volatile ("addu %0, %1, %2" : "=r" (result) : "r" (left), "r" (right));
    return result;
}
inline unsigned subtract(unsigned left, unsigned right) {
    unsigned result;
    __asm__ volatile ("subu %0, %1, %2" : "=r" (result) : "r" (left), "r" (right));
    return result;
}
}
