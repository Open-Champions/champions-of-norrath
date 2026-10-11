// amxChangeSwoop is the 16-byte native at 0x0013b320. It stores cell 1 into
// DLG_SwoopMode at 0x00485070 and returns zero.
extern "C" int DLG_SwoopMode;
extern "C" int amxChangeSwoop__FP5__amxPi(void* amx, int* params) {
    (void)amx;
    DLG_SwoopMode = params[1];
    return 0;
}
