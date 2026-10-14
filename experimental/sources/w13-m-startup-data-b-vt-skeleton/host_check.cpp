// Host check only. Not an EE build input.
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// (host_check.cpp includes the unit; do not also list the unit on the command line.)
// Every slot must hold a zero adjustment and the stub declared at that position;
// the table length and the null last slot are checked too. The stubs only record the slot
// order; they do not check the image symbol names, which the object-level check covers.
#include <cstdio>
#include "vt_skeleton.cpp"

extern "C" void vt_skeleton_fn_0() {}
extern "C" void vt_skeleton_fn_1() {}
extern "C" void vt_skeleton_fn_2() {}
extern "C" void vt_skeleton_fn_3() {}
extern "C" void vt_skeleton_fn_4() {}
extern "C" void vt_skeleton_fn_5() {}
extern "C" void vt_skeleton_fn_6() {}
extern "C" void vt_skeleton_fn_7() {}
extern "C" void vt_skeleton_fn_8() {}
extern "C" void vt_skeleton_fn_9() {}
extern "C" void vt_skeleton_fn_10() {}
extern "C" void vt_skeleton_fn_11() {}
extern "C" void vt_skeleton_fn_12() {}
extern "C" void vt_skeleton_fn_13() {}
extern "C" void vt_skeleton_fn_14() {}
extern "C" void vt_skeleton_fn_15() {}
extern "C" void vt_skeleton_fn_16() {}
extern "C" void vt_skeleton_fn_17() {}
extern "C" void vt_skeleton_fn_18() {}
extern "C" void vt_skeleton_fn_19() {}
extern "C" void vt_skeleton_fn_20() {}
extern "C" void vt_skeleton_fn_21() {}
extern "C" void vt_skeleton_fn_22() {}
extern "C" void vt_skeleton_fn_23() {}
extern "C" void vt_skeleton_fn_24() {}
extern "C" void vt_skeleton_fn_25() {}
extern "C" void vt_skeleton_fn_26() {}
extern "C" void vt_skeleton_fn_27() {}
extern "C" void vt_skeleton_fn_28() {}

int main() {
    static_assert(sizeof(vt_skeleton) / sizeof(VtSlot) == 30, "slot count");
    void (*expected[30])() = {vt_skeleton_fn_0, vt_skeleton_fn_1, vt_skeleton_fn_2, vt_skeleton_fn_3, vt_skeleton_fn_4, vt_skeleton_fn_5, vt_skeleton_fn_6, vt_skeleton_fn_7, vt_skeleton_fn_8, vt_skeleton_fn_9, vt_skeleton_fn_10, vt_skeleton_fn_11, vt_skeleton_fn_12, vt_skeleton_fn_13, vt_skeleton_fn_14, vt_skeleton_fn_15, vt_skeleton_fn_16, vt_skeleton_fn_17, vt_skeleton_fn_18, vt_skeleton_fn_19, vt_skeleton_fn_20, vt_skeleton_fn_21, vt_skeleton_fn_22, vt_skeleton_fn_23, vt_skeleton_fn_24, vt_skeleton_fn_25, vt_skeleton_fn_26, vt_skeleton_fn_27, vt_skeleton_fn_28, nullptr};
    for (unsigned i = 0; i != 30; ++i)
        if (vt_skeleton[i].adjustment != 0 || vt_skeleton[i].function != expected[i]) { std::puts("FAIL"); return 1; }
    std::puts("PASS");
    return 0;
}
