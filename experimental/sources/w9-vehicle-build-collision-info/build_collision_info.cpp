// Observed buildCollisionInfo__7Vehicle at 0x00276b58, 1136 bytes. Nothing is
// returned. Nothing here was executed on the EE. Field meanings are not
// established here, so offsets stay numeric. Let a, b, c, d be the floats at
// +928, +932, +936, +940 and n the signed word at +1000.
//   1. The Point3 at +2040 becomes (0, float(+900), float(+904)); each of the
//      n Point3 values at +1236+12i has that Point3 subtracted component-wise.
//   2. Four corners are written: (a/2, -b, 0) at +404, (c/2, d, 0) at +416,
//      (-c/2, d, 0) at +428 and (-a/2, -b, 0) at +440; the float at +836 is
//      first set to 0.0.
//   3. For each corner k (next = (k+1) mod 4): e = corner[next] - corner[k]
//      scaled by rsqrt.s(1.0, e.e); the normal at +500+12k is e x (0, 0, 1)
//      (x = e.y*1 - e.z*0, y = e.z*0 - e.x*1, z = e.x*0 - e.y*0) scaled by
//      rsqrt.s(1.0, n.n), with every squared length summed as (x*x + y*y) +
//      z*z; the float at +836 grows to the length of corner[k] (square root of
//      the same sum) when that is larger.
//   4. h = (c + a) * 0.5 * 0.5 (called r2 below); q = ((b + d) - 2*r2) / (r2 *
//      0.5); q + 0.5 when q >= 0.0 else q - 0.5 is converted with cvt.w.s
//      (truncation) into the word at +2140 (m). step = ((((b+d) * 0.5) - r2) -
//      (-(b+d) * 0.5 + r2)) / float(m - 1). When m > 0, for i = 0..m-1 (m
//      re-read each pass): the sphere slot at +2104+4i, if non-null, has its
//      object's virtual in table offset 8 (halfword delta, pointer at +4)
//      called with flag 3; blockAlloc(84) then, if non-null, the
//      VehicleBodySphere constructor (object, vehicle, y, r2) with
//      y = -(b+d) * 0.5 + r2 + step * float(i); the slot gets the new object
//      (null when the allocation failed).
//   5. r = min.s(60.0, float(+836)) is stored at +28 and (unsigned)r as a byte
//      at +40 (the 2^31 adjustment of an unsigned conversion is kept).
struct __attribute__((may_alias)) Point3 { float x, y, z; };

extern "C" {
void* blockAlloc__Fi(int size);
void __17VehicleBodySphereP7Vehicleff(void* sphere, unsigned char* vehicle, float y, float radius);
}

typedef float AliasedFloat __attribute__((may_alias));
typedef int AliasedInt __attribute__((may_alias));
typedef short AliasedShort __attribute__((may_alias));
typedef unsigned char* AliasedPointer __attribute__((may_alias));
typedef void (*DestroyFunction)(unsigned char*, int);
typedef DestroyFunction AliasedDestroy __attribute__((may_alias));

namespace {
inline int& word(unsigned char* base, int offset) { return *reinterpret_cast<AliasedInt*>(base + offset); }
inline float& real(unsigned char* base, int offset) { return *reinterpret_cast<AliasedFloat*>(base + offset); }
inline float rsqrt_s(float numerator, float value) {
#if defined(__mips__)
    float out;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(numerator), "f"(value));
    return out;
#else
    return numerator / __builtin_sqrtf(value);
#endif
}
inline float min_s(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("min.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return b < a ? b : a;
#endif
}
inline int cvt_w_s(float value) {
#if defined(__mips__)
    int out;
    float scratch;
    __asm__("cvt.w.s %1, %2\n\tmfc1 %0, %1" : "=r"(out), "=&f"(scratch) : "f"(value));
    return out;
#else
    return static_cast<int>(value);
#endif
}
}

