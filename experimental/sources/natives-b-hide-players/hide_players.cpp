// Observed amxHidePlayers at 0x0013c2e0, 16 bytes. It stores the low
// byte of the first AMX cell into gameHidePlayers at 0x00484f28.
extern "C" unsigned char gameHidePlayers;

extern "C" int amxHidePlayers__FP5__amxPi(void*, int* params) {
    gameHidePlayers = static_cast<unsigned char>(params[1]);
    return 0;
}
