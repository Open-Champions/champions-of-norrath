// Observed __12GlowingShaftG6Point3iiii at 0x001e6910, 180 bytes. Arguments: the object, a position P (by value), then four
// integers A, B, C, D. Returns nothing. Steps, in order:
//  1. The GameObject constructor runs on the object with P, a null resource map and a null fourth argument.
//  2. The word at +72 becomes _vt$12GlowingShaft, the word at +12 gets bits 0x4009 set, A is stored in the words at +76 and +80,
//     B in the halfword at +84, C in the halfword at +86 and D in the halfword at +88.
//  3. objectAddToRunList runs on the object, then byte +40 becomes 48.
// Unknown: the meaning of A..D (the Hammer of Wrath cast path passes 90, 86, 126, 250) and of byte +40. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern unsigned char vt_GlowingShaft[] __asm__("_vt$12GlowingShaft");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void objectAddToRunList__FP10GameObject(void* object);
void __12GlowingShaftG6Point3iiii(void* self, Point3 position, int a, int b, int c, int d);
}

extern "C" void __12GlowingShaftG6Point3iiii(void* self, Point3 position, int a, int b, int c, int d) {
    using namespace w14pa;
    unsigned char* o = static_cast<unsigned char*>(self);
    game_object_construct(self, position, nullptr, nullptr);
    word(o, 80) = static_cast<unsigned>(a);
    word(o, 72) = to_word(vt_GlowingShaft);
    word(o, 12) |= 0x4009u;
    half(o, 84) = static_cast<short>(b);
    half(o, 86) = static_cast<short>(c);
    half(o, 88) = static_cast<short>(d);
    word(o, 76) = static_cast<unsigned>(a);
    objectAddToRunList__FP10GameObject(self);
    o[40] = 48;
}
