#include "is_prepared.cpp"

#include <cstdio>

int main() {
    if (VIMemCardIsPrepared__Fv() != 1) return 1;
    if (VIMemCardIsPrepared__Fv() != 1) return 2;
    std::puts("memcard is-prepared passed");
    return 0;
}
