#include "timer_usec_to_bus_clock.cpp"

#include <cstdio>

int main() {
    if (TimerUSec2BusClock(0, 0) != 0ull) return 1;
    if (TimerUSec2BusClock(0, 1000000) != 147456000ull) return 2;
    if (TimerUSec2BusClock(1, 0) != 147456000ull) return 3;
    if (TimerUSec2BusClock(0, 1) != 147ull) return 4;
    if (TimerUSec2BusClock(0, 10) != 1474ull) return 5;
    if (TimerUSec2BusClock(2, 500000) != 2ull * 147456000ull + 73728000ull) return 6;
    // The microsecond product exceeds 32 bits; a 32-bit multiply would fail here.
    if (TimerUSec2BusClock(0, 0xffffffffu) != 633318697451ull) return 7;
    if (TimerUSec2BusClock(0xffffffffu, 0) != 0xffffffffull * 147456000ull) return 8;
    std::puts("timer usec to bus clock contract passed");
    return 0;
}
