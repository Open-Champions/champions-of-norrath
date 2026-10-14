// Observed texIsDecompressing__Fv at 0x00172998, 8 bytes.
// Returns the sign-extended byte at 0x004840a1 (texturesAreDecompressing).
// The byte's symbol name comes from the executable's symbol table; it is not
// defined in this package.
extern "C" {
extern signed char texturesAreDecompressing;
int texIsDecompressing__Fv() {
    return texturesAreDecompressing;
}
}
