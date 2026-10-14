// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "get_point_size.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    W(0x4c3c) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) {
        W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i;
        W(0x7000 + 0x40 * i + 20) = 100 + i;
        W(0x7000 + 0x40 * i + 16) = 0xffffffffu;
        W(0x7000 + 0x40 * i + 24) = 0xffffffffu;
    }
    bool ok = GetPointSize__8VIRasteri(arena, 0) == 100 && GetPointSize__8VIRasteri(arena, 1) == 101 && GetPointSize__8VIRasteri(arena, 2) == 102;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
