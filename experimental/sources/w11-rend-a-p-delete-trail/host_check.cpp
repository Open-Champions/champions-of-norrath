#include <cstdio>
extern "C" unsigned char trail[66560];
extern "C" void P_DeleteTrail__Fi(int handle);
unsigned char trail[66560];
static unsigned half(int slot) { return trail[slot * 1040 + 1024] | (trail[slot * 1040 + 1025] << 8); }
static void setHalf(int slot, unsigned v) { trail[slot * 1040 + 1024] = v; trail[slot * 1040 + 1025] = v >> 8; }
int main() {
    setHalf(5, 0x0305); setHalf(6, 0x0306);
    P_DeleteTrail__Fi(0x0305);
    if (half(5) != 0 || half(6) != 0x0306) return 1;
    // Stale handle with the same slot: unchanged.
    setHalf(5, 0x0405);
    P_DeleteTrail__Fi(0x0305);
    if (half(5) != 0x0405) return 2;
    // Zero handle: nothing happens (slot 0 untouched even though 0 & 0xff selects it).
    setHalf(0, 0x0100);
    P_DeleteTrail__Fi(0);
    if (half(0) != 0x0100) return 3;
    // Slot 63, negative (sign-extended) halfword handle.
    setHalf(63, 0x803f);
    P_DeleteTrail__Fi(-32705);
    if (half(63) != 0) return 4;
    // Handle bit patterns that differ only above 16 bits do not match.
    setHalf(7, 0x0107);
    P_DeleteTrail__Fi(0x10107);
    if (half(7) != 0x0107) return 5;
    std::puts("P_DeleteTrail contract passed");
    return 0;
}
