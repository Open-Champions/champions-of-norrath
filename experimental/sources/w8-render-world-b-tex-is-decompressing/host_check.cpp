extern "C" {
signed char texturesAreDecompressing;
int texIsDecompressing__Fv();
}
int main() {
    texturesAreDecompressing = 0;
    if (texIsDecompressing__Fv() != 0) return 1;
    texturesAreDecompressing = 1;
    if (texIsDecompressing__Fv() != 1) return 2;
    // The byte is sign extended, not treated as a boolean.
    texturesAreDecompressing = -2;
    if (texIsDecompressing__Fv() != -2) return 3;
    return 0;
}
