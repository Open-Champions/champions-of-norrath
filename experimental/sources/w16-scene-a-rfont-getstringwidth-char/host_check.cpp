// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include "get_string_width.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static void rec(unsigned i, unsigned short key, unsigned char width) {
    *reinterpret_cast<unsigned short*>(arena + 0x1000 + 12 * i + 6) = key; arena[0x1000 + 12 * i + 10] = width;
}
int main() {
    W(36) = 6; W(64) = arena_base + 0x1000;
    const unsigned short keys[6] = {32, 63, 65, 66, 200, 255};
    for (unsigned i = 0; i < 6; ++i) rec(i, keys[i], 10 + i);
    bool ok = GetStringWidth__7VIRFontPCci(arena, "", 1) == 0;
    ok = ok && GetStringWidth__7VIRFontPCci(arena, "AB", 1) == (12 + 1) + (13 + 1);
    ok = ok && GetStringWidth__7VIRFontPCci(arena, "A B", 0) == 12 + 10 + 13;
    ok = ok && GetStringWidth__7VIRFontPCci(arena, "A\xc8\xff", 2) == 14 + 16 + 17;   // 200 and 255 are found
    ok = ok && GetStringWidth__7VIRFontPCci(arena, "AzB", 1) == (12 + 1) + (11 + 1) + (13 + 1);  // 'z' missing: uses '?'
    rec(1, 64, 11);  // no '?' any more
    ok = ok && GetStringWidth__7VIRFontPCci(arena, "AzB", 1) == -1 && GetStringWidth__7VIRFontPCci(arena, "AB", 1) == 27;
    W(36) = 0;
    ok = ok && GetStringWidth__7VIRFontPCci(arena, "A", 1) == -1 && GetStringWidth__7VIRFontPCci(arena, "", 1) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
