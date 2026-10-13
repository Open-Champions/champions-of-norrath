#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__13FemaleDarkElfi at 0x2861f8, 1828 bytes. The animation state is at object+288, the word
// at +36 points to a header table T, and the signed byte at +960 is the elf kind. The requested state lives in
// a stack word.
//  1. setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at once.
//  2. Bit 9 (0x200) of the 64-bit value at +160 is cleared. States of 16 and above, and 2, 4, 11 and 12,
//     only store. Otherwise the jump table at 0x477170 selects. "t" is animAddTransitionTo, "s" is
//     animAddOneShot (both rates equal), R is an eRand__10GameObject roll; every rate is 0.1 (0x3dcccccc).
//     0: t T+212, flags 3; +256 = word(+808) + 180.
//     1: resetRouting; getSmartRandomDestination(range 200) copied to +272; t T+228, 2051; animGlobalSync.
//     3: resetRouting; t T+228 when the kind is 1, else T+220, flags 2051; animGlobalSync.
//     5: t T+180, flags 3.
//     6: t T+180, 3; s T+148+8*(R&3), 2056; +256 = R2 mod 60 (a second roll).
//     7: resetRouting; t T+140, 2051; animGlobalSync; +256 = word(+808) + 120.
//     8: the state becomes the current signed halfword at +572 when it is 9. Otherwise, when the word at +744
//        has any of the bits 0x380000: with k = 32 + 16*(R&3), t T+k+4, flags 20; animAddToEnd T+k+12, flags 36;
//        animAddToEnd T+212, flags 2. When it has none: s T+188, flags 16 if bit 9 of the 64-bit value at +160 is
//        set (not reachable after the clear above), else s T+124 for kind 1 and T+204 for any other kind.
//     9: t T+212, flags 3.
//     13: t T+132, 3; s T+108+8*(R&1), 2056; +256 = R2 mod 60 + 120.
//     14: t T+180, 3; +256 = word(+808) + 120; bit 9 of the 64-bit value at +160 is set.
//     15: word +12 &= ~9; for kind 0 only, +964 = (R&2) + 29 and then +1104 = (R2 mod 3)*2 + 33 (R2 a second
//        roll); bit 32 of the +160 value is set; initType__13FemaleDarkElfb(object, false); objectAddToRunList;
//        halfwords 50, 128, 255, 150 at +146, +150, +152, +154 and 0, 128, 255, 0 at +140, +142, +144, +148; the
//        state becomes 0.
//     10: t T+36+16*(R&3), flags 6; when bit 32 of the +160 value is clear,
//        dropTreasure__8CreatureG6Point3iffffffPc(object, copy of +16, halfword +56, 36.0, 2.0, 1.8, 0.6, 2.0,
//        0.25, null name). When bit 1 of the word at +12 is clear, for each of the words +964 and +1104 that is
//        non-zero (indexes 0 and 1): modelGetCharAttachmentPos(model T+12, copy of +16, halfword +56 minus 16384,
//        object+288, index, two point outputs, 1.0), then a 584-byte blockAlloc__Fi and, if it succeeded, the PhProp
//        constructor with the position, the strings T+0, T+8*word and T+8*word+8, 1, the flag word 16641 and the
//        single at +224; the word is then cleared. After that the virtual slot at byte offset 192.
//  3. Unless returned early, the low 16 bits of the state word go to +572.
// Unknown: the state names, the kinds and what the table fields and the word at +744 hold.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animAddToEnd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int a, float f12, float f13, float f14,
                                              float f15, float f16, float f17, char* name);
void initType__13FemaleDarkElfb(void* self, bool flag);
void objectAddToRunList__FP10GameObject(void* object);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* point, short a, void* animation, int index, void* out_a, void* out_b, float scale);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* point, char* a, char* b, char* c, long d,
                                                    int e, void* description, float scale);
}

namespace {
using namespace rtti_c;
const unsigned rate = 0x3dccccccu;
__attribute__((always_inline)) inline void* header(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void tr(Address object, unsigned offset, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset), single_from_bits(rate), flags);
}
__attribute__((always_inline)) inline void os(Address object, unsigned offset, int flags) {
    const float value = single_from_bits(rate);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, offset), value, value, flags);
}
__attribute__((always_inline)) inline int kind(Address object) { return read_s8(object + 960u); }
__attribute__((always_inline)) inline unsigned roll(void* self) { return static_cast<unsigned>(eRand__10GameObject(self)); }
__attribute__((always_inline)) inline void spawn(Address object, unsigned word_offset, int index) {
    float position[3], model_point[3], extra[3];
    for (unsigned i = 0; i < 12u; ++i) {
        reinterpret_cast<unsigned char*>(position)[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        reinterpret_cast<unsigned char*>(model_point)[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
    }
    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
        pointer(read_word(address(pointer(read_word(object + 36u))) + 12u)), model_point,
        static_cast<short>(read_u16(object + 56u) - 16384u), bytes(object + 288u), index, position, extra,
        single_from_bits(0x3f800000u));
    void* block = blockAlloc__Fi(584);
    if (block != nullptr) {
        const Address table = address(pointer(read_word(object + 36u)));
        const unsigned word = read_word(object + word_offset);
        const Address entry = table + word * 8u;
        __6PhPropG6Point3PcN22liPQ24item11descriptionf(
            block, position, static_cast<char*>(pointer(read_word(table))), static_cast<char*>(pointer(read_word(entry))),
            static_cast<char*>(pointer(read_word(entry + 8u))), 1, 16641, nullptr, read_single(object + 224u));
    }
    write_word(object + word_offset, 0u);
}
}

