// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
extern "C" unsigned norrath_host_address(const unsigned char* p) { return arena_base + static_cast<unsigned>(p - arena); }
#include <cstring>
static int log_[64], n, new_fail, init_ret, pool_ret, list_fail_at, adds;
static unsigned new_size, seen_elem; static unsigned char* seen_pool; static unsigned char* seen_list[8]; static unsigned char* seen_arr[2];
extern "C" void* __builtin_new(unsigned size) { log_[n++] = 1; new_size = size; return new_fail ? 0 : arena + 0x4000; }
extern "C" void __t7VIArray1Zi(unsigned char* a) { log_[n++] = 2; seen_arr[0] = a; }
extern "C" int Init__t7VIArray1Zii(unsigned char* a, int c) { log_[n++] = 3; seen_arr[1] = a; *reinterpret_cast<int*>(a) = c; *reinterpret_cast<unsigned*>(a + 4) = arena_base + 0x5000; return init_ret; }
extern "C" int Add__t6VIPool1ZP13VIMaterialPalRCP13VIMaterialPal(unsigned char* p, const unsigned* e) { log_[n++] = 4; seen_pool = p; seen_elem = *e; return pool_ret; }
extern "C" unsigned Add__t6VIList1Z10VIMaterial(unsigned char* l) {
    log_[n++] = 5; seen_list[adds & 7] = l;
    if (adds == list_fail_at) { ++adds; return (unsigned)-1; }
    return arena_base + 0x6000 + 0x40 * adds++;
}
#include "create_material_pal.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static void reset() { std::memset(arena, 0, sizeof arena); n = 0; adds = 0; new_fail = 0; init_ret = 0; pool_ret = 9; list_fail_at = -1; W(17872) = 0x11112222; W(17876) = 0x33334444; }
int main() {
    bool ok = true;
    reset();
    ok = ok && CreateMaterialPal__8VIRasterii(arena, 3, 1) == 9 && n == 7 && new_size == 16 && log_[0] == 1 && log_[1] == 2 && log_[2] == 3 && log_[3] == 4
        && seen_arr[0] == arena + 0x4008 && seen_arr[1] == arena + 0x4008 && seen_pool == arena + 19428 && seen_elem == arena_base + 0x4000
        && W(0x4000) == 0 && W(0x4004) == 1 && W(0x5000) == arena_base + 0x6000 && W(0x5004) == arena_base + 0x6040 && W(0x5008) == arena_base + 0x6080
        && seen_list[0] == arena + 19416 && seen_list[2] == arena + 19416
        && W(0x6000 + 24) == 0x11112222 && W(0x6000 + 28) == 0x33334444 && W(0x6080 + 24) == 0x11112222 && W(0x6000 + 20) == 0;
    reset();
    ok = ok && CreateMaterialPal__8VIRasterii(arena, 2, 0) == 9 && W(0x4004) == 0;
    reset(); new_fail = 1;
    ok = ok && CreateMaterialPal__8VIRasterii(arena, 2, 1) == -1 && n == 1;
    reset(); init_ret = -2;
    ok = ok && CreateMaterialPal__8VIRasterii(arena, 2, 1) == -1 && n == 3 && W(0x4004) == 0;
    reset(); pool_ret = -1;
    ok = ok && CreateMaterialPal__8VIRasterii(arena, 2, 1) == -1 && n == 4;
    reset(); pool_ret = 0;
    ok = ok && CreateMaterialPal__8VIRasterii(arena, 0, 1) == 0 && n == 4 && W(0x4004) == 1;
    reset(); list_fail_at = 1;
    ok = ok && CreateMaterialPal__8VIRasterii(arena, 3, 1) == -1 && n == 6 && W(0x5004) == (unsigned)-1 && W(0x4004) == 0 && W(0x6040 + 24) == 0;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
