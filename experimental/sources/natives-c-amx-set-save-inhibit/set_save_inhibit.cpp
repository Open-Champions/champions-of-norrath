// Observed amxSetSaveInhibit__FP5__amxPi at 0x0013c4c8, 16 bytes. Cell 1
// is stored in gameRecallInhibit. The native returns 0.
extern "C" int gameRecallInhibit;

extern "C" int amxSetSaveInhibit__FP5__amxPi(void*, int* params) {
    gameRecallInhibit = params[1];
    return 0;
}
