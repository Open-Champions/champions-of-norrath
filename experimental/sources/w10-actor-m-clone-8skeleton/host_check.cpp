// Host contract for clone__8Skeleton; not an EE input. Tables are small host arrays; the allocator and the two
// copy constructors are recorded stubs that also write a marker so missed or extra copies show. The
// source pattern never equals the 0xEE fill, and each segment's expected bytes are listed explicitly.
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <string>
#include <vector>
extern "C" const unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" const unsigned char norrath_actor_view_4488f0[8] = {};
extern "C" const unsigned char norrath_ctors_lava_wraith_view_44fee8[8] = {};
static unsigned char* nb; static int fail_alloc, allocsz; static std::string order;
static std::vector<unsigned> table_seen;
static void note(const char* tag, unsigned off) { order += tag; order += std::to_string(off); order += ";"; unsigned t; std::memcpy(&t, nb + 72, 4); table_seen.push_back(t); }
extern "C" void* blockAlloc__Fi(int size) { allocsz = size; return fail_alloc ? nullptr : nb; }
extern "C" void lh_stub(void* d, const void* s) __asm__("__8ListHeadRC8ListHead");
extern "C" void lh_stub(void* d, const void* s) { note("L", static_cast<unsigned>(static_cast<unsigned char*>(d) - nb)); std::memset(d, 0, 4); (void)s; }
extern "C" void anim_stub(void* d, const void* s) __asm__("__15AnimationState8RC15AnimationState8");
extern "C" void anim_stub(void* d, const void* s) { note("A", static_cast<unsigned>(static_cast<unsigned char*>(d) - nb)); std::memset(d, 0x77, 8); (void)s; }
#include "clone.cpp"
int main() {
    alignas(16) static unsigned char src[1128], dst[1128 + 64], want[1128 + 64];
    int bad = 0;
    for (int mode = 0; mode < 2; ++mode) {
        for (int i = 0; i < 1128; ++i) src[i] = static_cast<unsigned char>((i * 7 + 13) | 1);
        std::memset(dst, 0xEE, sizeof dst); nb = dst; order.clear(); table_seen.clear(); fail_alloc = (mode == 1); allocsz = 0;
        void* r = clone__8Skeleton(src);
        if (allocsz != 1128) bad = 1;
        if (mode == 1) { if (r != nullptr || !order.empty() || std::memcmp(dst, want, 0) != 0) bad = 1; for (unsigned i = 0; i < sizeof dst; ++i) if (dst[i] != 0xEE) bad = 1; continue; }
        if (r != dst) bad = 1;
        if (order != "L0;L4;L8;A288;L576;L752;L888;L960;L964;") bad = 1;
        std::memset(want, 0xEE, sizeof want);
        const struct { unsigned a, n; } spans[] = {{12,30},{44,14},{60,12},{76,1},{80,6},{88,69},{160,14},{176,14},{192,14},{208,26},{236,50},{552,19},{572,2},{580,170},{756,45},{804,84},{892,68},{968,5},{976,68},{1048,80}};
        for (auto& sp : spans) std::memcpy(want + sp.a, src + sp.a, sp.n);
        std::memset(want, 0, 4); std::memset(want + 4, 0, 4); std::memset(want + 8, 0, 4);
        std::memset(want + 576, 0, 4); std::memset(want + 752, 0, 4); std::memset(want + 888, 0, 4); std::memset(want + 960, 0, 4); std::memset(want + 964, 0, 4);
        std::memset(want + 288, 0x77, 8);
        const unsigned gv = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_game_object_vtable_view));
        const unsigned cv = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_actor_view_4488f0));
        const unsigned fv = static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(norrath_ctors_lava_wraith_view_44fee8));
        std::memcpy(want + 72, &fv, 4);
        if (std::memcmp(dst, want, sizeof dst) != 0) bad = 1;
        // table seen at each call: GameObject for the first three heads, Creature after, the final one where noted
        const std::vector<unsigned> expect = {gv, gv, gv, cv, cv, cv, cv, fv, fv};
        if (table_seen.size() != expect.size()) bad = 1; else for (size_t i = 0; i < expect.size(); ++i) if (table_seen[i] != expect[i]) bad = 1;
    }
    return bad;
}
