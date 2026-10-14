// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "surface.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    W(0x4bbc) = arena_base + 0x6000;                      // table pointer inside the raster object
    for (unsigned i = 0; i < 5; ++i) W(0x6000 + 12 * i) = 0xa000u + i;  // first word of each 12-byte slot
    for (unsigned i = 0; i < 5; ++i) W(0x6000 + 12 * i + 4) = 0xdeadu;  // other words must not be returned
    bool ok = true;
    for (int i = 0; i < 5; ++i) ok = ok && Surface__8VIRasteri(arena, i) == 0xa000u + i;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
