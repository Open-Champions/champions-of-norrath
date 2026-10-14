// Host contract for clone__8Scorpion; not an EE input. Callees log; destination bytes start as
// 0xEE so a missed or extra copy shows. Checks every copied range, every untouched gap, the
// order of the callee calls and the null-allocation path.
#include <cstdint>
#include <cstring>
#include <string>
static std::string calls; static unsigned char* nb; static int fail_alloc, allocsz;
extern "C" const unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" const unsigned char norrath_actor_view_4488f0[8] = {};
extern "C" const unsigned char norrath_ctors_lava_wraith_view_44b630[8] = {};
extern "C" void* blockAlloc__Fi(int size) { allocsz = size; return fail_alloc ? nullptr : nb; }
extern "C" void lh_stub(void* d, const void* s) __asm__("__8ListHeadRC8ListHead");
extern "C" void lh_stub(void* d, const void* s) { calls += "lh" + std::to_string((unsigned char*)d - nb) + ";"; std::memset(d, 0, 4); (void)s; }
extern "C" void anim_stub(void* d, const void* s) __asm__("__15AnimationState8RC15AnimationState8");
extern "C" void anim_stub(void* d, const void* s) { calls += "anim" + std::to_string((unsigned char*)d - nb) + ";"; (void)s; }
#include "clone.cpp"
int main() {
    alignas(16) static unsigned char src[1100], dst[1100];
    int bad = 0;
    for (int mode = 0; mode < 2; ++mode) {
        for (int i = 0; i < 1100; ++i) src[i] = static_cast<unsigned char>((i * 7 + 13) | 1);
        std::memset(dst, 0xEE, sizeof dst); nb = dst; calls.clear(); fail_alloc = mode;
        void* r = clone__8Scorpion(src);
        if (allocsz != 968) bad = 1;
        if (mode == 1) { if (r != nullptr || !calls.empty()) bad = 1; continue; }
        if (r != dst) bad = 1;
        if (calls != "lh0;lh4;lh8;anim288;lh576;lh752;lh888;") bad = 1;
        const int copied[][2] = {{12, 41}, {44, 57}, {60, 71}, {76, 76}, {80, 85}, {88, 156}, {160, 173}, {176, 189}, {192, 205}, {208, 233}, {236, 285}, {552, 570}, {572, 573}, {580, 749}, {756, 800}, {804, 887}, {892, 960}};
        bool copiedFlag[1100] = {};
        for (auto& r2 : copied) for (int i = r2[0]; i <= r2[1]; ++i) { copiedFlag[i] = true; if (dst[i] != src[i]) bad = 1; }
        for (int off : {0, 4, 8, 576, 752, 888}) for (int i = 0; i < 4; ++i) { copiedFlag[off + i] = true; if (dst[off + i] != 0) bad = 1; }
        for (int i = 0; i < 1100; ++i) if (!copiedFlag[i] && i != 72 && i != 73 && i != 74 && i != 75 && !(i >= 288 && i < 552) && dst[i] != 0xEE) bad = 1;
        unsigned vt; std::memcpy(&vt, dst + 72, 4);
        if (vt != static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(norrath_ctors_lava_wraith_view_44b630))) bad = 1;
        for (int i = 288; i < 552; ++i) if (dst[i] != 0xEE) bad = 1;   // owned by the AnimationState copy constructor
    }
    return bad;
}
