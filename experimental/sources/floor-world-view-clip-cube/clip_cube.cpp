// Observed worldClipCubeToFrustum__FPA1_fi at 0x00177ed0, 308 bytes.
// Six planes live in worldFrustum at 0x0037eea0, 16 bytes each. The file
// bytes are zero. Three signed bytes per plane live in frustumNormalSign at
// 0x006ec790. That symbol's size field is 0; droplet's neighbor is 24 bytes
// later, so the table is 24 bytes. Both objects are zero in the image.
// worldTransformFrustum__FP8Matrix34 at 0x00177a48 is the writer and is not
// in this package.
//
// A mask bit that is already 1 skips that plane. Otherwise the bytes select
// one end of each bounds pair. The near corner is plane.w plus the three
// scaled components. A negative near corner returns 0 immediately. The far
// corner flips each byte's low bit. A far corner that is not negative sets
// the plane's mask bit. The finished mask is the return value.
extern "C" {
float worldFrustum[24];
signed char frustumNormalSign[24];
}

namespace {
__attribute__((always_inline)) inline int opposite_index(unsigned char raw) {
    return static_cast<signed char>(static_cast<unsigned char>(raw ^ 1u));
}
}

extern "C" int worldClipCubeToFrustum__FPA1_fi(float* bounds, int mask) {
    for (int plane = 0; plane < 6; ++plane) {
        if (((mask >> plane) & 1) != 0) continue;
        const signed char* sign = frustumNormalSign + plane * 3;
        const float* row = worldFrustum + plane * 4;
        const int near_x = sign[0];
        const int near_y = sign[1];
        const int near_z = sign[2];
        const float near = (row[3] + row[0] * bounds[near_x]) +
                           row[1] * bounds[near_y + 2] +
                           row[2] * bounds[near_z + 4];
        if (near < 0.0f) return 0;
        const float far = (row[3] + row[0] * bounds[opposite_index(static_cast<unsigned char>(sign[0]))]) +
                          row[1] * bounds[opposite_index(static_cast<unsigned char>(sign[1])) + 2] +
                          row[2] * bounds[opposite_index(static_cast<unsigned char>(sign[2])) + 4];
        if (0.0f <= far) mask |= 1 << plane;
    }
    return mask;
}
