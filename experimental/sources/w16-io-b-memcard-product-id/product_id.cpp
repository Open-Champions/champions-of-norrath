// Observed: VIMemCardProductId__Fv at 0x010d3f10 is 12 bytes. It returns the
// address of the local 32-byte buffer sProductId (0x011afa78) and does not
// read it.
// Inferred: callers use the returned text as the card directory name.
// Unknown: whether any caller writes through the returned pointer.
extern "C" char sProductId[32];

extern "C" char* VIMemCardProductId__Fv() {
    return sProductId;
}
