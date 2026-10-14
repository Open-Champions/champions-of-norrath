// Observed __4PropG6Point3P12_modelHeaderP8_textureli at 0x001478b0, 404 bytes. Arguments: the object, a position P (by value), a model
// header M, a texture T, a 64-bit value L and an integer F. This is the Prop constructor overload that takes resources directly (the
// other overload at 0x001476b8 looks them up by name). Returns nothing. In order:
//  1. The GameObject constructor runs on the object with P, a null resource map and a null fourth argument.
//  2. _vt$4Prop goes to +72. The words +76 and +80 become M and T. L is stored as 8 bytes at +232.
//  3. The word at +68 is stepped as w * 0x19660d + 0x3c6ef35f (32-bit wrap) and the high 16 bits of the result go to the halfword
//     at +56. The singles at +48, +252 and +256 and the halfword at +268 are cleared.
//  4. modelGetBoundingBox__FP12_modelHeaderlPiN52 is called with (M, L, and the addresses of six ints b[0]..b[5]). The singles at
//     +240, +244 and +248 get the arithmetic-shift centres (b[0]+b[3])>>1, (b[1]+b[4])>>1 and (b[2]+b[5])>>1, and the single at
//     +264 gets the third centre minus float(b[2]).
//  5. When bit 2 (value 4) of F is set, the three centres are added to the singles at +16, +20 and +24 and objectUpdateInGrid runs.
//  6. init__4Propi is called with the object and F.
// Inferred: b is a bounding box (minimum and maximum corners) from the name of the callee. Unknown: the meaning of L and F beyond the
// bit tested here, and the fields. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
void game_object_construct(void* self, Point3 position, void* map, char** names)
    __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void modelGetBoundingBox__FP12_modelHeaderlPiN52(void* header, long long l, int* a, int* b, int* c, int* d, int* e, int* f);
void objectUpdateInGrid__FP10GameObject(void* object);
void init__4Propi(void* object, int flags);
extern unsigned char vt_Prop[] __asm__("_vt$4Prop");
void construct(void* self, Point3 position, void* model, void* texture, long long value, int flags)
    __asm__("__4PropG6Point3P12_modelHeaderP8_textureli");
}

extern "C" void construct(void* self, Point3 position, void* model, void* texture, long long value, int flags) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    game_object_construct(self, position, nullptr, nullptr);
    word(o, 68) = word(o, 68) * 0x19660du + 0x3c6ef35fu;
    word(o, 72) = to_word(vt_Prop);
    real(o, 48) = 0.0f;
    real(o, 256) = 0.0f;
    real(o, 252) = 0.0f;
    half(o, 56) = static_cast<short>(word(o, 68) >> 16);
    word(o, 80) = to_word(texture);
    word(o, 76) = to_word(model);
    dword(o, 232) = static_cast<unsigned long long>(value);
    half(o, 268) = 0;
    int b[6];
    modelGetBoundingBox__FP12_modelHeaderlPiN52(model, value, &b[0], &b[1], &b[2], &b[3], &b[4], &b[5]);
    const float cz = static_cast<float>((b[2] + b[5]) >> 1);
    const float cx = static_cast<float>((b[0] + b[3]) >> 1);
    const float cy = static_cast<float>((b[1] + b[4]) >> 1);
    real(o, 248) = cz;
    real(o, 240) = cx;
    real(o, 244) = cy;
    real(o, 264) = cz - static_cast<float>(b[2]);
    if ((flags & 4) != 0) {
        real(o, 16) += cx;
        real(o, 20) += cy;
        real(o, 24) += cz;
        objectUpdateInGrid__FP10GameObject(self);
    }
    init__4Propi(self, flags);
}
