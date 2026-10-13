// Observed msg_run__15GlowingCylinder at 0x001d8f50, 460 bytes. O is the object; n is the signed word at +76
// (a countdown), the words at +88 and +92 are two trail ids (inferred from where they are passed:
// P_ExtendTrail__FiG6Point3 and P_SetTrailColor__Fiiiii). Steps in order, with A = frameNm << 11:
//  1. Point P = (O.x + icos(A) * 20, O.y + isin(A) * 20, O.z + float((70 - n) * 2)), with (x, y, z) the floats at
//     +16, +20, +24. P_ExtendTrail(word at +88, P) is called.
//  2. Point Q = (O.x - icos(A) * 20, O.y - isin(A) * 20, z of P) and P_ExtendTrail(word at +92, Q) is called
//     (frameNm is read again before each trig call).
//  3. With n read again: when n < 35, f = float(n) * k (k is the global float at gp - 29188, stored bits
//     0x3cea0ea0 = 1/35 rounded; no symbol names it) and the four ints are (int)(f * 86), (int)(f * 126),
//     (int)(f * 250), (int)(f * 200) = (c1, c2, c3, c4). P_SetTrailColor(word +88, c1, c2, c3, c4) and then
//     P_SetTrailColor(word +92, c1, c2, c3, c4) are called.
//  4. The word at +76 (read again) minus one is stored back. When n - 1 is zero or less, slot byte 8
//     of the table at +72 is called on the object adjusted by its halfword with the argument 3.
// Inferred: this is the run step of a short-lived glowing cylinder with two fading trails; the slot at byte 8
// with argument 3 is the object's delete request (the Vehicle table's slot there is the destructor, see the
// prop-poke-clone notes). Unknown: the meaning of the point values. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
void P_SetTrailColor__Fiiiii(int trail, int a, int b, int c, int d);

void msg_run__15GlowingCylinder(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const float twenty = 20.0f;
    Point3 p;
    p.x = real(o, 16) + icos__Fi(frameNm << 11) * twenty;
    p.y = real(o, 20) + isin__Fi(frameNm << 11) * twenty;
    p.z = real(o, 24) + static_cast<float>((70 - sword(o, 76)) * 2);
    P_ExtendTrail__FiG6Point3(sword(o, 88), p);
    p.x = real(o, 16) - icos__Fi(frameNm << 11) * twenty;
    p.y = real(o, 20) - isin__Fi(frameNm << 11) * twenty;
    P_ExtendTrail__FiG6Point3(sword(o, 92), p);
    const int n = sword(o, 76);
    if (n < 35) {
        const float f = static_cast<float>(n) * bits(0x3cea0ea0u);
        const int c4 = static_cast<int>(f * 200.0f);
        const int c1 = static_cast<int>(f * 86.0f);
        const int c2 = static_cast<int>(f * 126.0f);
        const int c3 = static_cast<int>(f * 250.0f);
        P_SetTrailColor__Fiiiii(sword(o, 88), c1, c2, c3, c4);
        P_SetTrailColor__Fiiiii(sword(o, 92), c1, c2, c3, c4);
    }
    const int next = sword(o, 76) - 1;
    sword(o, 76) = next;
    if (next <= 0) call_entry(o, 8, 3);
}
}
