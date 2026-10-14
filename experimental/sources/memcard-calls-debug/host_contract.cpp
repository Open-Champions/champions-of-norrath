#include "debug.cpp"

#include <cstdio>

int main() {
    if (sizeof MC_Debug != 1 || MC_Debug != 0) return 1;
    MC_Debug = 1;
    if (MC_Debug != 1) return 2;
    std::puts("mc debug passed");
    return 0;
}
