// Host contract for _vt$13VISceneFilter; not an EE input. Checks the slot layout and the null last slot only.
#include <cstdio>
#include <cstddef>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
static void f0() {}
static VtSlot vt_13VISceneFilter[3] = {
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, nullptr},
};
int main() {
    int failures = 0;
    if (sizeof vt_13VISceneFilter / sizeof vt_13VISceneFilter[0] != 3) { std::fprintf(stderr, "fail: count\n"); ++failures; }
    for (int i = 0; i < 2; ++i) if (vt_13VISceneFilter[i].adjustment != 0u || vt_13VISceneFilter[i].function == nullptr) { std::fprintf(stderr, "fail: slot %d\n", i); ++failures; }
    if (vt_13VISceneFilter[2].function != nullptr || vt_13VISceneFilter[2].adjustment != 0u) { std::fprintf(stderr, "fail: last\n"); ++failures; }
    if (!failures) std::puts("w14-m-sprite-dtors-vt-13viscenefilter ok");
    return failures ? 1 : 0;
}
