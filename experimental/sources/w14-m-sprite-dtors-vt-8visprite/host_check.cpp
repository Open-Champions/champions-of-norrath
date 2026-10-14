// Host contract for _vt$8VISprite; not an EE input. Checks the slot layout and the null last slot only.
#include <cstdio>
#include <cstddef>
struct VtSlot {
    unsigned adjustment;
    void (*function)();
};
static void f0() {}
static VtSlot vt_8VISprite[12] = {
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, f0},
    {0x0u, nullptr},
};
int main() {
    int failures = 0;
    if (sizeof vt_8VISprite / sizeof vt_8VISprite[0] != 12) { std::fprintf(stderr, "fail: count\n"); ++failures; }
    for (int i = 0; i < 11; ++i) if (vt_8VISprite[i].adjustment != 0u || vt_8VISprite[i].function == nullptr) { std::fprintf(stderr, "fail: slot %d\n", i); ++failures; }
    if (vt_8VISprite[11].function != nullptr || vt_8VISprite[11].adjustment != 0u) { std::fprintf(stderr, "fail: last\n"); ++failures; }
    if (!failures) std::puts("w14-m-sprite-dtors-vt-8visprite ok");
    return failures ? 1 : 0;
}
