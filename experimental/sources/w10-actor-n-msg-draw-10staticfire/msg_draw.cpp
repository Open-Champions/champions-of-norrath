// Observed msg_draw__10StaticFire at 0x0028eb58, 1180 bytes. o is the object, t the word at +124,
// r the float at +100. m = (strstr(levelName + 81, "kel") != 0) is taken first, then
// gamePositionVisible(point +16, 941.0). Steps in order:
//  1. When the point is visible, the signed byte at +110 is non-zero, t is non-zero and the byte at
//     +109 is zero: one sprite at (x, y, z + 12) of size 26.0 with the second size
//     (float(+96) * 59.0 + 12000.0) * r, colour components (185, 104, 81), 110, texture
//     haloTexture2, short 0 and last argument 6 (P_FaceSprite).
//  2. The ring of 10-byte records (base = word at +1188) from the index in the halfword at +122 up to
//     the index in the halfword at +120, wrapping to 0 at the halfword at +118. For each record
//     (three halfwords a, b, c; the sprite position is the object point plus (a, b, c) / 16):
//       - skipped unless that position is visible (941.0);
//       - v = min(float(c) * float(+92), 1.0); when the signed byte at +111 is non-zero w = min(v + v,
//         1.0), otherwise w = v;
//       - colour: with t and the byte at +110 both non-zero, base = w * (-46, -43, -42) is blended
//         towards (188, 83, 69) by 1 - min((float(c) / 16) / (float(+96) * r), 1) (each target
//         minus (base + 56)), and 56 is added; otherwise ((w * (-46, -43, -42) + 56) * 0.5);
//       - when m holds the colour is pulled towards (70, 102, 107) by u = clamp((z + 480) *
//         0.0020876825, 0, 1) (c' = (c - target) * u + target), and the texture choice flag is
//         u < 0.99698496;
//       - a sprite is drawn at that position: size 5.0, second size (float(+104) - 4800) * w + 4800,
//         colour components (int)r', (int)g', (int)b', and (int)(v * -171 + 171), texture haloTexture8
//         when the flag is set else haloTexture7, a short spin of +/- (k mod 65536) with k =
//         (object address word + frameNm) * 179 (negated for odd indices), last argument 1.
// P_FaceSprite arguments after the position are named by position only; the meaning of the
// numbers is unknown. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
extern char levelName[];
extern int frameNm;
extern void* haloTexture2;
extern void* haloTexture7;
extern void* haloTexture8;
char* strstr(const char* haystack, const char* needle);
int gamePositionVisible__FP6Point3f(Point3* position, float radius);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(Point3 position, float size, float size2, int a, int b, int c, int d, void* texture, short spin, int flag);

void msg_draw__10StaticFire(void* self) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int index = half(o, 122);
    const bool matched = strstr(levelName + 81, "kel") != nullptr;
    if (gamePositionVisible__FP6Point3f(reinterpret_cast<Point3*>(o + 16), bits(0x446b4000u)) != 0 &&
        static_cast<signed char>(o[110]) != 0 && word(o, 124) != 0u && static_cast<signed char>(o[109]) == 0) {
        const float zero = 0.0f;
        const float lift = 12.0f;
        const float second = (real(o, 96) * 59.0f + bits(0x463b8000u)) * real(o, 100);
        Point3 position;
        position.x = real(o, 16) + zero;
        position.y = real(o, 20);
        position.z = real(o, 24) + lift;
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(position, 26.0f, second, 185, 104, 81, 110, haloTexture2, 0, 6);
    }
    if (index == half(o, 120)) return;
    do {
        unsigned char* const record = from_word(word(o, 1188)) + index * 10;
        Point3 position;
        position.x = real(o, 16) + static_cast<float>(half(record, 0)) * 0.0625f;
        position.y = real(o, 20) + static_cast<float>(half(record, 2)) * 0.0625f;
        position.z = real(o, 24) + static_cast<float>(half(record, 4)) * 0.0625f;
        if (gamePositionVisible__FP6Point3f(&position, bits(0x446b4000u)) != 0) {
            float raw = static_cast<float>(half(record, 4)) * real(o, 92);
            if (1.0f < raw) raw = 1.0f;
            float w = raw;
            if (static_cast<signed char>(o[111]) != 0) {
                w = raw + raw;
                if (1.0f < w) w = 1.0f;
            }
            float r, g, b;
            if (word(o, 124) != 0u && static_cast<signed char>(o[110]) != 0) {
                float br = w * -46.0f;
                float bg = w * -43.0f;
                float bb = w * -42.0f;
                const float span = real(o, 96) * real(o, 100);
                float share = (static_cast<float>(half(record, 4)) * 0.0625f) / span;
                if (1.0f < share) share = 1.0f;
                const float keep = 1.0f - share;
                const float dr = 188.0f - (br + 56.0f);
                const float dg = 83.0f - (bg + 56.0f);
                const float db = 69.0f - (bb + 56.0f);
                br = br + dr * keep;
                bg = bg + dg * keep;
                bb = bb + db * keep;
                r = br + 56.0f;
                g = bg + 56.0f;
                b = bb + 56.0f;
            } else {
                r = (w * -46.0f + 56.0f) * 0.5f;
                g = (w * -43.0f + 56.0f) * 0.5f;
                b = (w * -42.0f + 56.0f) * 0.5f;
            }
            int cool = 0;
            if (matched) {
                float u = (position.z - -480.0f) * bits(0x3b08d180u);
                if (1.0f < u) u = 1.0f;
                if (u < 0.0f) u = 0.0f;
                r = (r - 70.0f) * u + 70.0f;
                g = (g - 102.0f) * u + 102.0f;
                b = (b - 107.0f) * u + 107.0f;
                if (u < bits(0x3f7f3a68u)) cool = 1;
            }
            int mix = static_cast<int>(to_word(o)) + frameNm;
            mix = mix * 179;
            const int high = (mix < 0 ? mix + 0xffff : mix) >> 16;
            int spin = mix - (high << 16);
            if ((index & 1) != 0) spin = -spin;
            const float second = (real(o, 104) - 4800.0f) * w + 4800.0f;
            P_FaceSprite__FG6Point3ffiiiiP8_texturesi(position, 5.0f, second, static_cast<int>(r), static_cast<int>(g), static_cast<int>(b),
                                                       static_cast<int>(raw * -171.0f + 171.0f), cool != 0 ? haloTexture8 : haloTexture7, static_cast<short>(spin), 1);
        }
        ++index;
        if (index == half(o, 118)) index = 0;
    } while (index != half(o, 120));
}
}
