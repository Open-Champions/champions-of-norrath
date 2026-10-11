// amxCheckPlayerAlive is the 20-byte native at 0x0013c2f0. It returns
// gameCheckPlayerAlive and does not read the AMX arguments.
extern "C" int gameCheckPlayerAlive__Fv();
extern "C" int amxCheckPlayerAlive__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    return gameCheckPlayerAlive__Fv();
}
