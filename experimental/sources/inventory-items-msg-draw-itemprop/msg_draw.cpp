// Observed msg_draw__8ItemProp at 0x14c1f0, 544 bytes.
// The item description is at object+328; its first word points to a record that
// has a 64-bit flag value at +40. Steps, in order:
//  1. When bit 0 of the halfword at object+320 is clear, matMakeTransYaw runs
//     with the matrix at object+272, a stack copy of the 12 bytes at object+16,
//     and the signed halfword at object+56 plus 16384 truncated to 16 bits.
//  2. Halo. If frameNm - 3 < the word at object+260, or bit 39 of the 64-bit
//     value at record+40 is set, byte 3 of the 8-byte halo record at 0x483c30
//     (halos.1292: bytes ff ff ff ff, then the float 409.0) becomes
//     (int)(|isin(frameNm << 9)| * 12.0f + 12.0f) and modelSetHalo(record, 1) is
//     called. Otherwise setupHalo(object+328, 5.0f) is called.
//  3. When bit 28 of the 64-bit value at object+336 is set and the low byte of
//     frameNm is below 30: with n = 15 - |byte - 15|, s = ((float)n * 0.0666667f)
//     squared, P_FaceSprite is called with a stack copy of the point at
//     object+16, 10.0f, s * 6000.0f, 128, 128, 128, (int)(s * 64.0f), the word
//     haloTexture1, (short)((frameNm & 63) << 10), and 3.
//  4. modelDraw(model word at +76, texture word at +80, 1, a stack copy of the
//     point at object+16, the matrix at object+272, 0, the 64-bit value at
//     object+232, 0, 0), then modelResetOptionalParams.
// Constants 0x3d888888 and 0x45bb8000 are the image's single-precision words.
// Unknown: what the flag bits, the halo record and the sprite arguments mean.
// The 64-bit value at object+232 is passed whole, as the image loads it.

#include "../inventory-items-object/object.hpp"
#include "../inventory-items-record/record.hpp"

extern "C" {
extern int frameNm;
extern void* haloTexture1;
float isin__Fi(int angle);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
void modelSetHalo__FP7HaloDefi(void* halo, int mode);
void setupHalo__4itemPQ24item11descriptionf(void* description, float scale);
void P_FaceSprite__FG6Point3ffiiiiP8_texturesi(const void* point, float first, float second, int red, int green,
                                               int blue, int alpha, void* texture, short angle, int mode);
int modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
    void* model, void* texture, int mode, const void* point, void* matrix, void* animation,
    unsigned long long flags, void* draw_record, int last);
void modelResetOptionalParams__Fv();
}

namespace {
struct HaloRecord {
    unsigned char color[4];
    float size;
};
HaloRecord halo = {{255, 255, 255, 255}, 409.0f};
}

extern "C" void msg_draw__8ItemProp(void* object_storage) {
    using namespace inventory_object;
    unsigned char* object = static_cast<unsigned char*>(object_storage);
    unsigned char point[12];
    if ((object[320] & 1u) == 0u) {
        copy_bytes(point, object + 16, 12);
        const unsigned half = unsigned(object[56]) | (unsigned(object[57]) << 8);
        matMakeTransYaw__FP8Matrix34G6Point3s(object + 272, point, static_cast<short>(half + 16384u));
    }
    const int now = frameNm;
    const unsigned char* record =
        reinterpret_cast<const unsigned char*>(inventory_items::resolve<unsigned char>(read_word(object + 328)));
    if (now - 3 < static_cast<int>(read_word(object + 260)) || (record[44] & 0x80u) != 0u) {
        float level = isin__Fi(now << 9);
        if (level < 0.0f) level = -level;
        halo.color[3] = static_cast<unsigned char>(static_cast<int>(level * 12.0f + 12.0f));
        modelSetHalo__FP7HaloDefi(&halo, 1);
    } else {
        setupHalo__4itemPQ24item11descriptionf(object + 328, 5.0f);
    }
    if ((object[339] & 0x10u) != 0u) {
        const unsigned byte = static_cast<unsigned>(frameNm) & 0xffu;
        if (static_cast<int>(byte) < 30) {
            int distance = static_cast<int>(byte) - 15;
            if (distance < 0) distance = -distance;
            float scale = static_cast<float>(15 - distance) * __builtin_bit_cast(float, 0x3d888888u);
            scale = scale * scale;
            unsigned char sprite_point[12];
            copy_bytes(sprite_point, object + 16, 12);
            P_FaceSprite__FG6Point3ffiiiiP8_texturesi(
                sprite_point, 10.0f, scale * __builtin_bit_cast(float, 0x45bb8000u), 128, 128, 128,
                static_cast<int>(scale * 64.0f), haloTexture1,
                static_cast<short>((static_cast<unsigned>(frameNm) & 0x3fu) << 10), 3);
        }
    }
    unsigned char draw_point[12];
    copy_bytes(draw_point, object + 16, 12);
    unsigned long long flags = read_word(object + 232) | (static_cast<unsigned long long>(read_word(object + 236)) << 32);
    modelDraw__FP12_modelHeaderP8_textureiG6Point3P8Matrix34P14AnimationStateUlP11_drawRecordi(
        reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(read_word(object + 76))),
        reinterpret_cast<void*>(static_cast<__UINTPTR_TYPE__>(read_word(object + 80))), 1, draw_point, object + 272,
        nullptr, flags, nullptr, 0);
    modelResetOptionalParams__Fv();
}
