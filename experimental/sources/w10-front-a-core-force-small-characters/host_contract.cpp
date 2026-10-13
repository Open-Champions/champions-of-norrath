#include "core_force_small_characters.cpp"

#include <cstdio>

int main() {
    if (sizeof coreForceSmallCharacters != 1 || coreForceSmallCharacters != 0) return 1;
    std::puts("core force small characters passed");
    return 0;
}
