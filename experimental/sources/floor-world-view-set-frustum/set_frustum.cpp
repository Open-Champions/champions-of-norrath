// Observed worldSetFrustum__Fffff at 0x00177810, 568 bytes.
// Three tanf calls use the .lit4 word 0x3c8efa34, once from 0x00480040 and
// once from 0x00480048. The screen word 0x43eec000 is loaded from 0x00480044
// and divided by the first tangent. That quotient is stored to worldScrz.
// The second tangent scales the second and fourth arguments into viewFrustum.
// The third tangent scales only the second argument into clipViewFrustum;
// its raw value is the Z component of the Y planes. Each table is six
// 12-byte vectors. rsqrt.s normalizes them with a numerator of 1.
// Fresh compilation is not an original-byte match. tanf stays unresolved.
#if !defined(__mips__)
extern "C" float sqrtf(float);
#endif

extern "C" float tanf(float radians);
extern "C" {
float viewFrustum[18];
float clipViewFrustum[18];
float worldScrz = 1024.0f;
}

namespace {
__attribute__((always_inline)) inline float bits_float(unsigned bits) {
    union {
        unsigned bits;
        float value;
    } converted;
    converted.bits = bits;
    return converted.value;
}

__attribute__((always_inline)) inline float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / sqrtf(length_squared);
#endif
}

__attribute__((always_inline)) inline void normalize_six(float* plane) {
    for (int index = 0; index < 6; ++index) {
        const float nx = plane[0];
        const float ny = plane[1];
        const float nz = plane[2];
        const float length_squared = (nx * nx + ny * ny) + nz * nz;
        const float scale = reciprocal_sqrt(length_squared);
        plane[0] = nx * scale;
        plane[1] = ny * scale;
        plane[2] = nz * scale;
        plane += 3;
    }
}

__attribute__((always_inline)) inline void fill_planes(float* plane, float x_scale, float y_scale) {
    const float one = 1.0f;
    const float negative = -1.0f;
    plane[0] = one;
    plane[1] = 0.0f;
    plane[2] = x_scale;
    plane[3] = negative;
    plane[4] = 0.0f;
    plane[5] = x_scale;
    plane[6] = 0.0f;
    plane[7] = one;
    plane[8] = y_scale;
    plane[9] = 0.0f;
    plane[10] = negative;
    plane[11] = y_scale;
    plane[12] = 0.0f;
    plane[13] = 0.0f;
    plane[14] = negative;
    plane[15] = 0.0f;
    plane[16] = 0.0f;
    plane[17] = one;
}
}

extern "C" void worldSetFrustum__Fffff(float first, float second, float third, float fourth) {
    const float degree_scale = bits_float(0x3c8efa34u);
    const float half = 0.5f;
    const float screen = bits_float(0x43eec000u);
    const float opening = tanf((first * degree_scale) * half);
    worldScrz = screen / opening;
    const float widened = tanf(((first + 1.0f) * degree_scale) * half);
    fill_planes(viewFrustum, widened * second, widened * fourth);
    normalize_six(viewFrustum);
    const float clip_opening = tanf((third * degree_scale) * half);
    fill_planes(clipViewFrustum, clip_opening * second, clip_opening);
    normalize_six(clipViewFrustum);
}
