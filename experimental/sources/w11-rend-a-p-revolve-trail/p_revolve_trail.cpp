// Observed P_RevolveTrail__FiG6Point3N31i at 0x00183778, 524 bytes. Arguments: a trail handle,
// four Point3 values (each arrives by address of a caller copy; the routine copies all four to
// locals on entry, here p0..p3), and a step count. A zero handle returns. The 16-byte header
// at trail + (handle & 0xff)*1040 + 1024 (trail is the table at 0x00780b00) must hold the full
// handle in its signed halfword at +0, else the call returns. Then:
//   n = min(step count, 60); header bytes +4 and +5 = 0; halfword +2 = 0; halfword +8 = n;
//   a quaternion q is built with makeQuaternion__FP4Quatsfff(&q, (short)(65536 / (n - 2)),
//   p1.x, p1.y, p1.z) (a 32-bit signed divide whose result is truncated to 16 bits).
//   With n <= 0 the routine ends. Otherwise, n times:
//     A = p2 + p0, B = p3 + p0 (componentwise), P_ExtendSwoosh__FiG6Point3T1f(handle, &A, &B, 1.0f),
//     quatTransform__FP6Point3P4QuatT0(&p2, &q, &p2), the same for p3, then the header halfword
//     at +2 is incremented (read from memory, unsigned 16-bit).
// Nothing is returned.
// Inferred: p1 is an axis and p0 a centre offset; the two points p2/p3 are rotated by the same
// quaternion each step and handed to the swoosh code as a strip edge, so the strip sweeps an arc.
// Unknown: units of the angle, and what the header halfword at +2 counts beyond the increment.
// A step count of 2 divides by zero: the original has no zero check (the reviewed compiler would
// insert a trap), so that case is spelled out as the MIPS III result for a positive dividend,
// LO = -1. That value is inferred from the architecture, not observed on hardware. Nothing here
// was executed on the EE.
struct Point3 {
    float x, y, z;
};
struct Quat {
    float x, y, z, w;
};
extern "C" {
extern unsigned char trail[66560];
void makeQuaternion__FP4Quatsfff(Quat* q, short angle, float x, float y, float z);
void P_ExtendSwoosh__FiG6Point3T1f(int handle, Point3* a, Point3* b, float width);
void quatTransform__FP6Point3P4QuatT0(Point3* in, Quat* q, Point3* out);
void P_RevolveTrail__FiG6Point3N31i(int handle, Point3* a, Point3* b, Point3* c, Point3* d, int steps) {
    Point3 p0 = *a;
    Point3 p1 = *b;
    Point3 p2 = *c;
    Point3 p3 = *d;
    if (handle == 0) return;
    unsigned char* header = trail + (handle & 0xff) * 1040 + 1024;
    if (*reinterpret_cast<short*>(header) != handle) return;
    int n = steps < 60 ? steps : 60;
    header[5] = 0;
    header[4] = 0;
    *reinterpret_cast<short*>(header + 2) = 0;
    *reinterpret_cast<short*>(header + 8) = static_cast<short>(n);
    Quat q;
    makeQuaternion__FP4Quatsfff(&q, static_cast<short>(n == 2 ? -1 : 65536 / (n - 2)), p1.x, p1.y, p1.z);
    if (n <= 0) return;
    for (; n != 0; --n) {
        Point3 first = {p2.x + p0.x, p2.y + p0.y, p2.z + p0.z};
        Point3 second = {p3.x + p0.x, p3.y + p0.y, p3.z + p0.z};
        P_ExtendSwoosh__FiG6Point3T1f(handle, &first, &second, 1.0f);
        quatTransform__FP6Point3P4QuatT0(&p2, &q, &p2);
        quatTransform__FP6Point3P4QuatT0(&p3, &q, &p3);
        unsigned short& count = *reinterpret_cast<unsigned short*>(header + 2);
        count = static_cast<unsigned short>(count + 1);
    }
}
}