extern "C" void setState__13FemaleDarkElfi(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    write_u64(object + 160u, read_u64(object + 160u) & ~0x200ull);
    switch (static_cast<unsigned>(state)) {
    case 0u:
        tr(object, 212u, 3);
        write_word(object + 256u, read_word(object + 808u) + 180u);
        break;
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        tr(object, 228u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 3u:
        resetRouting__8Creature(self);
        tr(object, kind(object) == 1 ? 228u : 220u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 5u:
        tr(object, 180u, 3);
        break;
    case 6u: {
        tr(object, 180u, 3);
        const unsigned r = roll(self) & 3u;
        os(object, 148u + 8u * r, 2056);
        write_word(object + 256u, roll(self) % 60u);
        break;
    }
    case 7u:
        resetRouting__8Creature(self);
        tr(object, 140u, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        write_word(object + 256u, read_word(object + 808u) + 120u);
        break;
    case 8u: {
        const int current = read_i16(object + 572u);
        if (current == 9) {
            state = current;
        } else if ((read_word(object + 744u) & 0x380000u) != 0u) {
            const unsigned k = 32u + 16u * (roll(self) & 3u);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, k + 4u), single_from_bits(rate), 20);
            animAddToEnd__FP14AnimationStateP15AnimationHeaderi(bytes(object + 288u), header(object, k + 12u), 36);
            animAddToEnd__FP14AnimationStateP15AnimationHeaderi(bytes(object + 288u), header(object, 212u), 2);
        } else if ((read_u64(object + 160u) & 0x200ull) != 0ull) {
            os(object, 188u, 16);
        } else if (kind(object) == 1) {
            os(object, 124u, 16);
        } else {
            os(object, 204u, 16);
        }
        break;
    }
    case 9u:
        tr(object, 212u, 3);
        break;
    case 10u: {
        const unsigned r = roll(self) & 3u;
        tr(object, 36u + 16u * r, 6);
        unsigned char point[12];
        if ((read_u64(object + 160u) & (1ull << 32)) == 0ull) {
            for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
            dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u),
                                                     single_from_bits(0x40000000u), single_from_bits(0x3fe66666u),
                                                     single_from_bits(0x3f199999u), single_from_bits(0x40000000u),
                                                     single_from_bits(0x3e800000u), nullptr);
        }
        if ((read_word(object + 12u) & 2u) == 0u) {
            if (read_word(object + 964u) != 0u) spawn(object, 964u, 0);
            if (read_word(object + 1104u) != 0u) spawn(object, 1104u, 1);
        }
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 13u: {
        tr(object, 132u, 3);
        const unsigned r = roll(self) & 1u;
        os(object, 108u + 8u * r, 2056);
        write_word(object + 256u, roll(self) % 60u + 120u);
        break;
    }
    case 14u:
        tr(object, 180u, 3);
        write_word(object + 256u, read_word(object + 808u) + 120u);
        write_u64(object + 160u, read_u64(object + 160u) | 0x200ull);
        break;
    case 15u:
        write_word(object + 12u, read_word(object + 12u) & 0xfffffff6u);
        if (kind(object) == 0) {
            write_word(object + 964u, (roll(self) & 2u) + 29u);
            write_word(object + 1104u, (roll(self) % 3u) * 2u + 33u);
        }
        write_u64(object + 160u, read_u64(object + 160u) | (1ull << 32));
        initType__13FemaleDarkElfb(self, false);
        objectAddToRunList__FP10GameObject(self);
        write_u16(object + 140u, 0u);
        write_u16(object + 146u, 50u);
        write_u16(object + 150u, 128u);
        write_u16(object + 152u, 255u);
        write_u16(object + 154u, 150u);
        state = 0;
        write_u16(object + 142u, 128u);
        write_u16(object + 144u, 255u);
        write_u16(object + 148u, 0u);
        break;
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
