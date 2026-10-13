// Observed __8ItemPropG6Point3PQ24item11descriptionP8Matrix34 at 0x0014bbc8, 796 bytes. This is the ItemProp overload
// that takes an item description and a placement matrix (not the one taking a description and an int, at 0x0014b9e8).
// Arguments: the object, a position P (by value), an item description D and a matrix M. Returns nothing. Steps, in order:
//  1. item::getProxyMeshId(D) is asked twice. Entries of _4item$globalItem are 16 bytes: the word at entry(first call)+0 is
//     the lump-name word A and the word at entry(second call)+4 is the resource-name word B (both read before the
//     GameObject constructor runs).
//  2. The GameObject constructor runs on the object with P, a null resource map and a null fourth argument. The word at +12
//     is OR-ed with 0x1809 and _vt$4Prop goes to +72, then objectUpdateInGrid runs on the object.
//  3. The float at +28 becomes 4.0, the halfword at +320 becomes 1, and the 48 bytes of M are copied to +272.
//  4. The 13 bytes "equipmnt.lmp" (with its NUL) are copied to +84; strcpy(+100, A) and strcpy(+164, B) run. The word at
//     +76 becomes lumpFindResource__FPcT0(+84, +100) and the word at +80 becomes lumpFindResource__FPcT0(+84, +164). The
//     64-bit value at +232 becomes -1.
//  5. With the halfword at +320 read again: when bit 1 is set objectAddToRunList runs on the object and bit 0x4000 is set
//     in +12; then, after re-reading it, when bit 2 is set bit 0x1000 is set in +12.
//  6. _vt$8ItemProp goes to +72. The halfwords at +388, +380, +372 and +364 become -1, bytes +360 and +361 become -1 and
//     the eight halfwords at +344..+350 and +352..+358 become 0. The 72 bytes of D are copied to +328. The word at +12 is
//     OR-ed with 0x102000 and has bits 0x1800 cleared; when bit 39 of the 64-bit value at +336 is set bits 0x3000 are
//     cleared as well. objectUpdateInGrid runs on the object again.
// Inferred: D is a 72-byte item description as far as the copy goes; the two table words name resources in the lump
// "equipmnt.lmp". Unknown: what the halfword at +320 means (it is 1 when read back, so the branches of step 5 are not
// reachable unless a callee changes it), and the meaning of the copied fields. Nothing here ran on the EE.
#include "../w11-ctor-a-common/phprop.hpp"

extern "C" {
extern unsigned char vt_Prop[] __asm__("_vt$4Prop");
extern unsigned char vt_ItemProp[] __asm__("_vt$8ItemProp");
extern unsigned char globalItem[] __asm__("_4item$globalItem");
int getProxyMeshId__4itemPQ24item11description(void* description);
char* strcpy(char* to, const char* from);
void objectUpdateInGrid__FP10GameObject(void* object);
void __8ItemPropG6Point3PQ24item11descriptionP8Matrix34(void* self, Point3 position, void* description, Matrix34* matrix);
}

extern "C" void __8ItemPropG6Point3PQ24item11descriptionP8Matrix34(void* self, Point3 position, void* description, Matrix34* matrix) {
    using namespace w11;
    const unsigned lump_word = *reinterpret_cast<const unsigned*>(globalItem + (getProxyMeshId__4itemPQ24item11description(description) << 4));
    const unsigned name_word = *reinterpret_cast<const unsigned*>(globalItem + (getProxyMeshId__4itemPQ24item11description(description) << 4) + 4);
    game_object_construct(self, position, nullptr, nullptr);
    u32(self, 72) = word(vt_Prop);
    u32(self, 12) |= 0x1809u;
    objectUpdateInGrid__FP10GameObject(self);
    f32(self, 28) = 4.0f;
    i16(self, 320) = 1;
    *reinterpret_cast<Matrix34*>(&u32(self, 272)) = *matrix;
    char* const base = reinterpret_cast<char*>(static_cast<unsigned char*>(self));
    static const char lump_name[] = "equipmnt.lmp";
    for (int i = 0; i < 13; ++i) base[84 + i] = lump_name[i];
    strcpy(base + 100, static_cast<const char*>(pointer(lump_word)));
    strcpy(base + 164, static_cast<const char*>(pointer(name_word)));
    u32(self, 76) = word(lumpFindResource__FPcT0(base + 84, base + 100));
    u32(self, 80) = word(lumpFindResource__FPcT0(base + 84, base + 164));
    *reinterpret_cast<long long*>(&u32(self, 232)) = -1;
    if (u16(self, 320) & 2u) {
        objectAddToRunList__FP10GameObject(self);
        u32(self, 12) |= 0x4000u;
    }
    if (u16(self, 320) & 4u) u32(self, 12) |= 0x1000u;
    u32(self, 72) = word(vt_ItemProp);
    for (int i = 3; i >= 0; i--) i16(self, 364 + 8 * i) = -1;
    u8(self, 361) = 0xff;
    u8(self, 360) = 0xff;
    for (int i = 0; i < 4; i++) {
        u16(self, 344 + 2 * i) = 0;
        u16(self, 352 + 2 * i) = 0;
    }
    *reinterpret_cast<Block72*>(static_cast<unsigned char*>(self) + 328) = *static_cast<const Block72*>(description);
    unsigned flags = (u32(self, 12) | 0x102000u) & 0xffffe7ffu;
    u32(self, 12) = flags;
    if ((*reinterpret_cast<unsigned long long*>(static_cast<unsigned char*>(self) + 336) & (0x8000ull << 24)) != 0) u32(self, 12) = flags & 0xffffcfffu;
    objectUpdateInGrid__FP10GameObject(self);
}
