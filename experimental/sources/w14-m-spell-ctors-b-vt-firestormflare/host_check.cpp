// Host check only. Not an EE build input.
//   c++ -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter host_check.cpp
#include <cstdio>
#include "vtable.cpp"

extern "C" void vt_firestormflare_fn_0() {}
extern "C" void vt_firestormflare_fn_1() {}
extern "C" void vt_firestormflare_fn_2() {}
extern "C" void vt_firestormflare_fn_3() {}
extern "C" void vt_firestormflare_fn_4() {}
extern "C" void vt_firestormflare_fn_5() {}
extern "C" void vt_firestormflare_fn_6() {}
extern "C" void vt_firestormflare_fn_7() {}
extern "C" void vt_firestormflare_fn_8() {}
extern "C" void vt_firestormflare_fn_9() {}
extern "C" void vt_firestormflare_fn_10() {}
extern "C" void vt_firestormflare_fn_11() {}
extern "C" void vt_firestormflare_fn_12() {}
extern "C" void vt_firestormflare_fn_13() {}
extern "C" void vt_firestormflare_fn_14() {}
extern "C" void vt_firestormflare_fn_15() {}
extern "C" void vt_firestormflare_fn_16() {}
int main() {
    static_assert(sizeof(vt_firestormflare) / sizeof(VtSlot) == 18, "slot count");
    void (*expected[18])() = {vt_firestormflare_fn_0, vt_firestormflare_fn_1, vt_firestormflare_fn_2, vt_firestormflare_fn_3, vt_firestormflare_fn_4, vt_firestormflare_fn_5, vt_firestormflare_fn_6, vt_firestormflare_fn_7, vt_firestormflare_fn_8, vt_firestormflare_fn_9, vt_firestormflare_fn_10, vt_firestormflare_fn_11, vt_firestormflare_fn_12, vt_firestormflare_fn_13, vt_firestormflare_fn_14, vt_firestormflare_fn_15, vt_firestormflare_fn_16, nullptr};
    for (unsigned i = 0; i != 18; ++i)
        if (vt_firestormflare[i].adjustment != 0 || vt_firestormflare[i].function != expected[i]) { std::puts("FAIL"); return 1; }
    std::puts("PASS");
    return 0;
}
