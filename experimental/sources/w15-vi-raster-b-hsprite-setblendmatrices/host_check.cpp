// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
#include <cstring>
static unsigned char arena[1 << 17];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int log_[64], n, begin_ret, end_ret, next_calls; static int next_map[8];
static unsigned char* seen_a[64]; static const unsigned char* seen_b[64]; static const unsigned char* seen_c[64]; static int seen_i[64];
static float dtor_matrix[16]; static int dtor_flags, dtor_calls;
extern "C" int Begin__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* p) { log_[n++] = 1; (void)p; return begin_ret; }
extern "C" int End__Ct6VIPool1Z19VIHSpriteAttachment(const unsigned char* p) { log_[n++] = 2; (void)p; return end_ret; }
extern "C" int Next__Ct6VIPool1Z19VIHSpriteAttachmenti(const unsigned char* p, int i) { log_[n++] = 3; (void)p; ++next_calls; return next_map[i & 7]; }
extern "C" void Mul__10VIMatrix44RC10VIMatrix44T1(unsigned char* s, const unsigned char* a, const unsigned char* b) { log_[n] = 4; seen_a[n] = s; seen_b[n] = a; seen_c[n++] = b; }
extern "C" void SetObjectBlends__8VIRasteriP10VIMatrix44(unsigned char* r, int c, unsigned char* m) { log_[n] = 5; seen_a[n] = r; seen_i[n] = c; seen_b[n++] = m; }
extern "C" void _$_10VIMatrix44(unsigned char* m, int f) { log_[n++] = 6; std::memcpy(dtor_matrix, m, 64); dtor_flags = f; ++dtor_calls; }
#include "set_blend_matrices.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static void reset() {
    std::memset(arena, 0, sizeof arena); n = 0; next_calls = 0; dtor_calls = 0; std::memset(dtor_matrix, 0x7f, sizeof dtor_matrix);
    W(96 + 8) = arena_base + 0x6000;                       // pool table
    for (int i = 0; i < 6; ++i) W(0x6000 + 16 * i + 4) = i == 3 ? (unsigned)-1 : 5;
    for (int i = 0; i < 8; ++i) next_map[i] = i + 1;
    begin_ret = 0; end_ret = 6;
    W(64) = 2; W(72) = arena_base + 0x8000;                // first array: count, data
    W(76) = 0; W(84) = arena_base + 0xa000;                // second array data
}
int main() {
    bool ok = true;
    unsigned char* raster = arena + 0xf000;
    reset();
    ok = ok && SetBlendMatrices__9VIHSpriteR8VIRaster(arena, raster) == 0;
    // Begin, then End and Next for records 0-2, End for record 3 (found), 2 Mul, SetObjectBlends, dtor
    ok = ok && log_[0] == 1 && log_[1] == 2 && next_calls == 3 && dtor_calls == 1 && dtor_flags == 2;
    ok = ok && n == 1 + 4 + 3 + 2 + 1 + 1;
    int k = n - 4;
    ok = ok && log_[k] == 4 && seen_a[k] == arena + 0xa000 && seen_b[k] == arena + 0x8000 + 160 && seen_c[k] == arena + 0x8000 + 48
        && log_[k + 1] == 4 && seen_a[k + 1] == arena + 0xa040 && seen_b[k + 1] == arena + 0x8100 + 160 && seen_c[k + 1] == arena + 0x8100 + 48
        && log_[k + 2] == 5 && seen_a[k + 2] == raster && seen_i[k + 2] == 2 && seen_b[k + 2] == arena + 0xa000 && log_[k + 3] == 6;
    for (int i = 0; i < 16; ++i) ok = ok && dtor_matrix[i] == 0.0f;
    // no attachment with -1: nothing but the walk and the destructor
    reset(); W(0x6000 + 16 * 3 + 4) = 5;
    ok = ok && SetBlendMatrices__9VIHSpriteR8VIRaster(arena, raster) == 0 && next_calls == 6 && dtor_calls == 1 && log_[n - 2] == 2 && log_[n - 1] == 6;
    // first record matches immediately
    reset(); W(0x6000 + 4) = (unsigned)-1; W(64) = 1;
    ok = ok && SetBlendMatrices__9VIHSpriteR8VIRaster(arena, raster) == 0 && next_calls == 0 && n == 2 + 1 + 1 + 1 && log_[2] == 4;
    // empty pool
    reset(); end_ret = 0;
    ok = ok && SetBlendMatrices__9VIHSpriteR8VIRaster(arena, raster) == 0 && n == 3 && log_[2] == 6;
    // zero and negative count: no Mul, SetObjectBlends still called with the count
    reset(); W(64) = 0;
    ok = ok && SetBlendMatrices__9VIHSpriteR8VIRaster(arena, raster) == 0 && log_[n - 2] == 5 && seen_i[n - 2] == 0 && log_[n - 3] == 2;
    reset(); W(64) = (unsigned)-5;
    ok = ok && SetBlendMatrices__9VIHSpriteR8VIRaster(arena, raster) == 0 && log_[n - 2] == 5 && seen_i[n - 2] == -5 && log_[n - 3] == 2;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
