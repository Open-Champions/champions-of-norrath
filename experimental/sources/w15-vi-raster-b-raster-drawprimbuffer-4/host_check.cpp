// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int n, log_[8], seen_i[8], seen_s[8], seen_t[8], ret_val; static unsigned seen_u[8]; static unsigned char* seen_slot; static unsigned char* seen_r[8];
extern "C" void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* r, unsigned char* s) { log_[n] = 100; seen_r[n++] = r; seen_slot = s; }
extern "C" int DrawPrimBufferPackVUNC__8VIRasteriiiUi(unsigned char* r, int i, int s, int t, unsigned u) { log_[n] = 1; seen_r[n] = r; seen_i[n] = i; seen_s[n] = s; seen_t[n] = t; seen_u[n++] = u; return ret_val; }
#include "draw_prim_buffer.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    bool ok = true;
    W(19328) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i;
    for (unsigned kind = 0; kind < 10; ++kind) {
        W(0x7048) = kind; W(19532) = 1; ret_val = 40 + kind; n = 0;
        int r = DrawPrimBuffer__8VIRasteriiiUi(arena, 1, -7, 99, 0xfffffff0u);
        if (kind != 2) { ok = ok && r == -1 && n == 0; continue; }
        ok = ok && r == 42 && n == 1 && log_[0] == 1 && seen_r[0] == arena && seen_i[0] == 1 && seen_s[0] == -7 && seen_t[0] == 99 && seen_u[0] == 0xfffffff0u;
    }
    W(0x7088) = 2; W(19532) = 0; n = 0; ret_val = 9;
    ok = ok && DrawPrimBuffer__8VIRasteriiiUi(arena, 2, 3, 4, 5) == 9 && n == 2 && log_[0] == 100 && seen_slot == arena + 19492 && log_[1] == 1 && seen_i[1] == 2;
    W(0x7088) = 1; W(19532) = 7; n = 0;
    ok = ok && DrawPrimBuffer__8VIRasteriiiUi(arena, 2, 3, 4, 5) == -1 && n == 1 && log_[0] == 100;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
