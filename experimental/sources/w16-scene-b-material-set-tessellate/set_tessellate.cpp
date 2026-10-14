// Observed: SetTessellate__10VIMateriali at 0x010cf7a8 is 36 bytes. It sets bit 5
// (mask 0x20) of the word at offset 12 of the material when the argument is
// non-zero and clears it otherwise, then returns 0. Unlike its neighbours it does
// not store the changed word at offset 32.
// Inferred: offset 12 holds material state flags. Unknown: what reads mask 0x20.
extern "C" {
int SetTessellate__10VIMateriali(unsigned char* material, int enable) {
    unsigned flags = *reinterpret_cast<unsigned*>(material + 12);
    if (enable)
        flags |= 0x20;
    else
        flags &= ~0x20u;
    *reinterpret_cast<unsigned*>(material + 12) = flags;
    return 0;
}
}
