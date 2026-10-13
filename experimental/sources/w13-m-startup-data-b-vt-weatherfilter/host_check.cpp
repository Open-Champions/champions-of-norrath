// Host check only. Not an EE build input.
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp
// (host_check.cpp includes the unit; do not also list the unit on the command line.)
// Every slot must hold a zero adjustment and the stub declared at that position;
// the table length and the null last slot are checked too. The stubs only record the slot
// order; they do not check the image symbol names, which the object-level check covers.
#include <cstdio>
#include "vt_weatherfilter.cpp"

extern "C" void vt_weatherfilter_fn_0() {}
extern "C" void vt_weatherfilter_fn_1() {}

int main() {
    static_assert(sizeof(vt_weatherfilter) / sizeof(VtSlot) == 3, "slot count");
    void (*expected[3])() = {vt_weatherfilter_fn_0, vt_weatherfilter_fn_1, nullptr};
    for (unsigned i = 0; i != 3; ++i)
        if (vt_weatherfilter[i].adjustment != 0 || vt_weatherfilter[i].function != expected[i]) { std::puts("FAIL"); return 1; }
    std::puts("PASS");
    return 0;
}
