// Observed gameRadialDamage__FP10GameObjectG6Point3iffiii at 0x001d0698, 616 bytes. Arguments by the mangled name: a source object S,
// a centre C (Point3 by value), an integer A, floats R0 and R1, and integers F, M and X. Returns nothing. In order:
//  1. objectFindInBox runs with the corners C - R1 and C + R1 (all three components), a 128-word list on the frame and the
//     capacity 128; the list is walked until its first zero word.
//  2. A scale is 100.0, or, when 1.0 < R1 - R0, float(A) / (R1 - R0).
//  3. For each listed object O (word at O+12 = its flags, here W): when bit 0x20 of W is set, O is skipped if F has bit 0x08000000
//     set and the signed byte at O+172 is not 16; when bit 0x20 is clear, O is skipped if F has bit 0x08000000 set. Then O is skipped
//     if it is S, or if W & X is nonzero.
//  4. With dx and dy the differences between O's first two position floats (at +16, +20) and C's first two, the distance is
//     |dx| + |dy| - 0.333 * min(|dx|, |dy|) - the float at O+28, where 0.333 is the single 0x3eaa7ef9 read from .lit4. O is skipped
//     when R1 <= distance.
//  5. Otherwise the amount is (R1 - distance) * scale, raised to float(M) when smaller, limited to float(A), truncated to an
//     integer and converted back to a float; the table entry at byte 16 of the table at O+72 is then called on O (adjusted by the
//     entry's signed halfword) with (amount, S, F).
// Inferred: a falloff-weighted area hit with an octagonal distance approximation and the entry at 16 being a hurt call.
// Unknown: the meaning of F (only bit 0x08000000 is tested here), X, and the byte at O+172. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, unsigned* out, int capacity);
void gameRadialDamage__FP10GameObjectG6Point3iffiii(void* source, Point3 center, int a, float r0, float r1, int f, int m, int x);
}

extern "C" void gameRadialDamage__FP10GameObjectG6Point3iffiii(void* source, Point3 center, int a, float r0, float r1, int f, int m, int x) {
    using namespace w14pa;
    Point3 low;
    low.x = center.x - r1;
    low.y = center.y - r1;
    low.z = center.z - r1;
    Point3 high;
    high.x = center.x + r1;
    high.y = center.y + r1;
    high.z = center.z + r1;
    unsigned list[128];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, list, 128);
    const float limit = static_cast<float>(a);
    const float span = r1 - r0;
    float scale = 100.0f;
    if (1.0f < span) scale = limit / span;
    for (int i = 0; list[i] != 0; ++i) {
        unsigned char* const o = from_word(list[i]);
        const unsigned flags = word(o, 12);
        const bool filtered = (f & 0x08000000) != 0;
        if ((flags & 0x20) != 0) {
            if (filtered && static_cast<signed char>(o[172]) != 16) continue;
        } else if (filtered) {
            continue;
        }
        if (o == source) continue;
        if ((flags & static_cast<unsigned>(x)) != 0) continue;
        const float dx = real(o, 16) - center.x;
        const float dy = real(o, 20) - center.y;
        const float ax = __builtin_fabsf(dx);
        const float ay = __builtin_fabsf(dy);
        const float distance = (ax + ay) - __builtin_fminf(ax, ay) * bits(0x3eaa7ef9u) - real(o, 28);
        if (r1 <= distance) continue;
        float amount = (r1 - distance) * scale;
        const float floor_value = static_cast<float>(m);
        if (amount < floor_value) amount = floor_value;
        amount = __builtin_fminf(amount, limit);
        call_hurt(o, static_cast<float>(static_cast<int>(amount)), source, f);
    }
}
