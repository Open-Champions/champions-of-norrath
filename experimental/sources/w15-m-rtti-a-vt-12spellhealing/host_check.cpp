// Host contract for _vt$12SpellHealing; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_12SpellHealing[6] asm("_vt$12SpellHealing");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf12SpellHealing");
extern "C" void vt_fn_0() { hit = "__tf12SpellHealing"; }
extern "C" void vt_fn_1() asm("_$_12SpellHealing");
extern "C" void vt_fn_1() { hit = "_$_12SpellHealing"; }
extern "C" void vt_fn_2() asm("run__12SpellHealingG6Point3s");
extern "C" void vt_fn_2() { hit = "run__12SpellHealingG6Point3s"; }
extern "C" void vt_fn_3() asm("draw__5Spell");
extern "C" void vt_fn_3() { hit = "draw__5Spell"; }
extern "C" void vt_fn_4() asm("canQueue__5Spell");
extern "C" void vt_fn_4() { hit = "canQueue__5Spell"; }
int main() {
    const char* want[] = {
        "__tf12SpellHealing",
        "_$_12SpellHealing",
        "run__12SpellHealingG6Point3s",
        "draw__5Spell",
        "canQueue__5Spell",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_12SpellHealing / sizeof vt_12SpellHealing[0] != 6) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 6; ++i) {
        if (vt_12SpellHealing[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_12SpellHealing[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_12SpellHealing[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$12SpellHealing slots ok\n");
    return bad != 0;
}
