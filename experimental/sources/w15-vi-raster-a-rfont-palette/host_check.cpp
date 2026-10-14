// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "palette.cpp"
#include <cstdio>
int main() {
    unsigned char* a = arena;
    *reinterpret_cast<unsigned*>(a + 32) = 0x11111111u;
    *reinterpret_cast<unsigned*>(a + 72) = 0x22222222u;
    int count = 77; unsigned store = 88, data = 99;
    *reinterpret_cast<unsigned*>(a + 52) = 0;
    bool ok = Palette__7VIRFontRiR12VIPixelStorePPUc(a, &count, &store, &data) == -1 && count == 77 && store == 88 && data == 99;
    *reinterpret_cast<unsigned*>(a + 52) = 256;
    ok = ok && Palette__7VIRFontRiR12VIPixelStorePPUc(a, &count, &store, &data) == 0 && count == 256 && store == 0x11111111u && data == 0x22222222u;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
