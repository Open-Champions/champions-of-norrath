// Observed msg_draw__4Gold at 0x14ad38, 1160 bytes.
// A zero word at object+128 (the coin count) returns at once. Otherwise, in order:
//  1. matMakeTransYaw on a local matrix, a stack copy of the 12 bytes at
//     object+16, and the signed halfword at object+56 unchanged.
//  2. Model: with n = min(count, 62), modelDraw(get(goldModel), get(goldTex), 1,
//     a stack copy of the point, the local matrix, the animation state at
//     object+88, the 64-bit mask (1 << ((n + 1) & 63)) - 1, 0, 0). get is
//     ResourceMap::get on the 16-byte objects at 0x600d30 and 0x600d40.
//  3. Main sprite: m = min(count, 50); alpha = max(0, min(min(count + 4, 25),
//     frameNm - word(object+80) - 20)); P_FaceSprite(point copy, 12.0f,
//     (float)(114*m + 6900), 255, 200, 50, alpha, haloTexture2, 0, 2).
//  4. When count < 11, return. Otherwise five sparkles i = 0..4 are drawn from the
//     points at object+132+12*i. Each takes age = (frameNm + 12*i) % 60. When age
//     is 59, the slot is respawned: the generator seed word at object+68 steps
//     with seed = seed * 1664525 + 1013904223; three steps give
//     (float)(seed >> 16) / 65536 * 2 - 1 as x, y and z, repeated until
//     x*x + y*y + z*z <= 1; the final seed is stored; the slot becomes
//     (object.x + 8x, object.y + 8y, object.z + 8z + 8) and the age counts as 0.
//     The sprite is P_FaceSprite(slot copy, 12.0f, (float)(int)((1.0f - d *
//     0.0333333f) * 2000.0f), 255, 200, 50, 2 * alpha, haloTexture1, (short)
//     ((frameNm << 12) + (i << 15)), 2) with d = |age - 30|.
// The single-precision words 0x3d088888 and the arithmetic order are the
// image's. Unknown: what the seed, the mask and the stamp at +80 mean.

#include "../inventory-items-object/object.hpp"

extern "C" {
extern int frameNm;
extern void* haloTexture1;
extern void* haloTexture2;
extern unsigned char goldModel[];
extern unsigned char goldTex[];
void* get__11ResourceMap(void* map);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* point, float first, float second, int red, int green,
                                               int blue, int alpha, void* texture, short angle, int mode);
}

namespace {
struct Vec {
    float x, y, z;
};
float random_unit(unsigned& seed) {
    seed = seed * 1664525u + 1013904223u;
    const float unit = static_cast<float>(seed >> 16) * __builtin_bit_cast(float, 0x37800000u);
    return (unit + unit) - 1.0f;
}
}

extern "C" void msg_draw__4Gold(void* object_storage) {
    using namespace inventory_object;
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    const int count = static_cast<int>(read_word(object + 128));
    if (count == 0) return;
    unsigned char matrix[64];
    unsigned char point[12];
    copy_bytes(point, object + 16, 12);
    const unsigned half = unsigned(object[56]) | (unsigned(object[57]) << 8);
    matMakeTransYaw__FP8Matrix34G6Point3s(matrix, point, static_cast<short>(half));
    const int limit = count < 63 ? count : 62;
    void* model = get__11ResourceMap(goldModel);
    void* texture = get__11ResourceMap(goldTex);
    unsigned char draw_point[12];
    copy_bytes(draw_point, object + 16, 12);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        model, texture, 1, draw_point, matrix, object + 88, (1ull << ((limit + 1) & 63)) - 1ull, nullptr, 0);

    const int capped = count < 50 ? count : 50;
    int alpha = count + 4 < 26 ? count + 4 : 25;
    const int fade = frameNm - static_cast<int>(read_word(object + 80)) - 20;
    if (fade < alpha) alpha = fade;
    if (alpha < 0) alpha = 0;
    unsigned char sprite_point[12];
    copy_bytes(sprite_point, object + 16, 12);
    P_FaceSprite__FG6Point3ffiiiiP8_texturesi(sprite_point, 12.0f, static_cast<float>(114 * capped + 6900), 255, 200,
                                              50, alpha, haloTexture2, 0, 2);
    if (count < 11) return;

    const int double_alpha = alpha * 2;
    for (int i = 0; i < 5; ++i) {
        unsigned char* slot = object + 132 + 12 * i;
        int age = (frameNm + 12 * i) % 60;
        if (age == 59) {
            unsigned seed = read_word(object + 68);
            float x, y, z;
            do {
                x = random_unit(seed);
                y = random_unit(seed);
                z = random_unit(seed);
            } while (1.0f < x * x + y * y + z * z);
            write_word(object + 68, seed);
            const Vec origin = {__builtin_bit_cast(float, read_word(object + 16)),
                                __builtin_bit_cast(float, read_word(object + 20)),
                                __builtin_bit_cast(float, read_word(object + 24))};
            const Vec spawn = {origin.x + x * 8.0f + 0.0f, origin.y + y * 8.0f + 0.0f, origin.z + z * 8.0f + 8.0f};
            write_word(slot, __builtin_bit_cast(unsigned, spawn.x));
            write_word(slot + 4, __builtin_bit_cast(unsigned, spawn.y));
            write_word(slot + 8, __builtin_bit_cast(unsigned, spawn.z));
            age = 0;
        }
        int distance = age - 30;
        if (distance < 0) distance = -distance;
        const float fade_size =
            static_cast<float>(static_cast<int>((1.0f - static_cast<float>(distance) * __builtin_bit_cast(float, 0x3d088888u)) * 2000.0f));
        unsigned char slot_point[12];
        copy_bytes(slot_point, slot, 12);
        P_FaceSprite__FG6Point3ffiiiiP8_texturesi(
            slot_point, 12.0f, fade_size, 255, 200, 50, double_alpha, haloTexture1,
            static_cast<short>((static_cast<unsigned>(frameNm) << 12) + (static_cast<unsigned>(i) << 15)), 2);
    }
}
