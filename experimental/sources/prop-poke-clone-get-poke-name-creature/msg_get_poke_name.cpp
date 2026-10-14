#include "../vslots-b-bytes/bytes.hpp"
// Observed msg_getPokeName__8CreaturePUsi at 0x111f68, 980 bytes.
// Returns zero without touching the buffer when the word at object+892 is
// zero, when the entity word at byte 21872 of the 23984-byte player record
// for the index is zero, or when the 64-bit word at object+160 has any of
// 0x00c00000. When bit 3 of the word at object+12 is set, the function at
// vtable byte 4 is called with no argument assignment (a type-info query;
// compare amxReapEnemies) and a result equal to the MummyKing type-info
// returns zero. A null object there calls __throw_bad_typeid.
//
// The vector from this object's point (+16) to the entity's point is scaled
// by rsqrt of its squared length; the z component is also scaled and stored
// but never read. "talkCone" starts as 0.5 and is replaced by the tag's
// value when the tag exists. When the "shop" tag is nonzero and someone is
// in the shop, the text "UpToShop" goes through usprintf with eight zero
// singles and the function returns 1. Otherwise icos and isin of the signed
// halfword at +56 form (c, s); x*c + y*s of the scaled vector must exceed
// the cone, else the buffer's first halfword is cleared and zero is
// returned. Then objectFindInBox is called with a box of 180.0 around this
// point (z from this z up to z + 180.0) and room for 64 objects. A listed
// object blocks when bits 3, 6 and 24 of its word at +12 are clear, bit 5 is
// set, its halfword at +792 is zero and bit 26 of its 64-bit word at +160
// is clear. A block copies "CannotTalk" into the buffer and returns zero.
// Otherwise "TalkTo" goes through usprintf into a 400-byte local, is copied
// to the buffer, then " " and the text for the tag pointer at +892 are
// appended, and the function returns 1.
// Unknown: the names of the flag bits and of the words at +160, +792 and +892.
extern "C" unsigned char player[];
extern "C" void* __tf9MummyKing();
extern "C" int __eq__C9type_infoRC9type_info(void* left, void* right);
extern "C" void __throw_bad_typeid();
extern "C" char* norrath_actor_find_tag_string(void* object, char* tag, int occurrence);
extern "C" float findTagFloat__10GameObjectPci(void* object, char* tag, int occurrence);
extern "C" int norrath_game_object_find_tag_int(void* object, char* tag, int occurrence);
extern "C" int gameSomeoneInShop__Fv();
extern "C" unsigned short* textGetString__FPc(char* key);
extern "C" void usprintf__FPUsT0ffffffff(unsigned short* destination, unsigned short* format, float a, float b, float c,
                                         float d, float e, float f, float g, float h);
extern "C" float icos__Fi(int angle);
extern "C" float isin__Fi(int angle);
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const float* minimum, const float* maximum, void** objects, int count);
extern "C" void ustrcpy__FPUsT0(unsigned short* destination, unsigned short* source);
extern "C" void ustrcat__FPUsPc(unsigned short* destination, char* source);
extern "C" void ustrcat__FPUsT0(unsigned short* destination, unsigned short* source);

namespace {
inline float reciprocal_length(float squared) {
#if defined(__mips__)
    float out;
    const float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(out) : "f"(one), "f"(squared));
    return out;
#else
    return 1.0f / __builtin_sqrtf(squared);
#endif
}
}

extern "C" int msg_getPokeName__8CreaturePUsi(void* self, unsigned short* buffer, int index) {
    using namespace vslots_b;
    const Address base = address(self);
    if (read_word(base + 892u) == 0u) return 0;
    const unsigned slot = static_cast<unsigned>(index) * 23984u + 21872u;
    const unsigned entity_bits = read_word(address(player) + slot);
    if (entity_bits == 0u) return 0;
    if ((read_u64(base + 160u) & 0x00c00000ull) != 0ull) return 0;
    if ((read_word(base + 12u) & 8u) != 0u) {
        if (self == nullptr) __throw_bad_typeid();
        using TypeCall = void* (*)();
        TypeCall type = reinterpret_cast<TypeCall>(pointer(read_word(address(pointer(read_word(base + 72u))) + 4u)));
        void* mine = type();
        if (__eq__C9type_infoRC9type_info(mine, __tf9MummyKing()) != 0) return 0;
    }
    const Address entity = address(pointer(entity_bits));
    const float dx = read_single(entity + 16u) - read_single(base + 16u);
    const float dy = read_single(entity + 20u) - read_single(base + 20u);
    const float dz = read_single(entity + 24u) - read_single(base + 24u);
    const float inverse = reciprocal_length(dx * dx + dy * dy + dz * dz);
    const float nx = dx * inverse;
    const float ny = dy * inverse;
    float cone = 0.5f;
    static char talk_cone[] = {'t', 'a', 'l', 'k', 'C', 'o', 'n', 'e', '\0'};
    if (norrath_actor_find_tag_string(self, talk_cone, 0) != nullptr) {
        cone = findTagFloat__10GameObjectPci(self, talk_cone, 0);
    }
    static char shop[] = {'s', 'h', 'o', 'p', '\0'};
    if (norrath_game_object_find_tag_int(self, shop, 0) != 0 && gameSomeoneInShop__Fv() != 0) {
        static char up_to_shop[] = {'U', 'p', 'T', 'o', 'S', 'h', 'o', 'p', '\0'};
        usprintf__FPUsT0ffffffff(buffer, textGetString__FPc(up_to_shop), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
        return 1;
    }
    const int angle = read_i16(base + 56u);
    const float c = icos__Fi(angle);
    const float s = isin__Fi(angle);
    if (!(cone < nx * c + ny * s)) {
        write_u16(address(buffer), 0u);
        return 0;
    }
    const float x = read_single(base + 16u);
    const float y = read_single(base + 20u);
    const float z = read_single(base + 24u);
    const float low[3] = {x - 180.0f, y - 180.0f, z};
    const float high[3] = {x + 180.0f, y + 180.0f, z + 180.0f};
    void* found[64];
    bool blocked = false;
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
    if (found[0] != nullptr) {
        void** cursor = found;
        do {
            const Address other = address(*cursor);
            ++cursor;
            const unsigned flags = read_word(other + 12u);
            if ((flags & 8u) == 0u && (flags & 0x40u) == 0u && (flags & 0x01000000u) == 0u && (flags & 0x20u) != 0u &&
                read_i16(other + 792u) == 0 && (read_u64(other + 160u) & 0x04000000ull) == 0ull) {
                blocked = true;
                break;
            }
        } while (*cursor != nullptr);
    }
    if (blocked) {
        static char cannot_talk[] = {'C', 'a', 'n', 'n', 'o', 't', 'T', 'a', 'l', 'k', '\0'};
        ustrcpy__FPUsT0(buffer, textGetString__FPc(cannot_talk));
        return 0;
    }
    unsigned short line[200];
    static char talk_to[] = {'T', 'a', 'l', 'k', 'T', 'o', '\0'};
    usprintf__FPUsT0ffffffff(line, textGetString__FPc(talk_to), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    ustrcpy__FPUsT0(buffer, line);
    static char space[] = {' ', '\0'};
    ustrcat__FPUsPc(buffer, space);
    ustrcat__FPUsT0(buffer, textGetString__FPc(static_cast<char*>(pointer(read_word(base + 892u)))));
    return 1;
}
