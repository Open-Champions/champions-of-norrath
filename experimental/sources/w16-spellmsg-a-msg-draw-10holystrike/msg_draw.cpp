// Observed msg_draw__10HolyStrike at 0x002b27c8, 348 bytes. O is the object and P the point (three singles) at O+16. frameNm is
// the global frame counter. Steps in order:
//  1. A = (the single at O+112, minus the single at O+108, 0.0) is normalised: its squared length is x*x + y*y, then + z*z, and the
//     components are multiplied by rsqrt.s of it (1 / sqrt, the EE instruction).
//  2. The position is ((A + (0, 0, 1)) + P), added component by component in that order.
//  3. s = isin__Fi(frameNm << 10) (a float), size = s * 250.0 + 5000.0 (the pool constant with bits 0x459c4000).
//  4. P_FaceSprite__FG6Point3ffiiiiP8_texturesi runs with the position, 5.0, size, 250, 200, 100, 128, haloTexture5, the halfword
//     (short)(frameNm << 9) and 3.
// Inferred: a pulsing halo sprite placed beside the object, offset along a unit direction taken from two singles of the object
// (the direction (O+112, -O+108) is perpendicular to (O+108, O+112)); argument roles come from the mangled name. Unknown: what the
// singles at O+108 and O+112 and the integer arguments of P_FaceSprite mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* haloTexture5;
float isin__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float a, float b, int c, int d, int e, int f, void* texture, short g, int h);

void msg_draw__10HolyStrike(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    float zero = 0.0f;
    __asm__("" : "+f"(zero));
    Point3 a = {real(o, 112), -real(o, 108), zero};
    const Point3 b = {zero, zero, 1.0f};
    const Point3 p = *reinterpret_cast<Point3*>(o + 16);
    float inv;
    const float one = 1.0f;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(inv) : "f"(one), "f"(a.x * a.x + a.y * a.y + a.z * a.z));
    a.x = a.x * inv;
    a.y = a.y * inv;
    a.z = a.z * inv;
    Point3 position = {a.x + b.x, a.y + b.y, a.z + b.z};
    position.x = position.x + p.x;
    position.y = position.y + p.y;
    position.z = position.z + p.z;
    const float s = isin__Fi(frameNm << 10);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(position, 5.0f, s * 250.0f + bits(0x459c4000u), 250, 200, 100, 128, haloTexture5,
                                              static_cast<short>(frameNm << 9), 3);
}
}
