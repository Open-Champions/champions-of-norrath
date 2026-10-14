// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "share_surface.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    W(0x4bbc) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) { W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i; W(0x7000 + 0x40 * i + 4) = 10 * i; W(0x7000 + 0x40 * i) = 0x55; }
    bool ok = ShareSurface__8VIRasteri(arena, 1) == 0 && W(0x7044) == 11 && W(0x7004) == 0 && W(0x7084) == 20 && W(0x7040) == 0x55;
    ok = ok && ShareSurface__8VIRasteri(arena, 1) == 0 && W(0x7044) == 12;
    ok = ok && ShareSurface__8VIRasteri(arena, 2) == 0 && W(0x7084) == 21;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
