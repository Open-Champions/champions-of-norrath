// Observed __6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf at 0x149250, 488 bytes. This is the
// PhProp overload that takes a model header and a texture. Arguments: the object, a position P (by value), a
// model header, a texture, a long N, an int F (flags), an item description (may be null) and a float S.
// Returns nothing. Steps, in order:
//  1. The Prop constructor taking a model header and a texture (__4PropG6Point3P12_modelHeaderP8_textureli) runs
//     with (object, P, model, texture, N, F). F is passed unchanged.
//  2. Word +72 = _vt$6PhProp and the description-area defaults are set as in the other two PhProp constructors
//     (halfwords at +364, +372, +380, +388 = -1, bytes +360/+361 = -1, eight halfwords at +344..+358 = 0).
//  3. Bit 0x4000 is set in the word at +12 and float +576 = S. A non-null description is copied (72 bytes) to +328,
//     byte +568 = 1, bit 0x100000 set and 0x800 cleared in the word at +12.
//  4. Byte +570 = 0 and word +564 = 0. Unlike the other overloads, the word at +556 is not touched here.
//  5. initStuff__6PhPropi(object, F) runs. With byte +568 non-zero and any of the bits 0x19 in F, the word at +68 =
//     word +68 * 0x19660d + 0x3c6ef35f, the sound "game_swish_chest<n>.anm" (n = (word +68 >> 16) % 3 + 1) is looked
//     up in "fx.lmp" and played on the object at volume 1.0. Unlike the other overloads, bit 0x2000 is not set.
// Unknown: what the description area, +564 and +570 mean. Nothing here ran on the EE.
#include "../w11-ctor-a-common/phprop.hpp"

extern "C" {
void prop_construct_texture(void* self, Point3 position, void* model, void* texture, long n, int flags)
    __asm__("__4PropG6Point3P12_modelHeaderP8_textureli");
void __6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf(void* self, Point3 position, void* model, void* texture, long n, int flags,
                                                                       void* description, float scale);
}

extern "C" void __6PhPropG6Point3P12_modelHeaderP8_textureliPQ24item11descriptionf(void* self, Point3 position, void* model, void* texture, long n, int flags,
                                                                                  void* description, float scale) {
    using namespace w11;
    prop_construct_texture(self, position, model, texture, n, flags);
    phprop_defaults(self);
    u32(self, 12) |= 0x4000u;
    f32(self, 576) = scale;
    phprop_description(self, description);
    u8(self, 570) = 0;
    u32(self, 564) = 0;
    initStuff__6PhPropi(self, flags);
    if (static_cast<signed char>(u8(self, 568)) != 0 && (flags & 0x19) != 0) {
        char name[64];
        u32(self, 68) = u32(self, 68) * 0x19660du + 0x3c6ef35fu;
        sprintf(name, "game_swish_chest%d.anm", static_cast<int>((u32(self, 68) >> 16) % 3u) + 1);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), name), self, 1.0f);
    }
}
