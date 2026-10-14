// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "pixel_store.cpp"
#include <cstdio>
int main() {
    *reinterpret_cast<unsigned*>(arena + 28) = 0xdeadbeefu;
    *reinterpret_cast<unsigned*>(arena + 32) = 7;
    bool ok = PixelStore__7VIRFont(arena) == 0xdeadbeefu;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
