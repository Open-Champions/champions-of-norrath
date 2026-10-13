#include "intersects.cpp"

int main() {
    int a[4] = {10, 20, 30, 40};
    struct Case { int b[4]; int want; } cases[] = {
        {{10, 20, 30, 40}, 1}, {{0, 0, 9, 100}, 0}, {{0, 0, 10, 20}, 1}, {{31, 0, 40, 100}, 0},
        {{30, 40, 50, 60}, 1}, {{0, 0, 100, 19}, 0}, {{0, 41, 100, 50}, 0}, {{15, 25, 16, 26}, 1},
        {{-100, -100, -50, -50}, 0}, {{0, 0, 100, 100}, 1}, {{30, 0, 31, 20}, 1}, {{30, 0, 31, 19}, 0},
    };
    for (unsigned i = 0; i < sizeof cases / sizeof cases[0]; ++i)
        if (Intersects__C6VIRectRC6VIRect(a, cases[i].b) != cases[i].want) return 1 + i;
    // asymmetric check: swapping the operands uses the opposite fields
    int c[4] = {0, 0, 5, 5}, d[4] = {6, 0, 9, 5};
    if (Intersects__C6VIRectRC6VIRect(c, d) != 0 || Intersects__C6VIRectRC6VIRect(d, c) != 0) return 40;
    return 0;
}
