#include "button_values.cpp"

#include <cstdio>

int main() {
    const unsigned want[14] = {0x20, 0x10, 0x80, 0x40, 0, 0, 0, 0, 4, 1, 8, 2, 0x400, 0};
    if (sizeof buttonValues != 56) return 1;
    for (int i = 0; i < 14; ++i) if (buttonValues[i] != want[i]) return 2;
    std::puts("button values passed");
    return 0;
}
