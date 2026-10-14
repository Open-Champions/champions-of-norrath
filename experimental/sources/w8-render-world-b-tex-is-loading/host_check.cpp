extern "C" {
signed char texturesAreDecompressing;
signed char texturesAreLoading;
int texIsLoading__Fv();
}
int main() {
    texturesAreDecompressing = 0; texturesAreLoading = 0;
    if (texIsLoading__Fv() != 0) return 1;
    texturesAreDecompressing = 0; texturesAreLoading = 1;
    if (texIsLoading__Fv() != 1) return 2;
    texturesAreDecompressing = 1; texturesAreLoading = 0;
    if (texIsLoading__Fv() != 1) return 3;
    texturesAreDecompressing = -5; texturesAreLoading = 7;
    if (texIsLoading__Fv() != 1) return 4;
    return 0;
}
