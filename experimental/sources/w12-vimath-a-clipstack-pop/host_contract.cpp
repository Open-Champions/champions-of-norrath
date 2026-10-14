#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return arena_base + static_cast<unsigned>(p - arena); }
#include "pop.cpp"
#include <string.h>

static int& W(unsigned off) { return *reinterpret_cast<int*>(arena + off); }

int main() {
    const unsigned obj = 0, entries = 256;
    memset(arena, 0x77, 64);
    for (int i = 0; i < 4; ++i) for (int j = 0; j < 4; ++j) W(entries + 16 * i + 4 * j) = 100 * i + j;
    W(obj + 0) = 3;
    W(obj + 8) = static_cast<int>(arena_base + entries);
    W(obj + 12) = 1000;
    W(obj + 24) = 2000;
    if (Pop__11VIClipStack(arena + obj) != 0) return 1;
    // entry index 2: word +0 is 200, word +8 is 202
    if (W(obj + 0) != 2) return 2;
    if (W(obj + 12) != 1000 - 202) return 3;
    if (W(obj + 24) != 2000 - 200) return 4;
    if (W(obj + 8) != static_cast<int>(arena_base + entries)) return 5;
    if (arena[4] != 0x77 || arena[16] != 0x77 || arena[28] != 0x77 || arena[63] != 0x77) return 6;
    if (Pop__11VIClipStack(arena + obj) != 0 || Pop__11VIClipStack(arena + obj) != 0) return 7;
    if (W(obj + 0) != 0) return 8;
    if (W(obj + 12) != 1000 - 202 - 102 - 2) return 9;
    if (W(obj + 24) != 2000 - 200 - 100 - 0) return 10;
    return 0;
}
