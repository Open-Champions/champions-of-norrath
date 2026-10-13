// Observed msg_draw__14HateProjectile at 0x001fe670, 284 bytes. O is the object and P the point at O+16.
// frameNm is the global frame counter. Three pool constants are read from the image, none has a symbol: K1 = bits
// 0x3d088888 (1/30), K2 = bits 0x461c4000 (10000.0) and K3 = 0.25. Steps in order:
//  1. u = (float)(the signed word at O+88 - frameNm + 180) * K1, limited to at most 1.0.
//  2. s = isin__Fi((frameNm % 32768) * 2701) (a float; the remainder is the C one, negative for negative frameNm).
//  3. a = u * K2; b = (float)(int)(a * K3); size = (float)(int)(a + s * b).
//  4. P_FaceSprite__FG6Point3ffiiiiP8_texturesi runs with a copy of P, 0.0, size, 80, 0, 0, (int)(u * 128.0),
//     haloTexture5, (short)(frameNm << 10) and 1.
// Inferred: a fading, pulsing halo sprite drawn at the projectile (same shape as the ShockProjectile draw with other
// constants); argument roles come from the mangled name. Unknown: what O+88 holds and what the integer arguments of
// P_FaceSprite mean. Nothing here ran on the EE.
#include "../w12-actor-spell-c-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* haloTexture5;
float isin__Fi(int angle);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 p, float a, float b, int c, int d, int e, int f, void* texture, short g, int h);

void msg_draw__14HateProjectile(void* self) {
    using namespace w12c;
    unsigned char* const o = static_cast<unsigned char*>(self);
    float u = static_cast<float>(sword(o, 88) - frameNm + 180) * bits(0x3d088888u);
    if (1.0f < u) u = 1.0f;
    const float s = isin__Fi((frameNm % 32768) * 2701);
    const float a = u * bits(0x461c4000u);
    const float b = static_cast<float>(static_cast<int>(a * bits(0x3e800000u)));
    const float size = static_cast<float>(static_cast<int>(a + s * b));
    Point3 position = *reinterpret_cast<Point3*>(o + 16);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(position, 0.0f, size, 80, 0, 0, static_cast<int>(u * 128.0f), haloTexture5,
                                              static_cast<short>(frameNm << 10), 1);
}
}
