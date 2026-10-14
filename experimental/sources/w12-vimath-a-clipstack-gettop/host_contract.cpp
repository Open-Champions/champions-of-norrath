#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return arena_base + static_cast<unsigned>(p - arena); }
#include "gettop.cpp"
#include <string.h>

static int& W(unsigned off) { return *reinterpret_cast<int*>(arena + off); }

int main() {
    const unsigned obj = 0, entries = 256, vectors = 1024, planes = 4096;
    memset(arena, 0x77, 64);
    const int table[3][4] = {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 4; ++j) W(entries + 16 * i + 4 * j) = table[i][j];
    W(obj + 0) = 3;
    W(obj + 8) = static_cast<int>(arena_base + entries);
    W(obj + 20) = static_cast<int>(arena_base + vectors);
    W(obj + 32) = static_cast<int>(arena_base + planes);
    unsigned char copy[64]; memcpy(copy, arena, sizeof copy);
    int b = -1, a = -1; unsigned v = 0, p = 0;
    if (GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(arena + obj, &b, &v, &a, &p) != 0) return 1;
    if (b != 11 || a != 9) return 2;
    if (v != arena_base + vectors + 12 * 12) return 3;
    if (p != arena_base + planes + 16 * 10) return 4;
    if (memcmp(copy, arena, sizeof copy) != 0) return 5;
    W(obj + 0) = 1;
    if (GetTop__11VIClipStackRiPP7VIVect3T1PP7VIPlane(arena + obj, &b, &v, &a, &p) != 0) return 6;
    if (b != 3 || a != 1 || v != arena_base + vectors + 4 * 12 || p != arena_base + planes + 2 * 16) return 7;
    return 0;
}
