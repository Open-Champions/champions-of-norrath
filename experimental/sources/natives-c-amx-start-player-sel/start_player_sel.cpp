// Observed amxStartPlayerSel__FP5__amxPi at 0x0013aa88, 32 bytes. It calls
// playerSelectionStart and returns 0. The AMX arguments are not read.
extern "C" void playerSelectionStart__Fv();

extern "C" int amxStartPlayerSel__FP5__amxPi(void*, int*) {
    playerSelectionStart__Fv();
    return 0;
}
