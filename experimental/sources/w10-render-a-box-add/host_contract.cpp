// Host contract for boxAdd__FP3BoxT0. Not an EE plan input.
#include "box_add.cpp"
#include "../level-anim-load-common/host_check.hpp"
static int check(const char* what, const float* a, const float* b, const float* want) {
    unsigned char box[24], other[24], keep[24];
    for (int i = 0; i != 6; ++i) { sf_(box + 4 * i, a[i]); sf_(other + 4 * i, b[i]); }
    std::memcpy(keep, other, 24);
    boxAdd__FP3BoxT0(box, other);
    for (int i = 0; i != 6; ++i) {
        unsigned char w[4]; sf_(w, want[i]);
        if (want[i] == want[i] && lf_(box + 4 * i) != want[i]) { std::printf("%s: word %d got %g want %g\n", what, i, lf_(box + 4 * i), want[i]); return 1; }
    }
    return same(what, other, keep, 24);
}
int main() {
    const float inf = 1.0e7f;
    const float empty[6] = {inf, inf, inf, -inf, -inf, -inf};
    const float p[6] = {1, 2, 3, 4, 5, 6};
    const float q[6] = {0, 5, 3, 3, 9, 6};
    const float pq[6] = {0, 2, 3, 4, 9, 6};
    const float nan = __builtin_nanf("");
    const float withnan[6] = {nan, nan, nan, nan, nan, nan};
    if (check("grow empty", empty, p, p)) return 1;
    if (check("mixed", p, q, pq)) return 2;
    if (check("equal keeps", p, p, p)) return 3;
    if (check("nan other", p, withnan, p)) return 4;
    std::puts("boxAdd contract passed");
    return 0;
}
