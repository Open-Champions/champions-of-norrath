#include "checkpoint_age.cpp"

#include <cstdio>

int main() {
    if (sizeof MC_CheckPointAge != 4 || MC_CheckPointAge != 0) return 1;
    MC_CheckPointAge = 12;
    if (MC_CheckPointAge != 12) return 2;
    std::puts("checkpoint age passed");
    return 0;
}
