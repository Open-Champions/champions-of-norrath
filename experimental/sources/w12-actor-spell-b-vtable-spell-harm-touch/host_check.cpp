// Host check only. Not an EE build input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt_spell_harm_touch.cpp
// Every slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstddef>

const char* hit = "";
int same(const char* a, const char* b) {
    for (unsigned i = 0;; ++i) {
        if (a[i] != b[i]) return 0;
        if (a[i] == 0) return 1;
    }
}
extern "C" void vt_fn_0() asm("__tf14SpellHarmTouch");
extern "C" void vt_fn_0() { hit = "__tf14SpellHarmTouch"; }
extern "C" void vt_fn_1() asm("_$_14SpellHarmTouch");
extern "C" void vt_fn_1() { hit = "_$_14SpellHarmTouch"; }
extern "C" void vt_fn_2() asm("run__14SpellHarmTouchG6Point3s");
extern "C" void vt_fn_2() { hit = "run__14SpellHarmTouchG6Point3s"; }
extern "C" void vt_fn_3() asm("draw__5Spell");
extern "C" void vt_fn_3() { hit = "draw__5Spell"; }
extern "C" void vt_fn_4() asm("canQueue__5Spell");
extern "C" void vt_fn_4() { hit = "canQueue__5Spell"; }
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_spell_harm_touch[6] asm("_vt$14SpellHarmTouch");

int main() {
    static const char* const want[5] = {
    "__tf14SpellHarmTouch",
    "_$_14SpellHarmTouch",
    "run__14SpellHarmTouchG6Point3s",
    "draw__5Spell",
    "canQueue__5Spell",
    };
    if (sizeof(vt_spell_harm_touch) != 6 * sizeof(VtSlot)) return 1;
    for (int i = 0; i < 5; ++i) {
        if (vt_spell_harm_touch[i].adjustment != 0) return 2;
        hit = "";
        vt_spell_harm_touch[i].function();
        if (!same(hit, want[i])) {
            std::printf("slot %d: got %s want %s\n", i, hit, want[i]);
            return 3;
        }
    }
    if (vt_spell_harm_touch[5].adjustment != 0 || vt_spell_harm_touch[5].function != nullptr) return 4;
    std::printf("ok 14SpellHarmTouch 6 slots\n");
    return 0;
}
