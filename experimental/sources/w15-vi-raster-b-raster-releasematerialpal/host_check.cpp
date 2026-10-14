// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
static int log_[32], n;
static unsigned char* seen_a[32]; static unsigned char* seen_b[32]; static int seen_i[32];
static int remove_ret, erase_ret, rel_ret[8];
extern "C" int ReleaseMaterial__8VIRasteriR12VIDictionary(unsigned char* r, int i, unsigned char* d) { log_[n] = 4; seen_a[n] = r; seen_b[n] = d; seen_i[n++] = i; return rel_ret[i & 7]; }
extern "C" int Remove__12VIDictionaryR10VIResource(unsigned char* d, unsigned char* r) { log_[n] = 1; seen_a[n] = d; seen_b[n++] = r; return remove_ret; }
extern "C" void _$_t7VIArray1Zi(unsigned char* a, int fl) { log_[n] = 2; seen_a[n] = a; seen_i[n++] = fl; }
extern "C" void __builtin_delete(void* p) { log_[n] = 5; seen_a[n++] = (unsigned char*)p; }
extern "C" int Erase__t6VIPool1ZP13VIMaterialPali(unsigned char* p, int i) { log_[n] = 3; seen_a[n] = p; seen_i[n++] = i; return erase_ret; }
#include "release_material_pal.cpp"
#include <cstdio>
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
int main() {
    unsigned char* dict = arena + 0xa000;
    W(19428 + 8) = arena_base + 0x6000;
    for (unsigned i = 0; i < 3; ++i) W(0x6000 + 12 * i) = arena_base + 0x7000 + 0x40 * i;
    bool ok = true;
    // pal 1 at 0x7040: refcount, array at +8 (count, data pointer)
    W(0x7040) = 7; W(0x7048) = 3; W(0x704c) = arena_base + 0x8000; W(0x8000) = 10; W(0x8004) = 11; W(0x8008) = 12;
    W(0x7044) = 2; n = 0;
    ok = ok && ReleaseMaterialPal__8VIRasteriR12VIDictionary(arena, 1, dict) == 0 && W(0x7044) == 1 && n == 0;
    W(0x7044) = 1; remove_ret = 0; erase_ret = 0; n = 0;
    ok = ok && ReleaseMaterialPal__8VIRasteriR12VIDictionary(arena, 1, dict) == 0 && n == 7
        && log_[0] == 4 && seen_i[0] == 10 && log_[1] == 4 && seen_i[1] == 11 && log_[2] == 4 && seen_i[2] == 12 && seen_a[0] == arena && seen_b[0] == dict
        && log_[3] == 1 && seen_a[3] == dict && seen_b[3] == arena + 0x7040
        && log_[4] == 2 && seen_a[4] == arena + 0x7048 && seen_i[4] == 2
        && log_[5] == 5 && seen_a[5] == arena + 0x7040 && log_[6] == 3 && seen_a[6] == arena + 19428 && seen_i[6] == 1;
    // negative from the second entry stops the walk
    W(0x7044) = 1; rel_ret[11 & 7] = -1; n = 0;
    ok = ok && ReleaseMaterialPal__8VIRasteriR12VIDictionary(arena, 1, dict) == -1 && n == 2;
    rel_ret[11 & 7] = 0;
    // empty array, unregistered
    W(0x7044) = 1; W(0x7048) = 0; W(0x7040) = 0; n = 0;
    ok = ok && ReleaseMaterialPal__8VIRasteriR12VIDictionary(arena, 1, dict) == 0 && n == 3 && log_[0] == 2 && log_[1] == 5 && log_[2] == 3;
    // negative array count behaves as empty
    W(0x7044) = 1; W(0x7048) = (unsigned)-3; n = 0;
    ok = ok && ReleaseMaterialPal__8VIRasteriR12VIDictionary(arena, 1, dict) == 0 && n == 3;
    // Remove fails
    W(0x7044) = 1; W(0x7048) = 0; W(0x7040) = 5; remove_ret = -1; n = 0;
    ok = ok && ReleaseMaterialPal__8VIRasteriR12VIDictionary(arena, 1, dict) == -1 && n == 1;
    // Erase fails
    W(0x7044) = 1; remove_ret = 0; erase_ret = -1; n = 0;
    ok = ok && ReleaseMaterialPal__8VIRasteriR12VIDictionary(arena, 1, dict) == -1 && n == 4;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
