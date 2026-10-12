// Observed amxHudFadeScreen at 0x0013a7b8, 24 bytes. It tail-calls
// hudFadeScreen with the first AMX cell and returns that result.
extern "C" int hudFadeScreen__Fi(int value);

extern "C" int amxHudFadeScreen__FP5__amxPi(void*, int* params) {
    return hudFadeScreen__Fi(params[1]);
}
