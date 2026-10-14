// amxGetRand is the 40-byte native at 0x0013bca8. It advances syncRandState
// at 0x00483e5c by state * 0x19660d + 0x3c6ef35f and returns the high half.
// The multiply and add are 32-bit. The arguments are not read.
extern "C" unsigned syncRandState;
extern "C" int amxGetRand__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    (void)params;
    unsigned state = syncRandState * 0x19660du + 0x3c6ef35fu;
    syncRandState = state;
    return static_cast<int>(state >> 16);
}
