// Observed worldClipCubeToClipFrustum__FPA1_fi at 0x00178008, 208 bytes.
// Six planes of four floats are read from worldClipFrustum (0x006eb9f8) and
// three signed bytes per plane from clipFrustumNormalSign (0x006ec7a8). The
// arguments are a pointer to six floats (min/max pairs per axis) and a mask.
// For each plane whose mask bit is clear, each sign byte has its low bit
// flipped and selects one float in its axis pair (x at 0, y at +2, z at +4).
// The value plane.w + a*x + b*y + c*z is accumulated in that order; if it is
// not negative the plane's mask bit is set. Planes whose bit is already set
// are skipped. Unlike the frustum clip, there is no early return for a
// negative corner. The finished mask is returned.
// Both tables are defined by other work in this build and stay unresolved.
extern "C" {
extern float worldClipFrustum[24];
extern signed char clipFrustumNormalSign[24];
int worldClipCubeToClipFrustum__FPA1_fi(float* bounds, int mask) {
    for (int plane = 0; plane < 6; ++plane) {
        if (((mask >> plane) & 1) != 0) continue;
        const signed char* sign = clipFrustumNormalSign + plane * 3;
        const float* row = worldClipFrustum + plane * 4;
        const int x = static_cast<signed char>(static_cast<unsigned char>(sign[0]) ^ 1u);
        const int y = static_cast<signed char>(static_cast<unsigned char>(sign[1]) ^ 1u);
        const int z = static_cast<signed char>(static_cast<unsigned char>(sign[2]) ^ 1u);
        const float far = (row[3] + row[0] * bounds[x]) + row[1] * bounds[y + 2] + row[2] * bounds[z + 4];
        if (0.0f <= far) mask |= 1 << plane;
    }
    return mask;
}
}
