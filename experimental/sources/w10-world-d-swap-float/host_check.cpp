// Host contract for swap__H1Zf_RX01T0_v. Not an EE plan input.
#include <cstdio>
extern "C" void swap__H1Zf_RX01T0_v(float*, float*);
int main() {
    float a = 1.5f, b = -2.25f;
    swap__H1Zf_RX01T0_v(&a, &b);
    if (a != -2.25f || b != 1.5f) return 1;
    swap__H1Zf_RX01T0_v(&a, &a);
    if (a != -2.25f) return 2;
    std::printf("swap float contract passed\n");
    return 0;
}
