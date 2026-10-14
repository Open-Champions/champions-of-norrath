// Host contract for _vt$15SpellHateEffect; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_15SpellHateEffect[6] asm("_vt$15SpellHateEffect");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf15SpellHateEffect");
extern "C" void vt_fn_0() { hit = "__tf15SpellHateEffect"; }
extern "C" void vt_fn_1() asm("_$_15SpellHateEffect");
extern "C" void vt_fn_1() { hit = "_$_15SpellHateEffect"; }
extern "C" void vt_fn_2() asm("run__15SpellHateEffectG6Point3s");
extern "C" void vt_fn_2() { hit = "run__15SpellHateEffectG6Point3s"; }
extern "C" void vt_fn_3() asm("draw__5Spell");
extern "C" void vt_fn_3() { hit = "draw__5Spell"; }
extern "C" void vt_fn_4() asm("canQueue__5Spell");
extern "C" void vt_fn_4() { hit = "canQueue__5Spell"; }
int main() {
    const char* want[] = {
        "__tf15SpellHateEffect",
        "_$_15SpellHateEffect",
        "run__15SpellHateEffectG6Point3s",
        "draw__5Spell",
        "canQueue__5Spell",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_15SpellHateEffect / sizeof vt_15SpellHateEffect[0] != 6) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 6; ++i) {
        if (vt_15SpellHateEffect[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_15SpellHateEffect[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_15SpellHateEffect[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$15SpellHateEffect slots ok\n");
    return bad != 0;
}
