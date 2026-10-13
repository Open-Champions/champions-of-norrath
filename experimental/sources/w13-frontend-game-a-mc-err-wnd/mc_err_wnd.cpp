// Observed McErrWnd__Fv at 0x0109fb58, 12 bytes: returns the address of the global window object
// sWndMcErr (0x01190a20, 0x210 bytes). It reads no memory and has no side effects.
// Inferred: the result is a window pointer (callers compare it with other window pointers).
extern "C" unsigned char sWndMcErr[0x210];

extern "C" void* McErrWnd__Fv() {
    return sWndMcErr;
}
