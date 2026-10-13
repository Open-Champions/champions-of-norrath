// Observed fastestRotDirection__FssPi at 0x0015a5c0, 76 bytes. Takes two 16-bit angles
// a and b (sign-extended) and an optional output pointer. d = (short)(a - b). With a
// non-null pointer, *out = |d| (the 16-bit difference, negated when negative). The
// function returns -1 when d > 0 and +1 otherwise (including d == 0), with or
// without the output pointer (null stores nothing). Angle units are not
// established here. Nothing here was executed on the EE.
extern "C" int fastestRotDirection__FssPi(short a, short b, int* out) {
    short d = static_cast<short>(a - b);
    if (out) {
        int m = d;
        if (m < 0) m = -m;
        *out = m;
    }
    return d > 0 ? -1 : 1;
}
