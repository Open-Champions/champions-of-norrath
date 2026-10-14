// Observed worldTransformFrustum__FP8Matrix34 at 0x00177a48, 1156 bytes. Nothing
// is returned. Nothing here was executed on the EE. The argument is read as twelve
// floats m[0..11]: a 3x3 block in rows of three (m[0..8]) and a Point3 at m[9..11].
// Let n = -(m[9], m[10], m[11]) and p = (n.x*m0 + n.y*m1 + n.z*m2, n.x*m3 + n.y*m4 +
// n.z*m5, n.x*m6 + n.y*m7 + n.z*m8), each summed left to right in single precision.
// viewFrustum (0x006eb9b0) and clipViewFrustum (0x006eba58) are six Point3s each
// (18 floats); worldFrustum (0x0037eea0) and worldClipFrustum (0x006eb9f8) are six
// planes of four floats (24 floats).
// 1. halfFrustumMode (byte, 0x00484134): the floats 0..2 of viewFrustum are saved,
//    and floats 3..5 are saved. Mode 1 or 3 sets floats 0..2 to (1.0, 0, 0). Mode 2
//    or 3 sets floats 3..5 to (-1.0, 0, 0). Other modes change nothing.
// 2. For plane i of worldFrustum from the Point3 v = viewFrustum[3i..3i+2]:
//    plane[0] = v.x*m0 + v.y*m1 + v.z*m2, plane[1] = v.x*m3 + v.y*m4 + v.z*m5,
//    plane[2] = v.x*m6 + v.y*m7 + v.z*m8, plane[3] = -(plane[0]*p.x + plane[1]*p.y +
//    plane[2]*p.z).
// 3. The saved floats go back into viewFrustum. worldFrustum[19] gets farClip
//    (0x004869d8) added and worldFrustum[23] gets 18.0 subtracted. The signed byte
//    array frustumNormalSign (0x006ec790) receives 1 at index 3i+k when
//    worldFrustum[4i+k] > 0.0, else 0, for i in 0..5 and k in 0..2.
// 4. The same construction runs for clipViewFrustum into worldClipFrustum, then
//    worldClipFrustum[19] += farClip, worldClipFrustum[23] -= 18.0, and the signs go
//    to clipFrustumNormalSign (0x006ec7a8). The modified viewFrustum of step 1 is
//    used by step 2 only; clipViewFrustum is not changed.
// What the planes mean (a view volume in world space and a clip variant) is inferred
// from the names only.
extern "C" {
extern unsigned char halfFrustumMode;
extern float farClip;
extern float viewFrustum[18];
extern float clipViewFrustum[18];
extern float worldFrustum[24];
extern float worldClipFrustum[24];
extern signed char frustumNormalSign[18];
extern signed char clipFrustumNormalSign[18];
}

typedef float __attribute__((may_alias)) AliasedFloat;

static inline void transformPlanes(const float* m, const float* normals, float* planes, const float* p) {
    for (int i = 0; i < 6; ++i) {
        const float* v = normals + 3 * i;
        float* plane = planes + 4 * i;
        plane[0] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2];
        plane[1] = v[0] * m[3] + v[1] * m[4] + v[2] * m[5];
        plane[2] = v[0] * m[6] + v[1] * m[7] + v[2] * m[8];
        plane[3] = -(plane[0] * p[0] + plane[1] * p[1] + plane[2] * p[2]);
    }
}

static inline void signs(const float* planes, signed char* out) {
    for (int i = 0; i < 6; ++i) {
        for (int k = 0; k < 3; ++k) out[3 * i + k] = 0.0f < planes[4 * i + k] ? 1 : 0;
    }
}

extern "C" void worldTransformFrustum__FP8Matrix34(const float* m) {
    const unsigned char mode = halfFrustumMode;
    const float n0 = -m[9];
    const float n1 = -m[10];
    const float n2 = -m[11];
    float p[3];
    p[0] = n0 * m[0] + n1 * m[1] + n2 * m[2];
    p[1] = n0 * m[3] + n1 * m[4] + n2 * m[5];
    p[2] = n0 * m[6] + n1 * m[7] + n2 * m[8];

    float saved[6];
    for (int i = 0; i < 6; ++i) saved[i] = viewFrustum[i];
    if (mode != 0) {
        if (mode == 1 || mode == 3) {
            viewFrustum[1] = 0.0f;
            viewFrustum[0] = 1.0f;
            viewFrustum[2] = 0.0f;
        }
        if (static_cast<unsigned char>(mode - 2) < 2) {
            viewFrustum[4] = 0.0f;
            viewFrustum[3] = -1.0f;
            viewFrustum[5] = 0.0f;
        }
    }
    transformPlanes(m, viewFrustum, worldFrustum, p);
    for (int i = 0; i < 6; ++i) viewFrustum[i] = saved[i];
    const float far = farClip;
    worldFrustum[19] = worldFrustum[19] + far;
    worldFrustum[23] = worldFrustum[23] - 18.0f;
    signs(worldFrustum, frustumNormalSign);

    transformPlanes(m, clipViewFrustum, worldClipFrustum, p);
    worldClipFrustum[19] = worldClipFrustum[19] + far;
    worldClipFrustum[23] = worldClipFrustum[23] - 18.0f;
    signs(worldClipFrustum, clipFrustumNormalSign);
}
