#include "../w11-rend-a-common/rend.hpp"
// Observed drawWorldDynamicLight__FP13PolysetHeader at 0x0017cb18, 884 bytes. The argument is a
// polyset header ph. Words used: ph+0 (a pointer to an info record h), ph+4 and ph+32 (state words
// kept between frames), and the ints at ph+40, ph+44, ph+48 (a position in quarter units).
//  1. p = (float)ph[40] * 0.25f, (float)ph[44] * 0.25f, (float)ph[48] * 0.25f.
//  2. n = getDynamicLightNumber__FP13PolysetHeader(ph); an index of 16 or more (unsigned) returns.
//     Otherwise the light is the 68-byte entry n of DynamicLightArray, offset 8 (light L).
//  3. radius r starts as (int)L[28] (float at +28). Selected by the unsigned halfword h[20]
//     (values above 4 act like 4 without the final skip below):
//       0: r = h[16].
//       1: when ph[32] < frameNm: ph[32] = frameNm + h[22], one generator step
//          (eRandState = eRandState * 0x19660d + 0x3c6ef35f, u = state >> 16) and
//          r = h[16] + (int)((float)(h[18] - h[16]) * ((float)u * 2^-16)).
//       2: when ph[32] < frameNm: ph[32] = frameNm + h[22]; bit 31 of ph[4] toggles: if clear it is set
//          and r = h[18], else it is cleared and r = h[16]. Then if h[22] != 0,
//          r += ((ph[4] bit 31 set ? -1 : 1) * (h[18] - h[16])) / h[22] (truncating).
//       3: when ph[32] < frameNm: ph[32] = frameNm + h[22]; r = (r == h[16]) ? h[18] : h[16].
//       4: r = r unchanged (r from the light).
//  4. Unless h[20] == 4: r is clamped to 0..299, then the light L is updated: bytes of the 12-byte
//     p go to L+48; L[28] = L[36] = (float)r; L[40] = L[32] = 1.0f / ((float)r * (float)r); the word
//     L[44] = 0; halfwords h[24], h[26], h[28], h[30] go to both L+18, +20, +22, +24 and L+10, +12,
//     +14, +16 (signed halfwords stored as halfwords); byte L[8] = 0; then lightAdd__FP5Light(L).
//  5. When the signed byte h[39] != 0 and lightHaloTexture is nonzero: with
//     k = (int)(ef1Noise__Fi(0x7efe41 + (unsigned)L * 0x19660d + (frameNm << 4)) * 22.0f),
//     P_FaceSprite__FG6Point3ffiiiiP8_texturesi(&p, (float)h[40], (float)(h[32] + (k << h[34])),
//     h[35], h[36], h[37], h[38], lightHaloTexture, 0, 2) with the bytes at h+34..h+40 unsigned.
// Nothing is returned.
// Inferred: a world-placed dynamic light with five radius behaviours (fixed, random between two
// bounds, ping-pong, toggle) and an optional halo sprite flickering by noise. Unknown: the
// polyset header and info-record layouts beyond the offsets used, the meaning of each of the
// five modes by name, and whether halfwords h[24..30] are colours. Nothing here was executed
// on the EE.
extern "C" {
extern unsigned char DynamicLightArray[16 * 68];
extern int frameNm;
extern int eRandState;
extern unsigned char* lightHaloTexture;
unsigned getDynamicLightNumber__FP13PolysetHeader(unsigned char* polyset);
void lightAdd__FP5Light(unsigned char* light);
float ef1Noise__Fi(int index);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(float* position, float a, float b, int c, int d, int e, int f,
                                               unsigned char* texture, short g, int h);
void drawWorldDynamicLight__FP13PolysetHeader(unsigned char* ph) {
    using namespace rend;
    float p[3];
    p[0] = static_cast<float>(at<i32>(ph, 40)) * 0.25f;
    p[1] = static_cast<float>(at<i32>(ph, 44)) * 0.25f;
    p[2] = static_cast<float>(at<i32>(ph, 48)) * 0.25f;
    unsigned char* h = ptr(ph, 0);
    const unsigned n = getDynamicLightNumber__FP13PolysetHeader(ph);
    if (n >= 16u) return;
    unsigned char* light = DynamicLightArray + n * 68 + 8;
    int r = static_cast<int>(at<f32>(light, 28));
    switch (at<u16>(h, 20)) {
    case 0:
        r = at<u16>(h, 16);
        break;
    case 1:
        if (at<i32>(ph, 32) < frameNm) {
            at<i32>(ph, 32) = frameNm + at<u16>(h, 22);
            const unsigned s = static_cast<unsigned>(eRandState) * 1664525u + 1013904223u;
            eRandState = static_cast<int>(s);
            const int low = at<u16>(h, 16);
            const int span = at<u16>(h, 18) - low;
            r = low + static_cast<int>(static_cast<float>(span) * (static_cast<float>(static_cast<int>(s >> 16)) * 0x1p-16f));
        }
        break;
    case 2: {
        if (at<i32>(ph, 32) < frameNm) {
            at<i32>(ph, 32) = frameNm + at<u16>(h, 22);
            const int word = at<i32>(ph, 4);
            if (word >= 0) {
                r = at<u16>(h, 18);
                at<i32>(ph, 4) = static_cast<int>(static_cast<unsigned>(word) | 0x80000000u);
            } else {
                r = at<u16>(h, 16);
                at<i32>(ph, 4) = word & 0x7fffffff;
            }
        }
        const int period = at<u16>(h, 22);
        if (period != 0) {
            const int direction = (static_cast<unsigned>(at<i32>(ph, 4)) & 0x80000000u) != 0 ? -1 : 1;
            r += direction * (at<u16>(h, 18) - at<u16>(h, 16)) / period;
        }
        break;
    }
    case 3:
        if (at<i32>(ph, 32) < frameNm) {
            at<i32>(ph, 32) = frameNm + at<u16>(h, 22);
            r = (r == static_cast<int>(at<u16>(h, 16))) ? at<u16>(h, 18) : at<u16>(h, 16);
        }
        break;
    default:
        break;
    }
    if (at<u16>(h, 20) != 4) {
        if (!(r < 300)) r = 299;
        if (r < 0) r = 0;
        const float radius = static_cast<float>(r);
        f32* position = &at<f32>(light, 48);
        position[0] = p[0];
        position[1] = p[1];
        position[2] = p[2];
        const short c0 = at<i16>(h, 24);
        const short c1 = at<i16>(h, 26);
        const short c2 = at<i16>(h, 28);
        const short c3 = at<i16>(h, 30);
        const float inverse = 1.0f / (radius * radius);
        at<i16>(light, 18) = c0;
        at<i16>(light, 20) = c1;
        at<i16>(light, 22) = c2;
        at<i16>(light, 24) = c3;
        at<f32>(light, 28) = radius;
        at<f32>(light, 36) = radius;
        at<i32>(light, 44) = 0;
        at<i16>(light, 10) = c0;
        at<i16>(light, 12) = c1;
        at<i16>(light, 14) = c2;
        at<i16>(light, 16) = c3;
        at<f32>(light, 40) = inverse;
        at<f32>(light, 32) = inverse;
        at<i8>(light, 8) = 0;
        lightAdd__FP5Light(light);
    }
    if (at<i8>(h, 39) == 0) return;
    if (lightHaloTexture == nullptr) return;
    const unsigned address = static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(light));
    const float noise = ef1Noise__Fi(static_cast<int>(0x7efe41u + address * 0x19660du + (static_cast<unsigned>(frameNm) << 4)));
    const int k = static_cast<int>(noise * 22.0f);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(p, static_cast<float>(static_cast<int>(h[40])),
        static_cast<float>(static_cast<int>(at<u16>(h, 32)) + (k << h[34])), h[35], h[36], h[37], h[38],
        lightHaloTexture, 0, 2);
}
}
