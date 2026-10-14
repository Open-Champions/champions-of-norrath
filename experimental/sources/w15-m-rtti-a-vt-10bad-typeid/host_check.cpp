// Host contract for _vt$10bad_typeid; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_10bad_typeid[4] asm("_vt$10bad_typeid");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf10bad_typeid");
extern "C" void vt_fn_0() { hit = "__tf10bad_typeid"; }
extern "C" void vt_fn_1() asm("_$_10bad_typeid");
extern "C" void vt_fn_1() { hit = "_$_10bad_typeid"; }
extern "C" void vt_fn_2() asm("what__C9exception");
extern "C" void vt_fn_2() { hit = "what__C9exception"; }
int main() {
    const char* want[] = {
        "__tf10bad_typeid",
        "_$_10bad_typeid",
        "what__C9exception",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_10bad_typeid / sizeof vt_10bad_typeid[0] != 4) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 4; ++i) {
        if (vt_10bad_typeid[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_10bad_typeid[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_10bad_typeid[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$10bad_typeid slots ok\n");
    return bad != 0;
}
