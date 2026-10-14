// Host contract for _vt$9bad_alloc; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_9bad_alloc[4] asm("_vt$9bad_alloc");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf9bad_alloc");
extern "C" void vt_fn_0() { hit = "__tf9bad_alloc"; }
extern "C" void vt_fn_1() asm("_$_9bad_alloc");
extern "C" void vt_fn_1() { hit = "_$_9bad_alloc"; }
extern "C" void vt_fn_2() asm("what__C9bad_alloc");
extern "C" void vt_fn_2() { hit = "what__C9bad_alloc"; }
int main() {
    const char* want[] = {
        "__tf9bad_alloc",
        "_$_9bad_alloc",
        "what__C9bad_alloc",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_9bad_alloc / sizeof vt_9bad_alloc[0] != 4) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 4; ++i) {
        if (vt_9bad_alloc[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_9bad_alloc[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_9bad_alloc[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$9bad_alloc slots ok\n");
    return bad != 0;
}
