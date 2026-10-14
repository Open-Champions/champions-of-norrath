// Observed texIsLoading__Fv at 0x001729a0, 36 bytes.
// Returns 1 when the byte at 0x004840a1 (texturesAreDecompressing) or the
// byte at 0x004840a0 (texturesAreLoading) is nonzero, otherwise 0. The first
// byte is tested first. Symbol names come from the executable's symbol table;
// neither byte is defined in this package.
extern "C" {
extern signed char texturesAreDecompressing;
extern signed char texturesAreLoading;
int texIsLoading__Fv() {
    if (texturesAreDecompressing != 0) return 1;
    return texturesAreLoading != 0 ? 1 : 0;
}
}
