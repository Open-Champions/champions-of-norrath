// Observed msg_run__8LostSoul at 0x00219858, 744 bytes. O is the object, the first argument. frameNm is the global frame counter.
// Constants named by their float bits (no symbol names them): m = 0x3f410624 (about 0.754). Steps in order:
//  1. When the word at O+164 is zero, the unsigned halfword at O+56 gains 500 (kept as 16 bits) and the point at O+88 becomes
//     (icos__Fi(h) * 240.0, isin__Fi(h) * 240.0, 50.0) with h that halfword (as signed). Otherwise the double
//     litodp(the word at O+84 - frameNm) is compared with 30.0 (bits 0x403e000000000000) using dpcmp, and when it is smaller the
//     single at O+96 gains 50.0.
//  2. move__10ProjectileG6Point3(O, a copy of the point at O+88) is called.
//  3. A = (the single at O+92, minus the single at O+88, 0.0) is normalised (squared length x*x + y*y, then + z*z; the components
//     are multiplied by rsqrt.s of it) and multiplied by k1 = icos__Fi(frameNm * 6880) * m; B = (0, 0, 1) is multiplied by
//     k2 = isin__Fi(frameNm * 6880) * m (the sine call comes after the cosine one). The position is ((A + B) + the point at O+16),
//     added component by component in that order, and P_ExtendTrail__FiG6Point3(the word at O+172, position) is called.
//  4. When the word at O+168 is nonzero, the point at O+16 is copied to O+148 and lightUpdatePosition__FP5Light is called on O+100.
//  5. When frameNm is not below the signed word at O+84 and O is not null, the entry at offset 8 of the table whose address is the
//     word at O+72 is called on O (adjusted by the entry's signed halfword) with the argument 3.
// Inferred: a drifting soul that orbits a point and ends at a frame number, leaving a trail and carrying a light. Unknown: what the
// words at O+164, +168 and +172 and the singles at O+88..O+96 mean beyond their use here. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
float icos__Fi(int angle);
float isin__Fi(int angle);
long long litodp(int a);
int dpcmp(long long a, long long b);
void move__10ProjectileG6Point3(void* self, Point3 p);
void P_ExtendTrail__FiG6Point3(int trail, Point3 p);
void lightUpdatePosition__FP5Light(void* light);

void msg_run__8LostSoul(void* self) {
    using namespace w12c;
    unsigned char* o = static_cast<unsigned char*>(self);
    if (word(o, 164) == 0u) {
        unsigned short& angle = *reinterpret_cast<unsigned short*>(o + 56);
        angle = static_cast<unsigned short>(angle + 500);
        const float x = icos__Fi(static_cast<short>(angle)) * bits(0x43700000u);
        const float y = isin__Fi(half(o, 56)) * bits(0x43700000u);
        const Point3 at = {x, y, 50.0f};
        *reinterpret_cast<Point3*>(o + 88) = at;
    } else if (dpcmp(litodp(sword(o, 84) - frameNm), 0x403e000000000000LL) < 0) {
        real(o, 96) = real(o, 96) + 50.0f;
    }
    {
        const Point3 at = *reinterpret_cast<Point3*>(o + 88);
        move__10ProjectileG6Point3(o, at);
    }
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    Point3 a = {real(o, 92), -real(o, 88), zero};
    Point3 b = {zero, zero, 1.0f};
    float inv;
    const float one = 1.0f;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(inv) : "f"(one), "f"(a.x * a.x + a.y * a.y + a.z * a.z));
    a.x = a.x * inv;
    a.y = a.y * inv;
    a.z = a.z * inv;
    const float k1 = icos__Fi(frameNm * 6880) * bits(0x3f410624u);
    a.x = a.x * k1;
    a.y = a.y * k1;
    a.z = a.z * k1;
    const float k2 = isin__Fi(frameNm * 6880) * bits(0x3f410624u);
    b.x = b.x * k2;
    b.y = b.y * k2;
    b.z = b.z * k2;
    const Point3 p = *reinterpret_cast<Point3*>(o + 16);
    Point3 position = {a.x + b.x, a.y + b.y, a.z + b.z};
    position.x = position.x + p.x;
    position.y = position.y + p.y;
    position.z = position.z + p.z;
    P_ExtendTrail__FiG6Point3(static_cast<int>(word(o, 172)), position);
    if (word(o, 168) != 0u) {
        *reinterpret_cast<Point3*>(o + 148) = *reinterpret_cast<Point3*>(o + 16);
        lightUpdatePosition__FP5Light(o + 100);
    }
    const int end = sword(o, 84);
    __asm__("" : "+r"(o));
    if (!(frameNm < end) && o != nullptr) call_entry(o, 8, 3);
}
}
