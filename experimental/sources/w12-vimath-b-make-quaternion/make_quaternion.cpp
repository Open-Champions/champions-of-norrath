// Observed: makeQuaternion__FP4Quatsfff at 0x00159e00 is 192 bytes. Arguments: q, angle (short in
// a1), x (f12), y (f13), z (f14). With h = (short)angle >> 1 (arithmetic), n = rsqrt.s(1.0f,
// (x*x + y*y) + z*z) (rsqrt.s computes 1.0f / sqrt of its third operand), it calls icos__Fi(h),
// then isin__Fi(h), and stores q[3] = icos result, s = isin result * n, q[2] = s*z, q[0] = s*x,
// q[1] = s*y. Nothing is returned.
// Inferred: a rotation quaternion about the axis (x, y, z) with the integer angle halved; the
// angle unit is that of icos/isin (not established here).
// Unknown: behaviour for a zero axis beyond what rsqrt.s does.
extern "C" {
float icos__Fi(int angle);
float isin__Fi(int angle);

void makeQuaternion__FP4Quatsfff(float* q, short angle, float x, float y, float z) {
    float length_squared = x * x + y * y + z * z;
    float n;
    const float one = 1.0f;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(n) : "f"(one), "f"(length_squared));
    int half = static_cast<int>(angle) >> 1;
    float c = icos__Fi(half);
    float s = isin__Fi(half) * n;
    q[3] = c;
    q[2] = s * z;
    q[0] = s * x;
    q[1] = s * y;
}
}
