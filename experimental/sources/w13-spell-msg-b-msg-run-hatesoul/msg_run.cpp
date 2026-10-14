// Observed msg_run__8HateSoul at 0x001fecf8, 980 bytes. O is the object; age = frameNm - the signed word at O+84 (frameNm
// is the global frame counter); H = the signed halfword at O+56; W = the signed word at O+160; T = the signed word at
// O+168 (a trail handle). "entry 8" is the entry at table byte 8 of the table whose address is the word at O+72, called on O.
//  1. The vector at O+88 (three floats) is chosen: when age is below 15, P_SetTrailWidth__Fii(T, (int)(float(age) *
//     0.06666666 * 700.0 + 35.0)) runs and the vector is (icos__Fi(H) * 540.0, isin__Fi(H) * 540.0, 50.0). Otherwise H
//     changes first: with W zero it becomes (short)(H + r % 500 + 500), else (short)(H - r % 500 - 500), r =
//     eRand__10GameObject(O) taken as unsigned; then the same vector is built from the new H.
//  2. move__10ProjectileG6Point3(O, copy of the vector at O+88) runs; its result is ignored.
//  3. A = (the float at O+92, -(the float at O+88), 0.0) is normalised with the EE reciprocal square root (1.0 / sqrt of
//     x*x + y*y + z*z, summed in that order) and scaled by icos__Fi(frameNm * 6880) * 0.754 (the float 0x3f410624);
//     B = (0, 0, 1.0) is scaled by isin__Fi(frameNm * 6880) * 0.754; P = A + B + (the three floats at O+16).
//     P_ExtendTrail__FiG6Point3(T, P) runs.
//  4. When the word at O+164 is non-zero, the three floats at O+16 are copied to O+148 and lightUpdatePosition__FP5Light(O+100)
//     is called.
//  5. When age is 601 or more, entry 8 is called with 3.
// The image tests O for null before the entry 8 call; that test cannot be true here and is not reproduced.
// Inferred: a spirit that moves along a changing heading and trails a wobble of its own. Unknown: what the words at +84,
// +160, +164 and +168 hold exactly. Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern int frameNm;
unsigned eRand__10GameObject(void* self);
float icos__Fi(int angle);
float isin__Fi(int angle);
void P_SetTrailWidth__Fii(int trail, int width);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
int move__10ProjectileG6Point3(void* self, Point3 velocity);
void lightUpdatePosition__FP5Light(void* light);

void msg_run__8HateSoul(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const int age = frameNm - sword(o, 84);
    Point3 heading;
    if (age < 15) {
        P_SetTrailWidth__Fii(sword(o, 168), static_cast<int>(static_cast<float>(age) * bits(0x3d888888u) * 700.0f + 35.0f));
        heading.x = icos__Fi(half(o, 56)) * 540.0f;
        heading.y = isin__Fi(half(o, 56)) * 540.0f;
        heading.z = 50.0f;
    } else {
        if (sword(o, 160) == 0) {
            half(o, 56) = static_cast<short>(static_cast<unsigned>(static_cast<unsigned short>(half(o, 56))) + eRand__10GameObject(o) % 500u + 500u);
        } else {
            half(o, 56) = static_cast<short>(static_cast<unsigned>(static_cast<unsigned short>(half(o, 56))) - eRand__10GameObject(o) % 500u - 500u);
        }
        heading.x = icos__Fi(half(o, 56)) * 540.0f;
        heading.y = isin__Fi(half(o, 56)) * 540.0f;
        heading.z = 50.0f;
    }
    *reinterpret_cast<Point3*>(o + 88) = heading;
    Point3 step = *reinterpret_cast<Point3*>(o + 88);
    move__10ProjectileG6Point3(o, step);
    Point3 a;
    a.x = real(o, 92);
    a.y = -real(o, 88);
    a.z = 0.0f;
    Point3 b;
    b.x = 0.0f;
    b.y = 0.0f;
    b.z = 1.0f;
    const float inverse = inverse_length(a.x * a.x + a.y * a.y + a.z * a.z);
    a.x = a.x * inverse;
    a.y = a.y * inverse;
    a.z = a.z * inverse;
    const float c = icos__Fi(frameNm * 6880) * bits(0x3f410624u);
    a.x = a.x * c;
    a.y = a.y * c;
    a.z = a.z * c;
    const float s = isin__Fi(frameNm * 6880) * bits(0x3f410624u);
    b.x = b.x * s;
    b.y = b.y * s;
    b.z = b.z * s;
    Point3 position;
    position.x = a.x + b.x;
    position.y = a.y + b.y;
    position.z = a.z + b.z;
    position.x = position.x + real(o, 16);
    position.y = position.y + real(o, 20);
    position.z = position.z + real(o, 24);
    P_ExtendTrail__FiG6Point3(sword(o, 168), position);
    if (word(o, 164) != 0u) {
        *reinterpret_cast<Point3*>(o + 148) = *reinterpret_cast<Point3*>(o + 16);
        lightUpdatePosition__FP5Light(o + 100);
    }
    if (frameNm - sword(o, 84) >= 601) call_entry(o, 8, 3);
}
}
