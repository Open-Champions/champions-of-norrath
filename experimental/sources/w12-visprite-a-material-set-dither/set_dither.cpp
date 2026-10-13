// Observed: SetDither__10VIMateriali at 0x010cf780 is 36 bytes. It sets bit 4
// (mask 0x10) of the word at offset 12 of the object when the argument is
// non-zero and clears it otherwise, and returns 0. Unlike the neighbouring
// SetZTest and SetZWrite it does not store the word at offset 32.
// Inferred: offset 12 holds material state flags. Unknown: why this setter
// leaves the changed flag alone.
extern "C" {
int SetDither__10VIMateriali(void* object, int enable) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    unsigned flags = *reinterpret_cast<unsigned*>(bytes + 12);
    if (enable)
        flags |= 0x10;
    else
        flags &= ~0x10u;
    *reinterpret_cast<unsigned*>(bytes + 12) = flags;
    return 0;
}
}
