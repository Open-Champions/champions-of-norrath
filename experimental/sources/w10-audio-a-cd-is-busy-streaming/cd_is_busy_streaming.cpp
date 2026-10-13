// Observed: cdIsBusyStreamingNonwithstanding__Fv at 0x00166d48 is 8 bytes. It returns the
// signed byte cdBusy (0x00483ffc), the same flag cdLoadAsync__FPcPvT0ii sets. It does not read
// the streaming flag despite the symbol name.
// Inferred: nothing.
// Unknown: why the symbol name mentions streaming.

extern "C" {
extern signed char cdBusy;
}

extern "C" int cdIsBusyStreamingNonwithstanding__Fv() {
    return cdBusy;
}
