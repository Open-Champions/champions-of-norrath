// Observed lightPushSceneLights__FG6Point3iiiiiiiii at 0x0013ccc8, 420 bytes.
// The Point3 arrives by address; the nine integers are a1..a3, t0..t3 and two
// stack words (the eighth and ninth). Only the low byte of the first eight is
// stored; the ninth is a whole word.
//  1. A signed interfaceMode (0x00483aac) above zero returns at once. Otherwise
//     interfaceMode is stored as itself plus one.
//  2. The three direction floats are read, then the 56 bytes of scene
//     (0x004f4060) are copied to savedScene (0x004f4098).
//  3. The direction is normalized: length squared is (x*x + y*y) + z*z, the
//     scale is rsqrt.s with numerator 1.0f, and each component is multiplied by
//     it. The result is stored in scene bytes 0 to 11.
//  4. Scene bytes: 36,37,38,39,40 take integers 1..5; 41 takes 6; 42 takes 7;
//     43 takes 8; 44,45,46,47,48 take integers 1..5 again; 49 takes 6; 50 takes
//     7; 51 takes 8. The word at 52 takes integer 9.
//  5. The store to byte 42 sits in the delay slot of the call, so it lands
//     before transformSceneLightTask__Fv runs; then lightResetOptionalParams__Fv.
// Fresh compilation is not an original-byte match.
extern "C" {
extern int interfaceMode;
extern unsigned char scene[];
extern unsigned char savedScene[];
void transformSceneLightTask__Fv();
void lightResetOptionalParams__Fv();
}

namespace {
float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}
}

extern "C" void lightPushSceneLights__FG6Point3iiiiiiiii(const float* direction, int a, int b, int c, int d, int e,
                                                          int f, int g, int h, int i) {
    const int mode = interfaceMode;
    if (mode > 0) return;
    interfaceMode = mode + 1;
    float x = direction[0];
    float y = direction[1];
    float z = direction[2];
    for (unsigned n = 0u; n < 56u; n += 4u) {
        *reinterpret_cast<unsigned*>(savedScene + n) = *reinterpret_cast<const unsigned*>(scene + n);
    }
    const float scale = reciprocal_sqrt(x * x + y * y + z * z);
    x *= scale;
    y *= scale;
    z *= scale;
    float* out = reinterpret_cast<float*>(scene);
    out[0] = x;
    out[1] = y;
    out[2] = z;
    scene[36] = static_cast<unsigned char>(a);
    scene[37] = static_cast<unsigned char>(b);
    scene[38] = static_cast<unsigned char>(c);
    scene[39] = static_cast<unsigned char>(d);
    scene[40] = static_cast<unsigned char>(e);
    scene[51] = static_cast<unsigned char>(h);
    scene[44] = static_cast<unsigned char>(a);
    scene[45] = static_cast<unsigned char>(b);
    scene[46] = static_cast<unsigned char>(c);
    scene[47] = static_cast<unsigned char>(d);
    scene[48] = static_cast<unsigned char>(e);
    scene[49] = static_cast<unsigned char>(f);
    scene[50] = static_cast<unsigned char>(g);
    scene[41] = static_cast<unsigned char>(f);
    *reinterpret_cast<int*>(scene + 52) = i;
    scene[43] = static_cast<unsigned char>(h);
    scene[42] = static_cast<unsigned char>(g);
    transformSceneLightTask__Fv();
    lightResetOptionalParams__Fv();
}
