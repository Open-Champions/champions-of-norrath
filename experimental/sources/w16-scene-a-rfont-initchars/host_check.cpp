// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "init_chars.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    unsigned char d[3] = {5, 6, 255};
    W(40) = 3; W(28) = 1; W(20) = 7; W(64) = arena_base + 0x1000; W(76) = 0x500; W(84) = 0x100;
    for (int i = 0; i < 64; ++i) arena[0x1000 + i] = 0xee;
    bool ok = InitChars__7VIRFontPUc(arena, d) == 0;
    ok = ok && W(0x1000) == 0x400 && W(0x1000 + 12) == 0x400 + 35 && W(0x1000 + 24) == 0x400 + 35 + 42;
    ok = ok && *reinterpret_cast<unsigned short*>(arena + 0x1004) == 5 && *reinterpret_cast<unsigned short*>(arena + 0x1000 + 16) == 6 && *reinterpret_cast<unsigned short*>(arena + 0x1000 + 28) == 255;
    ok = ok && *reinterpret_cast<unsigned short*>(arena + 0x1008) == 0xffff && arena[0x100a] == 5 && arena[0x1000 + 22] == 6 && arena[0x1000 + 34] == 255;
    ok = ok && arena[0x100b] == 0xee && arena[0x1006] == 0xee && arena[0x1007] == 0xee;  // bytes not written stay
    W(28) = 4;
    ok = ok && InitChars__7VIRFontPUc(arena, d) == 0 && *reinterpret_cast<unsigned short*>(arena + 0x1004) == 15 && W(0x1000 + 12) == 0x400 + 105;
    W(28) = 0;
    ok = ok && InitChars__7VIRFontPUc(arena, d) == 0 && *reinterpret_cast<unsigned short*>(arena + 0x1004) == 3 && *reinterpret_cast<unsigned short*>(arena + 0x1000 + 28) == 128;
    W(28) = 9;
    ok = ok && InitChars__7VIRFontPUc(arena, d) == -1;
    W(28) = 1; W(40) = 0; W(0x1000) = 0x1234;
    ok = ok && InitChars__7VIRFontPUc(arena, d) == 0 && W(0x1000) == 0x1234;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
