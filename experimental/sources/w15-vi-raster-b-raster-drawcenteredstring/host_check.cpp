// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int log_[256], n, cache_ret, add_ret;
static int vx[64], vy[64], vcount; static unsigned vu[64], vv[64], vcolor[64][4], v52[64], v56[64]; static int vmode[64];
static int cache_glyphs[8] = {2, 0, 1, 2, 0, 0, 0, 0};
extern "C" int Cache__7VIRFontR8VIRasterPCciPi(unsigned char*, unsigned char* r, const char*, int limit, int* out) { log_[n++] = 1; for (int i = 0; i < 8; ++i) out[i] = cache_glyphs[i]; (void)r; return limit == 256 ? cache_ret : -99; }
extern "C" int AddMaterial__8VIRasteriPP10_sceDmaTag(unsigned char*, int m, unsigned char*) { log_[n++] = 2; return m == 77 ? add_ret : -99; }
extern "C" void UploadRasterMicro__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned char*) { log_[n++] = 3; }
extern "C" void CalcMatrices__8VIRaster(unsigned char*) { log_[n++] = 4; }
extern "C" void UploadMatrices__8VIRasterPP10_sceDmaTag(unsigned char*, unsigned char*) { log_[n++] = 5; }
extern "C" void Vertex__8VIRasterRC8VIVect2I(unsigned char* r, const int* p) {
    log_[n++] = 6; vx[vcount] = p[0]; vy[vcount] = p[1];
    vu[vcount] = *reinterpret_cast<unsigned*>(r + 12); vv[vcount] = *reinterpret_cast<unsigned*>(r + 16);
    std::memcpy(vcolor[vcount], r + 32, 16); v52[vcount] = *reinterpret_cast<unsigned*>(r + 52); v56[vcount] = *reinterpret_cast<unsigned*>(r + 56);
    vmode[vcount] = *reinterpret_cast<int*>(r + 17884); ++vcount;
}
extern "C" void EndTriFan__8VIRaster(unsigned char*) { log_[n++] = 7; }
#include "draw_centered_string.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static void reset() {
    std::memset(arena, 0, sizeof arena); n = 0; vcount = 0; cache_ret = 2; add_ret = 0;
    W(19516) = arena_base + 0x6000;
    for (unsigned i = 0; i < 2; ++i) W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x200 * i;
    unsigned f = 0x7200;                                          // font 1
    W(f + 16) = 77; W(f + 20) = 14; W(f + 64) = arena_base + 0x8000; W(f + 68) = arena_base + 0x9000;
    // glyph records (12 bytes): halfword +8 = quad number, byte +10 = advance
    for (unsigned g = 0; g < 3; ++g) { *reinterpret_cast<unsigned short*>(arena + 0x8000 + 12 * g + 8) = (unsigned short)(2 - g); arena[0x8000 + 12 * g + 10] = (unsigned char)(5 + 2 * g); }
    for (unsigned q = 0; q < 3; ++q) for (unsigned k = 0; k < 14; ++k) W(0x9000 + 56 * q + 4 * k) = 1000 * q + k;
    W(19532) = 1; W(17888) = 0; W(19492) = 0x4000;
    for (int i = 0; i < 4; ++i) W(0xa000 + 4 * i) = 0x11110000 + i;
}
int main() {
    bool ok = true;
    int pos[2] = {100, 200};
    unsigned char* r = arena;
    reset();
    int ret = DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 0);
    // glyphs 2,0 -> advances 9,5 -> width (9+1)+(5+1) = 16; x = 100 - 8 = 92; y = 200 - 7 = 193; bottom = 193 + 14
    ok = ok && ret == 0 && vcount == 8 && log_[0] == 1 && log_[1] == 2 && n == 2 + 5 * 2 && vmode[0] == 2;
    ok = ok && vx[0] == 92 && vy[0] == 193 && vx[1] == 101 && vy[1] == 193 && vx[2] == 101 && vy[2] == 207 && vx[3] == 92 && vy[3] == 207;
    ok = ok && vx[4] == 102 && vy[4] == 193 && vx[5] == 107 && vx[6] == 107 && vy[6] == 207 && vx[7] == 102;
    // quad of glyph 2 has number (2 - 2) = 0, glyph 0 has number 2; texture words come from quad +24, +32, +40, +48
    ok = ok && vu[0] == 6 && vv[0] == 7 && vu[1] == 8 && vv[1] == 9 && vu[2] == 10 && vv[2] == 11 && vu[3] == 12 && vv[3] == 13;
    ok = ok && vu[4] == 2006 && vv[4] == 2007 && vu[7] == 2012;
    ok = ok && vcolor[0][0] == 0x11110000 && vcolor[3][3] == 0x11110003 && v56[0] == 0x4010 && v52[0] == 0x4030;
    ok = ok && *reinterpret_cast<unsigned*>(r + 48) == 0 && *reinterpret_cast<unsigned*>(r + 17884) == 2;
    // mode 1: only y moves; mode 2: only x; mode 3 like 0; others fail without drawing
    reset(); DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 1);
    ok = ok && vx[0] == 100 && vy[0] == 193;
    reset(); DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 2);
    ok = ok && vx[0] == 92 && vy[0] == 200 && vy[2] == 214;
    reset(); DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 3);
    ok = ok && vx[0] == 92 && vy[0] == 193;
    reset(); ok = ok && DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 4) == -1 && vcount == 0 && n == 2;
    reset(); cache_ret = -1;
    ok = ok && DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 0) == -1 && n == 1;
    reset(); add_ret = -3;
    ok = ok && DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 0) == -1 && n == 2;
    reset(); cache_ret = 0;
    ok = ok && DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 0) == 0 && n == 2 && vcount == 0;
    // micro not current and matrices dirty: the extra calls come before each glyph
    reset(); W(19532) = 0; W(17888) = 1; cache_ret = 1;
    ok = ok && DrawCenteredString__8VIRasteriRC8VIVect2IRC10VIColor32FPCcUi(r, 1, pos, reinterpret_cast<const unsigned*>(arena + 0xa000), "ab", 0) == 0
        && n == 2 + 3 + 5 && log_[2] == 3 && log_[3] == 4 && log_[4] == 5 && log_[5] == 6;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
