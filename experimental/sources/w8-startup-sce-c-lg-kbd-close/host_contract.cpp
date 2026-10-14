#include <cstdio>
#include <cstring>
#include "kbd_close.cpp"
extern "C" alignas(4) unsigned char GLibData[0x668] = {};
static int waits, signals, wait_sem = -1, signal_sem = -1, open_at_wait;
static void put(int offset, int value) { std::memcpy(GLibData + offset, &value, 4); }
static int get(int offset) { int value; std::memcpy(&value, GLibData + offset, 4); return value; }
extern "C" int norrath_player_wait_semaphore(int semaphore) { ++waits; wait_sem = semaphore; open_at_wait = get(0x58); return 0; }
extern "C" int norrath_player_signal_semaphore(int semaphore) { ++signals; signal_sem = semaphore; return 0; }
int main() {
    put(0x2c, 77); put(0x40, 3); put(0x58, 5);
    // Out of range: no semaphore use, no changes.
    if (lgKbdClose(0) != 0x80000004u || lgKbdClose(-4) != 0x80000004u || lgKbdClose(5) != 0x80000004u) return 1;
    if (waits != 0 || signals != 0 || get(0x58) != 5) return 2;
    // Slot 1 (handle 2) in state 4 becomes 1.
    put(0x88 + 272 + 0x1c, 4); put(0x88 + 0x1c, 4);
    if (lgKbdClose(2) != 0u) return 3;
    if (waits != 1 || signals != 1 || wait_sem != 77 || signal_sem != 77 || open_at_wait != 5 || get(0x58) != 4) return 4;
    if (get(0x88 + 272 + 0x1c) != 1 || get(0x88 + 0x1c) != 4) return 5;
    // Other states are left alone, the count still drops.
    put(0x88 + 0x1c, 2);
    if (lgKbdClose(1) != 0u || get(0x88 + 0x1c) != 2 || get(0x58) != 3) return 6;
    // The highest valid index is the limit itself (handle limit + 1).
    put(0x88 + 3 * 272 + 0x1c, 4);
    if (lgKbdClose(4) != 0u || get(0x88 + 3 * 272 + 0x1c) != 1) return 7;
    std::puts("lgKbdClose contract passed");
    return 0;
}
