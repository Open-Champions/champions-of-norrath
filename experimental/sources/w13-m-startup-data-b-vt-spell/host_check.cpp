// Host check only. Not an EE build input.
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// (host_check.cpp includes the unit; do not also list the unit on the command line.)
// Every slot must hold a zero adjustment and the stub declared at that position;
// the table length and the null last slot are checked too. The stubs only record the slot
// order; they do not check the image symbol names, which the object-level check covers.
#include <cstdio>
#include "vt_spell.cpp"

extern "C" void vt_spell_fn_0() {}
extern "C" void vt_spell_fn_1() {}
extern "C" void vt_spell_fn_2() {}
extern "C" void vt_spell_fn_3() {}
extern "C" void vt_spell_fn_4() {}

int main() {
    static_assert(sizeof(vt_spell) / sizeof(VtSlot) == 6, "slot count");
    void (*expected[6])() = {vt_spell_fn_0, vt_spell_fn_1, vt_spell_fn_2, vt_spell_fn_3, vt_spell_fn_4, nullptr};
    for (unsigned i = 0; i != 6; ++i)
        if (vt_spell[i].adjustment != 0 || vt_spell[i].function != expected[i]) { std::puts("FAIL"); return 1; }
    std::puts("PASS");
    return 0;
}
