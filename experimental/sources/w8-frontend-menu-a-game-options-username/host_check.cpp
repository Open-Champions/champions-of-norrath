#include <cstdio>
#include "username.cpp"
int main() {
    alignas(8) char block[128] = {};
    if (Username__C13VIGameOptions(block) != block + 16) return 1;
    if (Username__C13VIGameOptions(block + 5) != block + 21) return 2;
    block[16] = 'x';
    if (Username__C13VIGameOptions(block)[0] != 'x') return 3;
    std::puts("Username__C13VIGameOptions check passed");
    return 0;
}
