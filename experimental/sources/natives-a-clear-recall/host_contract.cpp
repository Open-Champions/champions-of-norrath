extern "C" int nmPlayers = 0;
extern "C" unsigned char player[95936] = {};
extern "C" int amxClearRecall__FP5__amxPi(void*, int*);
int main() {
    nmPlayers = 0;
    player[36] = 9;
    if (amxClearRecall__FP5__amxPi(0, 0) != 0 || player[36] != 9) return 1;
    nmPlayers = 2;
    player[36] = 1;
    player[23984 + 36] = 1;
    player[41] = 7;
    if (amxClearRecall__FP5__amxPi(0, 0) != 0) return 2;
    const unsigned char expect[5] = {'N', 'o', 'n', 'e', 0};
    for (int index = 0; index < 5; ++index) {
        if (player[36 + index] != expect[index]) return 3;
        if (player[23984 + 36 + index] != expect[index]) return 4;
    }
    if (player[41] != 7) return 5;
    return 0;
}
