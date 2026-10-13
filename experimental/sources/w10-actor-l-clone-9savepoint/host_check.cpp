// Host contract for clone__9Savepoint; not an EE input. Callees log; destination bytes start as
// 0xEE so a missed or extra copy shows. The member at +136 is checked on the clear path.
#include <cstdint>
#include <cstring>
#include <string>
static std::string calls; static unsigned char* nb; static int fail_alloc, allocsz;
extern "C" const unsigned char norrath_game_object_vtable_view[8] = {};
extern "C" const unsigned char norrath_ctors_lava_wraith_view_449550[8] = {};
extern "C" void* blockAlloc__Fi(int size) { allocsz = size; return fail_alloc ? nullptr : nb; }
extern "C" void lh_stub(void* d, const void* s) __asm__("__8ListHeadRC8ListHead");
extern "C" void lh_stub(void* d, const void* s) { calls += "lh" + std::to_string((unsigned char*)d - nb) + ";"; std::memset(d, 0, 4); (void)s; }
extern "C" void anim_stub(void* d, const void* s) __asm__("__14AnimationStateRC14AnimationState");
extern "C" void anim_stub(void* d, const void* s) { calls += "anim" + std::to_string((unsigned char*)d - nb) + ";"; (void)s; }
extern "C" void lightAdd__FP5Light(void* l) { calls += "light" + std::to_string((unsigned char*)l - nb) + ";"; }
#include "clone.cpp"
int main() {
    alignas(16) static unsigned char src[256], dst[256];
    int bad = 0;
    for (int mode = 0; mode < 5; ++mode) {
        for (int i = 0; i < 256; ++i) src[i] = static_cast<unsigned char>((i * 7 + 13) | 1);
        std::memset(dst, 0xEE, sizeof dst); nb = dst; calls.clear(); fail_alloc = (mode == 4);
        if (mode == 1) std::memset(src + 136, 0, 4);
        if (mode == 2) std::memset(src + 136, 0, 8);
        if (mode == 3) std::memset(src + 140, 0, 4);
        void* r = clone__9Savepoint(src);
        if (allocsz != 208) bad = 1;
        if (mode == 4) { if (r != nullptr || calls != "") bad = 1; continue; }
        if (r != dst) bad = 1;
        const bool light = !(mode == 2);
        std::string want = "lh0;lh4;lh8;anim76;" + std::string(light ? "light136;" : "");
        if (calls != want) bad = 1;
        auto same = [&](int a, int b) { return std::memcmp(dst + a, src + a, b - a + 1) == 0; };
        if (!same(12, 41) || !same(44, 57) || !same(60, 71) || !same(120, 135) || !same(144, 201) || !same(204, 207)) bad = 1;
        for (int i : {42, 43, 58, 59, 202, 203}) if (dst[i] != 0xEE) bad = 1;
        for (int i = 0; i < 4; ++i) if (dst[0 + i] != 0 || dst[4 + i] != 0 || dst[8 + i] != 0) bad = 1;
        for (int i = 208; i < 256; ++i) if (dst[i] != 0xEE) bad = 1;
        if (light) { for (int i = 136; i < 144; ++i) if (dst[i] != 0) bad = 1; }
        else if (std::memcmp(dst + 136, src + 136, 8)) bad = 1;
    }
    return bad;
}
