// Host contract for _vt$19VIFlatFileDirectory; not an EE input. Build with the unit beside it:
//   c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp vt.cpp
// Each slot must hold a zero adjustment and call the expected observed name; the last slot is null.
#include <cstdio>
#include <cstring>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
extern "C" VtSlot vt_19VIFlatFileDirectory[3] asm("_vt$19VIFlatFileDirectory");
static const char* hit = "";
extern "C" void vt_fn_0() asm("__tf19VIFlatFileDirectory");
extern "C" void vt_fn_0() { hit = "__tf19VIFlatFileDirectory"; }
extern "C" void vt_fn_1() asm("_$_19VIFlatFileDirectory");
extern "C" void vt_fn_1() { hit = "_$_19VIFlatFileDirectory"; }
int main() {
    const char* want[] = {
        "__tf19VIFlatFileDirectory",
        "_$_19VIFlatFileDirectory",
        nullptr,
    };
    int bad = 0;
    if (sizeof vt_19VIFlatFileDirectory / sizeof vt_19VIFlatFileDirectory[0] != 3) { std::printf("count\n"); ++bad; }
    for (int i = 0; i < 3; ++i) {
        if (vt_19VIFlatFileDirectory[i].adjustment != 0u) { std::printf("adjust %d\n", i); ++bad; }
        if (want[i] == nullptr) { if (vt_19VIFlatFileDirectory[i].function != nullptr) { std::printf("last %d\n", i); ++bad; } continue; }
        hit = "";
        vt_19VIFlatFileDirectory[i].function();
        if (std::strcmp(hit, want[i]) != 0) { std::printf("slot %d\n", i); ++bad; }
    }
    std::printf(bad ? "failed\n" : "_vt$19VIFlatFileDirectory slots ok\n");
    return bad != 0;
}
