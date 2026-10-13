// Observed lightSetSceneLights__FG6Point3iiiiiiiii at 0x0013e928, extent 240.
// (direction, i1..i9). The Point3 arrives by pointer to a caller copy. Arguments
// i1..i7 are in registers a1..a3 and t0..t3; i8 and i9 are the first two stack
// arguments. The direction is copied to a local and normalised with
//   scale = 1.0f / sqrt((x*x + y*y) + z*z)     (one reciprocal square root)
// and its three scaled floats are stored at scene+0, +4 and +8 (scene, 0x004f4060).
// The word i9 is stored at scene+52. The low bytes of i1..i7 are stored at scene+36
// to +42, and again at scene+44 to +50; the low byte of i8 is stored at scene+43
// and again at scene+51. Then transformSceneLightTask__Fv and
// lightResetOptionalParams__Fv are called, in that order.
// Inferred: two colour groups of seven bytes plus an eighth byte each, and a
// word that follows them; their roles are not established. A zero-length vector
// follows the hardware reciprocal square root (no guard in the original).
// scene and both callees are defined by other packages.
struct Point3 {
    float x, y, z;
};

extern "C" {
extern unsigned char scene[];
void transformSceneLightTask__Fv();
void lightResetOptionalParams__Fv();

void lightSetSceneLights__FG6Point3iiiiiiiii(Point3* direction, int i1, int i2, int i3, int i4, int i5, int i6,
                                             int i7, int i8, int i9) {
    Point3 v = *direction;
    const float scale = 1.0f / __builtin_sqrtf((v.x * v.x + v.y * v.y) + v.z * v.z);
    v.x = v.x * scale;
    v.y = v.y * scale;
    v.z = v.z * scale;
    float* out = reinterpret_cast<float*>(scene);
    out[0] = v.x;
    out[1] = v.y;
    out[2] = v.z;
    *reinterpret_cast<int*>(scene + 52) = i9;
    scene[43] = static_cast<unsigned char>(i8);
    scene[36] = static_cast<unsigned char>(i1);
    scene[37] = static_cast<unsigned char>(i2);
    scene[38] = static_cast<unsigned char>(i3);
    scene[39] = static_cast<unsigned char>(i4);
    scene[40] = static_cast<unsigned char>(i5);
    scene[41] = static_cast<unsigned char>(i6);
    scene[42] = static_cast<unsigned char>(i7);
    scene[51] = static_cast<unsigned char>(i8);
    scene[44] = static_cast<unsigned char>(i1);
    scene[45] = static_cast<unsigned char>(i2);
    scene[46] = static_cast<unsigned char>(i3);
    scene[47] = static_cast<unsigned char>(i4);
    scene[48] = static_cast<unsigned char>(i5);
    scene[49] = static_cast<unsigned char>(i6);
    scene[50] = static_cast<unsigned char>(i7);
    transformSceneLightTask__Fv();
    lightResetOptionalParams__Fv();
}
}
