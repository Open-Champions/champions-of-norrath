// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include <cstring>
static int log_[16], n, ret_val;
static unsigned char* seen_r[16]; static int seen_i[16], seen_s[16]; static unsigned seen_t[16]; static float seen_f[16]; static unsigned char* seen_slot;
extern "C" void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char* r, unsigned char* s) { log_[n] = 100; seen_r[n++] = r; seen_slot = s; }
#define STUB(NAME, CODE) extern "C" int NAME(unsigned char* r, int i, int s, unsigned t) { log_[n] = CODE; seen_r[n] = r; seen_i[n] = i; seen_s[n] = s; seen_t[n++] = t; return ret_val; }
STUB(DrawPrimBufferVUNC__8VIRasteriiUi, 1)
STUB(DrawPrimBufferPackVUNC__8VIRasteriiUi, 2)
STUB(DrawPrimBufferPackVUNCG__8VIRasteriiUi, 3)
STUB(DrawPrimBufferPackVUNBW__8VIRasteriiUi, 4)
extern "C" int DrawPrimBufferPackVU__8VIRasteriifUi(unsigned char* r, int i, int s, float f, unsigned t) { log_[n] = 5; seen_r[n] = r; seen_i[n] = i; seen_s[n] = s; seen_f[n] = f; seen_t[n++] = t; return ret_val; }
#include "draw_prim_buffer.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    bool ok = true;
    W(19328) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i;
    const int expect_log[9] = {1, 0, 2, 0, 3, 4, 0, 0, 5};
    for (unsigned kind = 0; kind < 12; ++kind) {
        W(0x7048) = kind; W(19532) = 1; ret_val = 40 + kind; n = 0;
        int r = DrawPrimBuffer__8VIRasteriiUi(arena, 1 + (kind & 1) * 0, -7, 0xfffffff0u);
        int want = kind < 9 ? expect_log[kind] : 0;
        if (want == 0) { ok = ok && r == -1 && n == 0; continue; }
        ok = ok && r == 40 + (int)kind && n == 1 && log_[0] == want && seen_r[0] == arena && seen_i[0] == 1 && seen_s[0] == -7 && seen_t[0] == 0xfffffff0u;
        if (want == 5) ok = ok && seen_f[0] == 1.0f;
    }
    // upload happens first when the raster word at +19532 is not 1
    W(0x7088) = 2; W(19532) = 0; n = 0; ret_val = 9;
    ok = ok && DrawPrimBuffer__8VIRasteriiUi(arena, 2, 3, 4) == 9 && n == 2 && log_[0] == 100 && seen_slot == arena + 19492 && log_[1] == 2 && seen_i[1] == 2;
    W(0x7088) = 1; W(19532) = 7; n = 0;
    ok = ok && DrawPrimBuffer__8VIRasteriiUi(arena, 2, 3, 4) == -1 && n == 1 && log_[0] == 100;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
