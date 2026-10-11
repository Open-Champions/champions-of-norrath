// Observed amxLevelLoading at 0x0013bee0, 8 bytes. It returns the
// word gameLevelLoading at 0x00484cd4 and does not read the AMX cells.
extern "C" int gameLevelLoading;

extern "C" int amxLevelLoading__FP5__amxPi(void*, int*) {
    return gameLevelLoading;
}
