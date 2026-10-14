// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "begin_char.cpp"
#include <cstdio>
int main() {
    unsigned char* a = arena;
    *reinterpret_cast<int*>(a + 36) = 2;
    *reinterpret_cast<unsigned*>(a + 64) = arena_base + 0x6000;
    *reinterpret_cast<unsigned*>(a + 84) = 0x300000u;
    unsigned char* e = arena + 0x6000 + 24;
    *reinterpret_cast<unsigned*>(e) = 0x1234u;
    *reinterpret_cast<unsigned short*>(e + 4) = 0xfffe;
    int size = 0; unsigned data = 0;
    bool ok = BeginChar__7VIRFontUsRiPPUc(a, 0x4142, &size, &data) == 0 && size == 0xfffe && data == 0x301234u && *reinterpret_cast<unsigned short*>(e + 6) == 0x4142;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
