// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static unsigned char* seen_font; static const void* seen_text; static int seen_flag, calls;
extern "C" int GetStringWidth__7VIRFontPCUsi(unsigned char* font, const unsigned short* text, int flag) { seen_font = font; seen_text = text; seen_flag = flag; ++calls; return 321; }
#include "get_string_width.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    W(0x4c3c) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i;
    const char* msg = "abc";
    bool ok = GetStringWidth__8VIRasteriPCUs(arena, 2, (const unsigned short*)msg) == 321 && calls == 1 && seen_font == arena + 0x7080 && seen_text == (const void*)msg && seen_flag == 1;
    ok = ok && GetStringWidth__8VIRasteriPCUs(arena, 0, (const unsigned short*)msg) == 321 && calls == 2 && seen_font == arena + 0x7000;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
