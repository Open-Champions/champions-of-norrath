// Host contract for _vt$13SpellEntangle; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_13SpellEntangle[6] asm("_vt$13SpellEntangle");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf13SpellEntangle");
extern "C" void vt_fn_0() { hit = "__tf13SpellEntangle"; }
extern "C" void vt_fn_1() asm("_$_13SpellEntangle");
extern "C" void vt_fn_1() { hit = "_$_13SpellEntangle"; }
extern "C" void vt_fn_2() asm("run__13SpellEntangleG6Point3s");
extern "C" void vt_fn_2() { hit = "run__13SpellEntangleG6Point3s"; }
extern "C" void vt_fn_3() asm("draw__5Spell");
extern "C" void vt_fn_3() { hit = "draw__5Spell"; }
extern "C" void vt_fn_4() asm("canQueue__5Spell");
extern "C" void vt_fn_4() { hit = "canQueue__5Spell"; }
int main() {
    const char* want[] = {
        "__tf13SpellEntangle",
        "_$_13SpellEntangle",
        "run__13SpellEntangleG6Point3s",
        "draw__5Spell",
        "canQueue__5Spell",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_13SpellEntangle / sizeof vt_13SpellEntangle[0] != 6) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 6; ++i) {
        if (vt_13SpellEntangle[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_13SpellEntangle[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_13SpellEntangle[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$13SpellEntangle slots ok\n");
    return bad != 0;
}