extern "C" void buildCollisionInfo__7Vehicle(unsigned char* vehicle) {
    Point3* const offset = reinterpret_cast<Point3*>(vehicle + 2040);
    offset->x = 0.0f;
    offset->y = real(vehicle, 900);
    offset->z = real(vehicle, 904);
    for (int i = 0; i < word(vehicle, 1000); ++i) {
        Point3* const p = reinterpret_cast<Point3*>(vehicle + 1236 + 12 * i);
        p->x = p->x - offset->x;
        p->y = p->y - offset->y;
        p->z = p->z - offset->z;
    }

    const float a = real(vehicle, 928);
    const float b = real(vehicle, 932);
    const float c = real(vehicle, 936);
    const float d = real(vehicle, 940);
    Point3* const corner = reinterpret_cast<Point3*>(vehicle + 404);
    Point3* const normal = reinterpret_cast<Point3*>(vehicle + 500);
    word(vehicle, 412) = 0;
    corner[0].x = a * 0.5f;
    corner[3].y = -b;
    corner[3].x = -a * 0.5f;
    corner[2].x = -c * 0.5f;
    corner[1].x = c * 0.5f;
    corner[2].y = d;
    corner[1].y = d;
    corner[0].y = -b;
    corner[0].z = 0.0f;
    corner[1].z = 0.0f;
    corner[2].z = 0.0f;
    corner[3].z = 0.0f;
    Point3 up;
    up.x = 0.0f;
    up.y = 0.0f;
    up.z = 1.0f;
    __asm__ volatile("" : : "r"(&up) : "memory");
    real(vehicle, 836) = 0.0f;
    for (int k = 0; k < 4; ++k) {
        const Point3* const here = &corner[k];
        const Point3* const there = &corner[k == 3 ? 0 : k + 1];
        Point3 e;
        Point3 cross;
        e.x = there->x - here->x;
        e.y = there->y - here->y;
        e.z = there->z - here->z;
        const float scale = rsqrt_s(1.0f, e.x * e.x + e.y * e.y + e.z * e.z);
        e.z = e.z * scale;
        e.y = e.y * scale;
        e.x = e.x * scale;
        cross.x = e.y * up.z - e.z * up.y;
        cross.y = e.z * up.x - e.x * up.z;
        cross.z = e.x * up.y - e.y * up.x;
        normal[k] = cross;
        const float inverse = rsqrt_s(1.0f, normal[k].x * normal[k].x + normal[k].y * normal[k].y + normal[k].z * normal[k].z);
        normal[k].z = normal[k].z * inverse;
        normal[k].x = normal[k].x * inverse;
        normal[k].y = normal[k].y * inverse;
        const float length = __builtin_sqrtf(here->x * here->x + here->y * here->y + here->z * here->z);
        if (real(vehicle, 836) < length) real(vehicle, 836) = length;
    }

    const float r2 = (c + a) * 0.5f * 0.5f;
    float q = ((b + d) - (r2 + r2)) / (r2 * 0.5f);
    if (0.0f <= q) q = q + 0.5f;
    else q = q - 0.5f;
    const int m = cvt_w_s(q);
    word(vehicle, 2140) = m;
    const float step = (((b + d) * 0.5f - r2) - (-(b + d) * 0.5f + r2)) / static_cast<float>(m - 1);
    int i = 0;
    if (m > 0) do {
        const float y = -(b + d) * 0.5f + r2 + step * static_cast<float>(i);
        unsigned char** const slot = reinterpret_cast<unsigned char**>(vehicle + 2104 + 4 * i);
        unsigned char* const old = *reinterpret_cast<AliasedPointer*>(slot);
        if (old != 0) {
            unsigned char* const entry = *reinterpret_cast<AliasedPointer*>(old + 72) + 8;
            (*reinterpret_cast<AliasedDestroy*>(entry + 4))(old + *reinterpret_cast<AliasedShort*>(entry), 3);
        }
        void* const sphere = blockAlloc__Fi(84);
        if (sphere != 0) __17VehicleBodySphereP7Vehicleff(sphere, vehicle, y, r2);
        *reinterpret_cast<AliasedPointer*>(slot) = static_cast<unsigned char*>(sphere);
        ++i;
    } while (i < word(vehicle, 2140));

    float r = min_s(60.0f, real(vehicle, 836));
    unsigned int whole;
    real(vehicle, 28) = r;
    if (0x1p31f <= r) whole = static_cast<unsigned int>(cvt_w_s(r - 0x1p31f)) | 0x80000000u;
    else whole = static_cast<unsigned int>(cvt_w_s(r));
    *(vehicle + 40) = static_cast<unsigned char>(whole);
}
