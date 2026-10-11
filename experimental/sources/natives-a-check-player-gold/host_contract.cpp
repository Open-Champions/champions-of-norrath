extern "C" int nmPlayers = 0;
extern "C" unsigned char player[95936] = {};
extern "C" int amxCheckPlayerGold__FP5__amxPi(void*, int*);
static void store(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    nmPlayers = 0;
    if (amxCheckPlayerGold__FP5__amxPi(0, 0) != 0) return 1;
    nmPlayers = -2;
    store(player + 232, 9u);
    if (amxCheckPlayerGold__FP5__amxPi(0, 0) != 0) return 2;
    nmPlayers = 2;
    store(player + 232, 10u);
    store(player + 23984 + 232, 25u);
    if (amxCheckPlayerGold__FP5__amxPi(0, 0) != 35) return 3;
    store(player + 232, 0xffffffffu);
    store(player + 23984 + 232, 2u);
    if (amxCheckPlayerGold__FP5__amxPi(0, 0) != 1) return 4;
    return 0;
}
