// Observed inventoryUnload__Fv at 0x001b1698, 132 bytes.
// inventoryLoadState (0x00484824) is a word. If it is neither 0 nor 2 the
// function first finishes the load: it repeats cdWait__Fv then
// inventoryLoad__Fv until the state equals 2. (The test is "not equal to 2", so
// a state that never reaches 2 loops forever, as in the original.) Then it calls
// textureReleaseCacheBuffer__Fv; if inventoryLump (0x00484820) is nonzero it
// passes it to lumpUnload__FPv and clears it; invGrid (0x00484544) is cleared on
// both paths. It then clears the word shopGrid (0x0048491c), clears
// inventoryLoadState, and calls coreStartLoading__Fv (the state store is in the
// call's delay slot, so it happens before the call). What the states mean beyond
// 0 and 2 is not established. None of the objects or callees is defined here.
extern "C" {
extern int inventoryLoadState;
extern void* inventoryLump;
extern void* invGrid;
extern void* shopGrid;
void cdWait__Fv();
void inventoryLoad__Fv();
void textureReleaseCacheBuffer__Fv();
void lumpUnload__FPv(void*);
void coreStartLoading__Fv();
}

extern "C" void inventoryUnload__Fv() {
    if (inventoryLoadState != 0 && inventoryLoadState != 2) {
        do {
            cdWait__Fv();
            inventoryLoad__Fv();
        } while (inventoryLoadState != 2);
    }
    textureReleaseCacheBuffer__Fv();
    if (inventoryLump != nullptr) {
        lumpUnload__FPv(inventoryLump);
        inventoryLump = nullptr;
    }
    invGrid = nullptr;
    shopGrid = nullptr;
    inventoryLoadState = 0;
    coreStartLoading__Fv();
}
