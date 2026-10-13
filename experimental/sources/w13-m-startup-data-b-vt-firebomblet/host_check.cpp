// Host check only. Not an EE build input.
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// (host_check.cpp includes the unit; do not also list the unit on the command line.)
// Every slot must hold a zero adjustment and the stub declared at that position;
// the table length and the null last slot are checked too. The stubs only record the slot
// order; they do not check the image symbol names, which the object-level check covers.
#include <cstdio>
#include "vt_firebomblet.cpp"

extern "C" void vt_firebomblet_fn_0() {}
extern "C" void vt_firebomblet_fn_1() {}
extern "C" void vt_firebomblet_fn_2() {}
extern "C" void vt_firebomblet_fn_3() {}
extern "C" void vt_firebomblet_fn_4() {}
extern "C" void vt_firebomblet_fn_5() {}
extern "C" void vt_firebomblet_fn_6() {}
extern "C" void vt_firebomblet_fn_7() {}
extern "C" void vt_firebomblet_fn_8() {}
extern "C" void vt_firebomblet_fn_9() {}
extern "C" void vt_firebomblet_fn_10() {}
extern "C" void vt_firebomblet_fn_11() {}
extern "C" void vt_firebomblet_fn_12() {}
extern "C" void vt_firebomblet_fn_13() {}
extern "C" void vt_firebomblet_fn_14() {}
extern "C" void vt_firebomblet_fn_15() {}
extern "C" void vt_firebomblet_fn_16() {}

int main() {
    static_assert(sizeof(vt_firebomblet) / sizeof(VtSlot) == 18, "slot count");
    void (*expected[18])() = {vt_firebomblet_fn_0, vt_firebomblet_fn_1, vt_firebomblet_fn_2, vt_firebomblet_fn_3, vt_firebomblet_fn_4, vt_firebomblet_fn_5, vt_firebomblet_fn_6, vt_firebomblet_fn_7, vt_firebomblet_fn_8, vt_firebomblet_fn_9, vt_firebomblet_fn_10, vt_firebomblet_fn_11, vt_firebomblet_fn_12, vt_firebomblet_fn_13, vt_firebomblet_fn_14, vt_firebomblet_fn_15, vt_firebomblet_fn_16, nullptr};
    for (unsigned i = 0; i != 18; ++i)
        if (vt_firebomblet[i].adjustment != 0 || vt_firebomblet[i].function != expected[i]) { std::puts("FAIL"); return 1; }
    std::puts("PASS");
    return 0;
}
