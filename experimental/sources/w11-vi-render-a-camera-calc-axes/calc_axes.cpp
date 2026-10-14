// Observed: CalcAxes__8VICameraRC7VIVect3R7VIVect3N22 at 0x010867c8 is 532 bytes
// and returns 0. Arguments: this, a const vector d, and three vector references
// r, u and f (a2, a3 and t0). Steps: (1) f = d, then Normalize__7VIVect3(f), the
// result ignored. (2) The three floats at this+8 receive f. (3) With the local
// vector w = (0, 1, 0), r = f x w, where x is the usual cross product
// (ay*bz - az*by, az*bx - ax*bz, ax*by - ay*bx), and Normalize__7VIVect3(r) is
// called. (4) If that call returns zero, r = f x (1, 0, 0) is computed the same
// way and normalized again, with the result ignored. (5) u = r x f. All
// arithmetic is single-precision with separate multiplies and subtracts.
// Inferred: f is the viewing direction, r the vector perpendicular to f and the
// world y axis (or the x axis when f is parallel to y), and u completes the set.
// Unknown: the member that this+8 is; the vector names beyond these roles.
extern "C" {
int Normalize__7VIVect3(void* vector);

static void cross(float* out, const float* a, const float* b) {
    float x = a[1] * b[2] - a[2] * b[1];
    float y = a[2] * b[0] - a[0] * b[2];
    float z = a[0] * b[1] - a[1] * b[0];
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

int CalcAxes__8VICameraRC7VIVect3R7VIVect3N22(void* object, const void* direction, void* right, void* up, void* forward) {
    float* f = static_cast<float*>(forward);
    const float* d = static_cast<const float*>(direction);
    f[0] = d[0];
    f[1] = d[1];
    f[2] = d[2];
    Normalize__7VIVect3(f);
    float* member = reinterpret_cast<float*>(static_cast<unsigned char*>(object) + 8);
    member[0] = f[0];
    member[1] = f[1];
    member[2] = f[2];
    float* r = static_cast<float*>(right);
    float w[3] = {0.0f, 1.0f, 0.0f};
    cross(r, f, w);
    if (Normalize__7VIVect3(r) == 0) {
        w[0] = 1.0f;
        w[1] = 0.0f;
        w[2] = 0.0f;
        cross(r, f, w);
        Normalize__7VIVect3(r);
    }
    cross(static_cast<float*>(up), r, f);
    return 0;
}
}
