// Observed: Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44 at 0x010869e0 is
// 804 bytes and returns 0. Arguments: this, a const vector p, three vector
// references x, y and z (a2, a3, t0), and a matrix reference m (t1). It builds a
// zero VIMatrix44 local, calls Init__9VIFrustum on this+304, then stores:
//  - p at this+28 (three floats);
//  - x.x, y.x, z.x, 0 at this+48; x.y, y.y, z.y, 0 at this+64; x.z, y.z, z.z, 0
//    at this+80;
//  - at this+96, 100, 104 the sums (-p.x*c0 + -p.y*c1) + -p.z*c2 where (c0, c1, c2)
//    are the floats at this+48, 64, 80 (for 96), 52, 68, 84 (for 100) and 56, 72,
//    88 (for 104); the float at this+108 is 1.0f;
//  - x, 0, y, 0, z, 0, p, 1.0f as 16 floats at this+176 (three zero-w rows then the
//    position row with w = 1.0f).
// It then calls sceVu0CopyMatrix(this+112, m), Mul__10VIMatrix44RC10VIMatrix44T1
// (this+240, this+48, this+112), Invert__10VIMatrix44RC10VIMatrix44(local, this+112),
// CalcFrustum__8VICameraRC10VIMatrix44T1(this, local, this+176), and destroys the
// local with _$_10VIMatrix44 flags 2.
// Inferred: this+48 holds a view matrix, this+176 a camera-to-world matrix and
// this+112 the matrix supplied by the caller (projection-like). The sum order
// is left to right. Unknown: the member names; the float at this+60, 76 and 92
// is a stored integer zero word.
extern "C" {
void Init__9VIFrustum(void* frustum);
void sceVu0CopyMatrix(void* destination, const void* source);
void Mul__10VIMatrix44RC10VIMatrix44T1(void* result, const void* left, const void* right);
void Invert__10VIMatrix44RC10VIMatrix44(void* result, const void* source);
void CalcFrustum__8VICameraRC10VIMatrix44T1(void* object, const void* inverse, const void* camera);
void _$_10VIMatrix44(void* matrix, int flags);

int Update__8VICameraRC7VIVect3R7VIVect3N22R10VIMatrix44(void* object, const void* position, void* x_axis, void* y_axis, void* z_axis, void* matrix) {
    unsigned char* bytes = static_cast<unsigned char*>(object);
    alignas(16) float local[16];
    for (int i = 0; i < 16; ++i) local[i] = 0.0f;
    Init__9VIFrustum(bytes + 304);
    const float* p = static_cast<const float*>(position);
    const float* x = static_cast<const float*>(x_axis);
    const float* y = static_cast<const float*>(y_axis);
    const float* z = static_cast<const float*>(z_axis);
    float* at28 = reinterpret_cast<float*>(bytes + 28);
    at28[0] = p[0];
    at28[1] = p[1];
    at28[2] = p[2];
    float* view = reinterpret_cast<float*>(bytes + 48);
    view[0] = x[0]; view[1] = y[0]; view[2] = z[0];
    *reinterpret_cast<int*>(bytes + 60) = 0;
    view[4] = x[1]; view[5] = y[1]; view[6] = z[1];
    *reinterpret_cast<int*>(bytes + 76) = 0;
    view[8] = x[2]; view[9] = y[2]; view[10] = z[2];
    *reinterpret_cast<int*>(bytes + 92) = 0;
    float nx = -p[0], ny = -p[1], nz = -p[2];
    view[12] = nx * view[0] + ny * view[4] + nz * view[8];
    view[13] = nx * view[1] + ny * view[5] + nz * view[9];
    view[14] = nx * view[2] + ny * view[6] + nz * view[10];
    view[15] = 1.0f;
    float* world = reinterpret_cast<float*>(bytes + 176);
    world[0] = x[0]; world[1] = x[1]; world[2] = x[2]; world[3] = 0.0f;
    world[4] = y[0]; world[5] = y[1]; world[6] = y[2]; world[7] = 0.0f;
    world[8] = z[0]; world[9] = z[1]; world[10] = z[2]; world[11] = 0.0f;
    world[12] = p[0]; world[13] = p[1]; world[14] = p[2]; world[15] = 1.0f;
    sceVu0CopyMatrix(bytes + 112, matrix);
    Mul__10VIMatrix44RC10VIMatrix44T1(bytes + 240, bytes + 48, bytes + 112);
    Invert__10VIMatrix44RC10VIMatrix44(local, bytes + 112);
    CalcFrustum__8VICameraRC10VIMatrix44T1(object, local, bytes + 176);
    _$_10VIMatrix44(local, 2);
    return 0;
}
}
