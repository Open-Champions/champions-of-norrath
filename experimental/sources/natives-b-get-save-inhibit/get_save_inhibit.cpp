// Observed amxGetSaveInhibit at 0x0013c4d8, 8 bytes. It returns the
// word gameRecallInhibit at 0x00486694 and does not read the AMX cells.
extern "C" int gameRecallInhibit;

extern "C" int amxGetSaveInhibit__FP5__amxPi(void*, int*) {
    return gameRecallInhibit;
}
