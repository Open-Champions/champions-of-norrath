// Observed amxHudResetFadeScreen at 0x0013a798, 32 bytes. It calls
// hudResetFadeScreen with the first AMX cell and returns zero.
extern "C" int hudResetFadeScreen__Fi(int value);

extern "C" int amxHudResetFadeScreen__FP5__amxPi(void*, int* params) {
    (void)hudResetFadeScreen__Fi(params[1]);
    return 0;
}
