// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return arena_base + static_cast<unsigned>(p - arena); }
#include <cstring>
static int log_[64], n, new_fail, init_ret, pool_ret, size_ret, copy_fail_at, copies, sizes;
static int seen_flag[8], seen_entry[8], seen_init[2];
extern "C" void* __builtin_new(unsigned size) { log_[n++] = 1; return size == 16 && !new_fail ? arena + 0x4000 : 0; }
extern "C" void __t7VIArray1Zi(unsigned char*) { log_[n++] = 2; }
extern "C" int GetMaterialPalSize__8VIRasteri(unsigned char*, int idx) { log_[n++] = 6; ++sizes; return idx == 1 ? size_ret : 99; }
extern "C" int Init__t7VIArray1Zii(unsigned char* a, int c) { log_[n++] = 3; seen_init[0] = c; seen_init[1] = (int)(a - (arena + 0x4000)); *reinterpret_cast<int*>(a) = c; *reinterpret_cast<unsigned*>(a + 4) = arena_base + 0x5000; return init_ret; }
extern "C" int Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(unsigned char*, const unsigned*) { log_[n++] = 4; return pool_ret; }
extern "C" int CopyMaterial__8VIRasterii(unsigned char*, int e, int fl) { log_[n++] = 5; seen_entry[copies & 7] = e; seen_flag[copies & 7] = fl; int k = copies++; return k == copy_fail_at ? -1 : 100 + e; }
#include "copy_material_pal.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static void reset() {
    std::memset(arena, 0, sizeof arena); n = 0; copies = 0; sizes = 0; new_fail = 0; init_ret = 0; pool_ret = 9; size_ret = 3; copy_fail_at = -1;
    W(19428 + 8) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i;
    W(0x7048) = 3; W(0x704c) = arena_base + 0x8000; W(0x8000) = 10; W(0x8004) = 11; W(0x8008) = 12;
}
int main() {
    bool ok = true;
    reset();
    ok = ok && CopyMaterialPal__8VIRasterii(arena, 1, 5) == 9 && seen_init[0] == 3 && seen_init[1] == 8 && copies == 3 && sizes == 5
        && seen_entry[0] == 10 && seen_entry[1] == 11 && seen_entry[2] == 12 && seen_flag[0] == 5 && seen_flag[2] == 5
        && W(0x5000) == 110 && W(0x5004) == 111 && W(0x5008) == 112 && W(0x4004) == 1 && W(0x4000) == 0;
    reset();
    ok = ok && CopyMaterialPal__8VIRasterii(arena, 1, 0) == 9 && W(0x4004) == 0 && seen_flag[0] == 0;
    reset(); new_fail = 1;
    ok = ok && CopyMaterialPal__8VIRasterii(arena, 1, 1) == -1 && n == 1;
    reset(); init_ret = -1;
    ok = ok && CopyMaterialPal__8VIRasterii(arena, 1, 1) == -1 && copies == 0 && W(0x4004) == 0;
    reset(); pool_ret = -4;
    ok = ok && CopyMaterialPal__8VIRasterii(arena, 1, 1) == -1 && copies == 0;
    reset(); size_ret = 0; pool_ret = 0;
    ok = ok && CopyMaterialPal__8VIRasterii(arena, 1, 1) == 0 && copies == 0 && W(0x4004) == 1;
    reset(); copy_fail_at = 1;
    ok = ok && CopyMaterialPal__8VIRasterii(arena, 1, 1) == -1 && copies == 2 && W(0x5004) == (unsigned)-1 && W(0x5008) == 0 && W(0x4004) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
