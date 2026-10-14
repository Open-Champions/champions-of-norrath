// Observed FinalizePosition__5Wheel at 0x00277440, 460 bytes. Nothing is
// returned. For a wheel `w` with i the word at w+384 and `car` the pointer at
// w+396 (the Vehicle):
//   1. The nine words of the 3x3 matrix at car+1116 are copied to w+400.
//   2. u = the third row of that copy (floats at w+424, +428, +432);
//      A = float at car + 676 + 4 * i; q = pointer at car + 2056 + 4 * i;
//      k = ((-q[0]) * 39.0) * (1.0 - A) + (float at w+376) * 39.0;
//      the Point3 at w+16 becomes (Point3 at car + 1332 + 12 * i) + u * k,
//      componentwise.
//   3. objectUpdateInGrid__FP10GameObject(w) is called.
//   4. If the float at w+372 is not 0.0, matRot__FP8Matrix33is(w + 400, 2,
//      angle) rotates the copied matrix about axis 2 by the 16-bit value
//      angle = (short)(int)((float at car+800 * 32767.0) * (float at w+372 * K1)),
//      K1 being the float word 0x3bb60b60.
//   5. t = (float at w+368) + ((float at w+364) * K2) * K3, with K2 the float
//      word 0x3c888888 and K3 = 0x40bfffff; t is stored at w+368 and, when
//      360.0 <= t, 360.0 is subtracted and the result stored again.
// That the matrix is a wheel orientation, +376 and +372 a steer or spin input,
// and +364/+368 a rotation speed and angle (in degrees, wrapped at 360) are
// inferences from the arithmetic and from the sibling functions; none of them
// is established. Nothing here was executed on the EE.
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
void objectUpdateInGrid__FP10GameObject(void* object);
void matRot__FP8Matrix33is(void* matrix, int axis, short angle);
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef unsigned AliasedWord __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));

namespace {
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline int word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
}

extern "C" void FinalizePosition__5Wheel(unsigned char* wheel) {
    unsigned char* const car = *reinterpret_cast<AliasedPointer*>(wheel + 396);
    for (int k = 0; k < 9; ++k) {
        *reinterpret_cast<AliasedWord*>(wheel + 400 + 4 * k) = *reinterpret_cast<AliasedWord*>(car + 1116 + 4 * k);
    }
    const int index = word(wheel, 384);
    const Point3 u = {real(wheel, 424), real(wheel, 428), real(wheel, 432)};
    const float amount = real(car, 676 + 4 * index);
    const float* const q = reinterpret_cast<const float*>(*reinterpret_cast<AliasedPointer*>(car + 2056 + 4 * index));
    const float complement = 1.0f - amount;
    const float k = ((-q[0]) * 39.0f) * complement + real(wheel, 376) * 39.0f;
    const Point3 d = {u.x * k, u.y * k, u.z * k};
    const Point3* const base = reinterpret_cast<const Point3*>(car + 1332 + 12 * index);
    Point3 p;
    p.x = base->x + d.x;
    p.y = base->y + d.y;
    p.z = base->z + d.z;
    *reinterpret_cast<Point3*>(wheel + 16) = p;
    objectUpdateInGrid__FP10GameObject(wheel);
    const float turn = real(wheel, 372);
    if (turn != 0.0f) {
        const float scaled = (real(car, 800) * 32767.0f) * (turn * 0x1.6c16c0p-8f);
        matRot__FP8Matrix33is(wheel + 400, 2, static_cast<short>(static_cast<int>(scaled)));
    }
    float t = real(wheel, 368) + (real(wheel, 364) * 0x1.111110p-6f) * 0x1.7ffffep+2f;
    real(wheel, 368) = t;
    if (360.0f <= t) {
        t = t - 360.0f;
        real(wheel, 368) = t;
    }
}
