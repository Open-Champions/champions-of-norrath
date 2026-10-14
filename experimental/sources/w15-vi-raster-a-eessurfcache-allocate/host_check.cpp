// Host check only; not a build input. Pointer words are 32-bit on the target, so the host maps them through a small arena.
#define NORRATH_HOST_ARENA
static unsigned char arena[1 << 16];
static const unsigned arena_base = 0x100000u;
extern "C" unsigned char* norrath_host_pointer(unsigned address) { return arena + (address - arena_base); }
#include <cstdio>
#include <cstring>
#include <initializer_list>
static unsigned key_word, nins, ins[4][4], ins_pos[4], next_calls, add_calls, added_index[4];
static int erase_calls, erase_arg, erase_result, insert_result;
extern "C" {
const unsigned* Key__Ct10VIMultiMap2ZUiZii(unsigned char*, int) { return &key_word; }
int Erase__t10VIMultiMap2ZUiZii(unsigned char*, int i) { ++erase_calls; erase_arg = i; return erase_result; }
int Next__Ct6VIPool1Z10VIEEBucketi(unsigned char*, int i) { ++next_calls; return i + 100; }
int Insert__t6VIPool1Z10VIEEBucketRC10VIEEBucketi(unsigned char*, const unsigned* b, int pos) {
    std::memcpy(ins[nins], b, 16); ins_pos[nins] = static_cast<unsigned>(pos); ++nins; return insert_result;
}
int AddToFreeMap__13VIEESurfCachei(unsigned char*, int i) { added_index[add_calls++] = static_cast<unsigned>(i); return 0; }
}
#include "allocate.cpp"
static unsigned& W(unsigned off) { return *reinterpret_cast<unsigned*>(arena + off); }
static unsigned* B(int i) { return reinterpret_cast<unsigned*>(arena + 0x6000 + 24 * i); }
static void setup(unsigned start, unsigned state, int link) {
    std::memset(arena, 0, 0x7000);
    W(40) = arena_base + 0x6000; W(20) = 1000;
    B(2)[0] = start; B(2)[2] = state; B(2)[3] = static_cast<unsigned>(link);
    nins = next_calls = add_calls = 0; erase_calls = 0; erase_result = 0; insert_result = 55;
}
int main() {
    bool ok = true;
    unsigned char req[12];
    auto R = [&](unsigned idx, unsigned tag, unsigned size) { std::memcpy(req, &idx, 4); std::memcpy(req + 4, &tag, 4); std::memcpy(req + 8, &size, 4); };
    // exact fit: no split
    setup(0x10000, 255, 7); key_word = 0xab000000u | 4096; R(2, 9, 4096);
    ok = ok && Allocate__13VIEESurfCacheR13VIEESurfAlloc(arena, req) == 0 && erase_calls == 1 && erase_arg == 7 && nins == 0
        && B(2)[1] == 4096 && B(2)[2] == 9 && B(2)[3] == 0xffffffffu && W(20) == 5096;
    // free bucket with room for an aligned tail: size 0x6000 at 0x10100, want 0x100 -> aligned tail at 0x12000
    setup(0x10100, 255, 7); key_word = 0x6000; R(2, 9, 0x100);
    ok = ok && Allocate__13VIEESurfCacheR13VIEESurfAlloc(arena, req) == 0 && nins == 2
        && ins[0][0] == 0x12000 && ins[0][1] == 0x6000 - (0x12000 - 0x10100) && ins[0][2] == 255 && ins[0][3] == 0xffffffffu && ins_pos[0] == 102 && added_index[0] == 55
        && ins[1][0] == 0x10200 && ins[1][1] == (0x12000 - 0x10100) - 0x100 && ins[1][2] == 9
        && B(2)[1] == 0x100 && B(2)[2] == 9;
    // used bucket (state != 255) with leftover: only the second split
    setup(0x10100, 3, 7); key_word = 0x6000; R(2, 9, 0x100);
    ok = ok && Allocate__13VIEESurfCacheR13VIEESurfAlloc(arena, req) == 0 && nins == 1 && ins[0][0] == 0x10200 && ins[0][1] == 0x5f00;
    // errors
    setup(0x10100, 255, -1); key_word = 0x6000; R(2, 9, 0x100);
    ok = ok && Allocate__13VIEESurfCacheR13VIEESurfAlloc(arena, req) == -1 && erase_calls == 0;
    setup(0x10100, 255, 3); key_word = 0x6000; R(2, 9, 0x100); erase_result = -1;
    ok = ok && Allocate__13VIEESurfCacheR13VIEESurfAlloc(arena, req) == -1 && B(2)[3] == 3;
    setup(0x10100, 255, 3); key_word = 0x6000; R(2, 9, 0x100); insert_result = -1;
    ok = ok && Allocate__13VIEESurfCacheR13VIEESurfAlloc(arena, req) == -1 && add_calls == 0 && W(20) == 1000;
    std::puts(ok ? "ok" : "FAIL");
    return !ok;
}
