// Host check only; not a build input.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 12];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "next.cpp"
int main() {
    unsigned pool[6] = {0, 0, arena_base + 0x10, 0, 0, 0};
    int values[5] = {3, -1, 0, 0x7ffffffe, -2};
    int want[5] = {3, 0x7fffffff, 0, 0x7ffffffe, -2};
    for (int i = 0; i < 5; ++i) *reinterpret_cast<int*>(arena + 0x10 + i * 104 + 96) = values[i];
    for (int i = 0; i < 5; ++i) if (Next__Ct6VIPool1Z13VIHSpritePlayi(pool, i) != want[i]) return 1 + i;
    return 0;
}
