#include <cstdio>
#include "password.cpp"
int main() {
    alignas(8) char block[192] = {};
    if (Password__C13VIGameOptions(block) != block + 80) return 1;
    if (Password__C13VIGameOptions(block + 3) != block + 83) return 2;
    block[80] = 'p';
    if (Password__C13VIGameOptions(block)[0] != 'p') return 3;
    std::puts("Password__C13VIGameOptions check passed");
    return 0;
}
