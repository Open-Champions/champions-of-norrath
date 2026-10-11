// amxExitToMainMenu is the 16-byte native at 0x0013c0f8. It stores 1 into
// gameBailFromGameLoop at 0x00483f28 and returns zero.
extern "C" int gameBailFromGameLoop;
extern "C" int amxExitToMainMenu__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    gameBailFromGameLoop = 1;
    return 0;
}
