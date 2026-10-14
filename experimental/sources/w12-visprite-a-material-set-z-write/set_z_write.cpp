// Observed: SetZWrite__10VIMateriali at 0x010cf720 is 44 bytes. It sets bit 1
// (mask 0x2) of the word at offset 12 of the object when the argument is
// non-zero and clears it otherwise, stores 1 in the word at offset 32 and
// returns 0.
// Inferred: offset 12 holds material state flags and offset 32 is a changed
// flag (SetLayerBlendMode writes the same word). Unknown: the flag names'
// meaning beyond the draw path that reads them.
extern "C" {
int SetZWrite__10VIMateriali(void* object, int enable) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned flags = *reinterpret_cast<unsigned*>(bytes + 12);
    if (enable)
        flags |= 0x2;
    else
        flags &= ~0x2u;
    *reinterpret_cast<unsigned*>(bytes + 12) = flags;
    *reinterpret_cast<int*>(bytes + 32) = 1;
    return 0;
}
}
