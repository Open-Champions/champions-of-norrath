extern "C" int activePlayer = 0;
extern "C" unsigned char player[95936] = {};
extern "C" int amxActivePlayerIsDwarf__FP5__amxPi(void*, int*);
static void store(unsigned char* address, unsigned value) {
    address[0] = static_cast<unsigned char>(value);
    address[1] = static_cast<unsigned char>(value >> 8);
    address[2] = static_cast<unsigned char>(value >> 16);
    address[3] = static_cast<unsigned char>(value >> 24);
}
int main() {
    activePlayer = 0;
    store(player + 84, 0u);
    if (amxActivePlayerIsDwarf__FP5__amxPi(0, 0) != 1) return 1;
    store(player + 84, 1u);
    if (amxActivePlayerIsDwarf__FP5__amxPi(0, 0) != 0) return 2;
    store(player + 84, 0xffffffffu);
    if (amxActivePlayerIsDwarf__FP5__amxPi(0, 0) != 0) return 3;
    activePlayer = 2;
    store(player + 2 * 23984 + 84, 0u);
    store(player + 84, 5u);
    if (amxActivePlayerIsDwarf__FP5__amxPi(0, 0) != 1) return 4;
    return 0;
}
