// lightClear__Fv at 0x0013e6f0, 36 bytes, read from the pinned SLUS-20565 image.
// Observed: it stores zero in the word at 0x00483aac (the symbol table calls it
// interfaceMode, a 4-byte .sdata object) and then calls
// lightGlobalCharLightScale__Ff (0x0013eac0) with 1.0f. The store is in the
// call's delay slot, so it lands before the callee runs. Fresh compilation is
// not an original-byte match.
extern "C" {
extern int interfaceMode;
void lightGlobalCharLightScale__Ff(float);

void lightClear__Fv() {
    interfaceMode = 0;
    lightGlobalCharLightScale__Ff(1.0f);
}
}
