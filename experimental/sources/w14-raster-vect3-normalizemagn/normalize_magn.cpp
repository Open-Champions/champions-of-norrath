// Observed: NormalizeMagn__7VIVect3 at 0x0104f360 is 112 bytes in the pinned SLUS-20565 image. The argument points at three
// floats x, y, z. The squared length is (x*x + y*y) + z*z. If it equals 0.0f the function returns that zero and writes nothing.
// Otherwise it takes the square root with sqrt.s, divides 1.0f by it (div.s, not rsqrt.s), multiplies z, x and y by the quotient
// (stored in the order z, x, y) and returns the square root.
// Inferred: normalises in place and returns the vector's original length (zero for a zero vector). A NaN squared length is not
// equal to zero and takes the scaling path. The result is the same as Normalize__7VIVect3 except that the length is returned.
// Unknown: the class layout beyond the first 12 bytes. Nothing here ran on the EE.
#if !defined(__mips__)
extern "C" float sqrtf(float);
#endif

extern "C" {
float NormalizeMagn__7VIVect3(float* v) {
    float x = v[0], y = v[1], z = v[2];
    float length_squared = x * x + y * y + z * z;
    if (length_squared == 0.0f) return length_squared;
    float length;
#if defined(__mips__)
    __asm__("sqrt.s %0, %1" : "=f"(length) : "f"(length_squared));
#else
    length = sqrtf(length_squared);
#endif
    float scale = 1.0f / length;
    v[2] = z * scale;
    v[0] = x * scale;
    v[1] = y * scale;
    return length;
}
}
