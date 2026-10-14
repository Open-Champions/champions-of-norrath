// Host comparison only. Not an EE build input. __unpack_d tags each record with
// the value it was given; __fpcmp_parts_d checks which record is which.
#include <cstdio>
extern "C" {
struct W9UnpackedDouble;
}
#include "dpcmp.cpp"
static int calls;
extern "C" void __unpack_d(const unsigned long long* v, W9UnpackedDouble* u) {
    ++calls; u->cls = static_cast<int>(*v); u->fraction = *v;
}
static const W9UnpackedDouble* seen_a; static const W9UnpackedDouble* seen_b;
extern "C" int __fpcmp_parts_d(const W9UnpackedDouble* a, const W9UnpackedDouble* b) {
    seen_a = a; seen_b = b;
    return a->cls < b->cls ? -1 : a->cls > b->cls ? 1 : 0;
}
int main() {
    int failures = 0;
    if (dpcmp(3ull, 9ull) != -1 || calls != 2) { std::printf("order wrong\n"); ++failures; }
    if (seen_a == seen_b) { std::printf("records must be distinct\n"); ++failures; }
    if (dpcmp(9ull, 3ull) != 1 || dpcmp(5ull, 5ull) != 0) { std::printf("result must pass through\n"); ++failures; }
    if (seen_a->fraction != 5ull || seen_b->fraction != 5ull) { std::printf("record contents\n"); ++failures; }
    if (failures) return 2;
    std::printf("dpcmp cases passed\n");
    return 0;
}
