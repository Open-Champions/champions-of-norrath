#include "ctor.cpp"

int main() {
    alignas(8) unsigned words[4] = {0xdeadbeef, 0x11111111, 0x22222222, 0x33333333};
    __13VIBattleMusic(words);
    if (words[0] != 0) return 1;
    if (words[1] != 0x11111111 || words[2] != 0x22222222 || words[3] != 0x33333333) return 2;
    return 0;
}
