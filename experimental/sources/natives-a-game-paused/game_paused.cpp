// amxGamePaused is the 8-byte native at 0x0013be28. It returns the word
// gamePause at 0x00483f14 and does not read the AMX arguments.
extern "C" int gamePause;
extern "C" int amxGamePaused__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    return gamePause;
}
