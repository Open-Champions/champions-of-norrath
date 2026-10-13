// Host comparison only. Not an EE build input. A scripted __pack_f records what it was given.
#include <cstdio>
#include <cstddef>
struct W10UnpackedSingle;
#include "make_fp.cpp"
static W10UnpackedSingle seen;
static int calls;
extern "C" float __pack_f(const W10UnpackedSingle* value) {
    ++calls;
    seen = *value;
    return 2.75f;
}
int main() {
    int failures = 0;
    static_assert(sizeof(W10UnpackedSingle) == 16 && offsetof(W10UnpackedSingle, fraction) == 12, "layout");
    float r = __make_fp(3, 1, -9, 0x40123456u);
    if (calls != 1 || r != 2.75f) { std::printf("result must pass through once\n"); ++failures; }
    if (seen.cls != 3 || seen.sign != 1 || seen.power != -9 || seen.fraction != 0x40123456u) { std::printf("fields wrong\n"); ++failures; }
    __make_fp(2, 0, 0, 0);
    if (calls != 2 || seen.cls != 2 || seen.sign != 0 || seen.power != 0 || seen.fraction != 0) { std::printf("second call\n"); ++failures; }
    if (failures) return 2;
    std::printf("make_fp cases passed\n");
    return 0;
}
