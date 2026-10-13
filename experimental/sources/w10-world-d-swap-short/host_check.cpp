// Host contract for swap__H1Zs_RX01T0_v. Not an EE plan input.
#include <cstdio>
extern "C" void swap__H1Zs_RX01T0_v(short*, short*);
int main() {
    short a = 7, b = -300;
    swap__H1Zs_RX01T0_v(&a, &b);
    if (a != -300 || b != 7) return 1;
    swap__H1Zs_RX01T0_v(&a, &a);
    if (a != -300) return 2;
    std::printf("swap short contract passed\n");
    return 0;
}
