// Observed: extractRotation__FP4Quat at 0x0015fc08 is 164 bytes (a local symbol in the image).
// With q = (x, y, z, w) it calls iatan2__Fff(2*(x*y + z*w), ((x*x - y*y) - z*z) + w*w) (first
// argument f12, second f13), keeps the integer result a, builds a quaternion r in a stack
// temporary with makeQuaternion__FP4Quatsfff(&r, (short)(-a), 0.0f, 0.0f, 1.0f), calls
// quatMul__FP4QuatN20(q, &r, q), and returns a.
// Inferred: removes the rotation about the third axis from q and returns the removed angle.
// Unknown: which game axis that is and the angle unit.
extern "C" {
int iatan2__Fff(float y, float x);
void makeQuaternion__FP4Quatsfff(float* q, short angle, float x, float y, float z);
void quatMul__FP4QuatN20(float* out, const float* a, const float* b);

int extractRotation__FP4Quat(float* q) {
    float x = q[0], y = q[1], z = q[2], w = q[3];
    float cross = x * y + z * w;
    int angle = iatan2__Fff(cross + cross, ((x * x - y * y) - z * z) + w * w);
    alignas(16) float r[4];
    makeQuaternion__FP4Quatsfff(r, static_cast<short>(-angle), 0.0f, 0.0f, 1.0f);
    quatMul__FP4QuatN20(q, r, q);
    return angle;
}
}
