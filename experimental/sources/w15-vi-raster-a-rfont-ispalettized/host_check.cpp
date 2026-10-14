// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "is_palettized.cpp"
#include <cstdio>
int main() {
    unsigned& w = *reinterpret_cast<unsigned*>(arena + 52);
    bool ok = true;
    w = 0; ok = ok && IsPalettized__7VIRFont(arena) == 0;
    w = 1; ok = ok && IsPalettized__7VIRFont(arena) == 1;
    w = 0x80000000u; ok = ok && IsPalettized__7VIRFont(arena) == 1;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
