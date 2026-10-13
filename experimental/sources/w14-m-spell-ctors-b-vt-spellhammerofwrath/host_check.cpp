// Host check only. Not an EE build input.
//   c++ -std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter host_check.cpp
#include <cstdio>
#include "vtable.cpp"

extern "C" void vt_spellhammerofwrath_fn_0() {}
extern "C" void vt_spellhammerofwrath_fn_1() {}
extern "C" void vt_spellhammerofwrath_fn_2() {}
extern "C" void vt_spellhammerofwrath_fn_3() {}
extern "C" void vt_spellhammerofwrath_fn_4() {}
int main() {
    static_assert(sizeof(vt_spellhammerofwrath) / sizeof(VtSlot) == 6, "slot count");
    void (*expected[6])() = {vt_spellhammerofwrath_fn_0, vt_spellhammerofwrath_fn_1, vt_spellhammerofwrath_fn_2, vt_spellhammerofwrath_fn_3, vt_spellhammerofwrath_fn_4, nullptr};
    for (unsigned i = 0; i != 6; ++i)
        if (vt_spellhammerofwrath[i].adjustment != 0 || vt_spellhammerofwrath[i].function != expected[i]) { std::puts("FAIL"); return 1; }
    std::puts("PASS");
    return 0;
}
