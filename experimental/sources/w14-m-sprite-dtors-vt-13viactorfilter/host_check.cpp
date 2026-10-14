// Host contract for _vt$13VIActorFilter; not an EE input. Checks the slot layout and the null last slot only.
#include <cstdio>
#include <cstddef>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
static void f0() {}
static VtSlot vt_13VIActorFilter[3] = {
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, nullptr},
};
int main() {
    int failures = 0;
    if (sizeof vt_13VIActorFilter / sizeof vt_13VIActorFilter[0] != 3) { std::fprintf(stderr, "fail: count\n"); ++failures; }
    for (int i = 0; i < 2; ++i) if (vt_13VIActorFilter[i].adjustment != 0u || vt_13VIActorFilter[i].function == nullptr) { std::fprintf(stderr, "fail: slot %d\n", i); ++failures; }
    if (vt_13VIActorFilter[2].function != nullptr || vt_13VIActorFilter[2].adjustment != 0u) { std::fprintf(stderr, "fail: last\n"); ++failures; }
    if (!failures) std::puts("w14-m-sprite-dtors-vt-13viactorfilter ok");
    return failures ? 1 : 0;
}
