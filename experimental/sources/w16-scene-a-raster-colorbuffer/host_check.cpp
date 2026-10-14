// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "color_buffer.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    W(0x4ba4) = arena_base + 0x6000;
    for (unsigned i = 0; i < 4; ++i) { W(0x6000 + 12 * i) = 0x1000 + i; W(0x6000 + 12 * i + 4) = 0xffffffffu; W(0x6000 + 12 * i + 8) = 0xffffffffu; }
    bool ok = ColorBuffer__8VIRasteri(arena, 0) == 0x1000 && ColorBuffer__8VIRasteri(arena, 3) == 0x1003;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
