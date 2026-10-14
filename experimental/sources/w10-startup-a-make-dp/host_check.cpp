// Host comparison only. Not an EE build input. A scripted __pack_d records what it was given.
#include <cstdio>
#include <cstddef>
struct W10UnpackedDouble;
#include "make_dp.cpp"
static W10UnpackedDouble seen;
static int calls;
extern "C" unsigned long long __pack_d(const W10UnpackedDouble* value) {
    ++calls;
    seen = *value;
    return 0xfeedfacecafebeefull;
}
int main() {
    int failures = 0;
    static_assert(offsetof(W10UnpackedDouble, fraction) == 16, "layout");
    unsigned long long r = __make_dp(3, 1, -77, 0x1234567890abcdefull);
    if (calls != 1 || r != 0xfeedfacecafebeefull) { std::printf("result must pass through once\n"); ++failures; }
    if (seen.cls != 3 || seen.sign != 1 || seen.power != -77 || seen.fraction != 0x1234567890abcdefull) {
        std::printf("fields wrong\n"); ++failures;
    }
    __make_dp(4, 0, 1023, 5);
    if (calls != 2 || seen.cls != 4 || seen.sign != 0 || seen.power != 1023 || seen.fraction != 5) { std::printf("second call\n"); ++failures; }
    if (failures) return 2;
    std::printf("make_dp cases passed\n");
    return 0;
}
