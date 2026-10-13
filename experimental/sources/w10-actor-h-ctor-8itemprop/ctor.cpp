// Observed __8ItemPropG6Point3PQ24item11descriptioni at 0x0014b9e8, 480 bytes.
// ItemProp(this, position, description d, flag). In the order of the image:
//  - The item table entry index is item::getProxyMeshId(d), asked twice. Entries of the table
//    _4item$globalItem are 16 bytes; the word at entry(first call)+0 and the word at
//    entry(second call)+4 are passed on.
//  - The Prop constructor (__4PropG6Point3PcN22li) runs on this with a copy of the position, the text
//    "equipmnt.lmp", those two words, -1 and the flag.
//  - this+72 gets the ItemProp table pointer (_vt$8ItemProp). Four halfwords at this+388, +380, +372
//    and +364 get -1; bytes this+360 and this+361 get -1; halfwords this+344..350 and this+352..358
//    (eight, two bytes apart) get 0.
//  - 72 bytes are copied from d to this+328 (this overwrites the halfword at this+388).
//  - The word at this+12 is OR-ed with 0x102000 and has bits 0x1800 cleared; when bit 39 of the
//    64-bit value at this+336 is set, bits 0x3000 are cleared as well.
//  - objectUpdateInGrid__FP10GameObject runs on this, then matMakeTransYaw__FP8Matrix34G6Point3s
//    builds the matrix at this+272 from a copy of this+16 and the 16-bit sum of the halfword at
//    this+56 plus 16384.
// No result is used (the final value left in the result register comes from the matrix call).
// Inferred: d is an item description 72 bytes long as far as this copy goes, and the table entry's
// two words name resources. Unknown: the meaning of the copied fields and of the flag bits.
// Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
int getProxyMeshId__4itemPQ24item11description(void* description);
void prop_construct(void* self, const void* position, char* lump, char* first, char* second, long number, int flag)
    __asm__("__4PropG6Point3PcN22li");
void objectUpdateInGrid__FP10GameObject(void* object);
void matMakeTransYaw__FP8Matrix34G6Point3s(void* matrix, const void* point, short yaw);
extern unsigned char globalItem[] __asm__("_4item$globalItem");
extern unsigned char vt_ItemProp[] __asm__("_vt$8ItemProp");
}

namespace {
unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
typedef unsigned long long __attribute__((may_alias)) AliasedDouble;
}  // namespace

extern "C" void itemprop_construct(void* self, const void* position, void* description, int flag)
    __asm__("__8ItemPropG6Point3PQ24item11descriptioni");
extern "C" void itemprop_construct(void* self, const void* position, void* description, int flag) {
    unsigned char* object = static_cast<unsigned char*>(self);
    unsigned char first_point[12];
    w10h::copy_bytes(first_point, position, 12);
    const unsigned char* first_entry = globalItem + (getProxyMeshId__4itemPQ24item11description(description) << 4);
    const unsigned char* second_entry = globalItem + (getProxyMeshId__4itemPQ24item11description(description) << 4);
    prop_construct(self, first_point, const_cast<char*>("equipmnt.lmp"),
                   reinterpret_cast<char*>(w10h::from_word(*reinterpret_cast<const unsigned*>(first_entry))),
                   reinterpret_cast<char*>(w10h::from_word(*reinterpret_cast<const unsigned*>(second_entry + 4))), -1L, flag);
    w10h::word_at(self, 72) = low32(vt_ItemProp);
    for (int i = 0; i < 4; ++i) *reinterpret_cast<short*>(object + 388 - 8 * i) = -1;
    object[361] = 0xff;
    object[360] = 0xff;
    for (int i = 0; i < 4; ++i) {
        *reinterpret_cast<unsigned short*>(object + 328 + 16 + 2 * i) = 0;
        *reinterpret_cast<unsigned short*>(object + 336 + 16 + 2 * i) = 0;
    }
    w10h::copy_bytes(object + 328, description, 72);
    unsigned flags = w10h::word_at(self, 12) | 0x102000u;
    flags = flags & 0xffffe7ffu;
    w10h::word_at(self, 12) = flags;
    if ((*reinterpret_cast<AliasedDouble*>(object + 336) & (0x8000ull << 24)) != 0) w10h::word_at(self, 12) = flags & 0xffffcfffu;
    objectUpdateInGrid__FP10GameObject(self);
    unsigned char second_point[12];
    w10h::copy_bytes(second_point, object + 16, 12);
    matMakeTransYaw__FP8Matrix34G6Point3s(object + 272, second_point, static_cast<short>(*reinterpret_cast<unsigned short*>(object + 56) + 16384));
}
