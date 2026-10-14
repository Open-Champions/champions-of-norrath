// Observed amxStartWithSwoopDone__FP5__amxPi at 0x0013b330, 16 bytes.
// It stores 1 in DLG_StartWithSwoopDone and returns 0.
extern "C" int DLG_StartWithSwoopDone;

extern "C" int amxStartWithSwoopDone__FP5__amxPi(void*, int*) {
    DLG_StartWithSwoopDone = 1;
    return 0;
}
