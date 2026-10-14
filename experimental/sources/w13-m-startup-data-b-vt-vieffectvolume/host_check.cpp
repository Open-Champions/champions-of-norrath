// Host check only. Not an EE build input.
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// (host_check.cpp includes the unit; do not also list the unit on the command line.)
// Every slot must hold a zero adjustment and the stub declared at that position;
// the table length and the null last slot are checked too. The stubs only record the slot
// order; they do not check the image symbol names, which the object-level check covers.
#include <cstdio>
#include "vt_vieffectvolume.cpp"

extern "C" void vt_vieffectvolume_fn_0() {}
extern "C" void vt_vieffectvolume_fn_1() {}
extern "C" void vt_vieffectvolume_fn_2() {}
extern "C" void vt_vieffectvolume_fn_3() {}
extern "C" void vt_vieffectvolume_fn_4() {}
extern "C" void vt_vieffectvolume_fn_5() {}
extern "C" void vt_vieffectvolume_fn_6() {}
extern "C" void vt_vieffectvolume_fn_7() {}
extern "C" void vt_vieffectvolume_fn_8() {}
extern "C" void vt_vieffectvolume_fn_9() {}
extern "C" void vt_vieffectvolume_fn_10() {}

int main() {
    static_assert(sizeof(vt_vieffectvolume) / sizeof(VtSlot) == 12, "slot count");
    void (*expected[12])() = {vt_vieffectvolume_fn_0, vt_vieffectvolume_fn_1, vt_vieffectvolume_fn_2, vt_vieffectvolume_fn_3, vt_vieffectvolume_fn_4, vt_vieffectvolume_fn_5, vt_vieffectvolume_fn_6, vt_vieffectvolume_fn_7, vt_vieffectvolume_fn_8, vt_vieffectvolume_fn_9, vt_vieffectvolume_fn_10, nullptr};
    for (unsigned i = 0; i != 12; ++i)
        if (vt_vieffectvolume[i].adjustment != 0 || vt_vieffectvolume[i].function != expected[i]) { std::puts("FAIL"); return 1; }
    std::puts("PASS");
    return 0;
}
