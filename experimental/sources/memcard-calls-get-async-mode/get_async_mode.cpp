// Observed: MC_GetAsyncMode__Fv at 0x0025ad08 is 8 bytes. It returns the
// asyncMode word.
// Inferred: nothing.
// Unknown: which store publishes a mode other than the -1 written by MC_Init.

extern "C" int asyncMode;

extern "C" int MC_GetAsyncMode__Fv() {
    return asyncMode;
}
