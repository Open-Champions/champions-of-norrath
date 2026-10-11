// Observed amxSaveDone__FP5__amxPi at 0x0013c0d0, 36 bytes. It returns 1
// when MC_GetAsyncMode is -1 and 0 for every other result.
extern "C" int MC_GetAsyncMode__Fv();

extern "C" int amxSaveDone__FP5__amxPi(void*, int*) {
    return MC_GetAsyncMode__Fv() == -1 ? 1 : 0;
}
