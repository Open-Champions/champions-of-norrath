// Observed amxPlayersAlive at 0x0013a6a8, 8 bytes. It returns the
// word nmPlayersAlive at 0x00484f44.
extern "C" int nmPlayersAlive;

extern "C" int amxPlayersAlive__FP5__amxPi(void*, int*) {
    return nmPlayersAlive;
}
