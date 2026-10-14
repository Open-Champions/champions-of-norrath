// Observed shopClipGridCoordinates__FPiN30 at 0x001bc270, 100 bytes. The mangled signature is four
// int pointers. Only the second and fourth are used: each pointed-to int is first raised to 105 when
// it is below 105, and then lowered to 600 when it is above 600. The checks run in the order second
// (low), fourth (low), second (high), fourth (high), re-reading each value from memory, so the
// result is the same as clamping both into 105..600, also when both pointers are the same. The first
// and third pointers are not read or written and nothing is returned.
// Inferred: a screen-space clip of two coordinates used by the shop grid code. Unknown: what the
// untouched first and third parameters are, and the unit of 105 and 600.
extern "C" void shopClipGridCoordinates__FPiN30(int* first, int* second, int* third, int* fourth) {
    (void)first;
    (void)third;
    if (*second < 105) *second = 105;
    if (*fourth < 105) *fourth = 105;
    if (*second > 600) *second = 600;
    if (*fourth > 600) *fourth = 600;
}
