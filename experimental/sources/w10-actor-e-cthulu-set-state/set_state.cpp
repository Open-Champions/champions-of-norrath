#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__6Cthului at 0x281b20, 1272 bytes. The animation state is at object+288 and the word at
// +36 points to a header table T. The requested state lives in a stack word. The word at +964 selects between
// two animation sets in several states below ("flag").
//  1. setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at once.
//  2. States of 17 and above, and 2, 4, 7, 11 and 12, only store. Otherwise the jump table at 0x476b20
//     selects. "t" is animAddTransitionTo, "s" is animAddOneShot (both rates equal), R is an
//     eRand__10GameObject roll. 0.1 is bits 0x3dcccccc and 0.3 is bits 0x3e999999; rates are 0.1 unless noted.
//     0: +256 = word(+808) + R mod 120; t T+156 when the flag is set, else T+76, flags 3.
//     1: resetRouting; getSmartRandomDestination(range 200) copied to +272; +256 = word(+808) + R mod 120 + 300;
//        t T+84 when the flag is set, else T+68, flags 2051; animGlobalSync.
//     3: resetRouting. When the signed word at +960 is below the signed word at +808: with the flag clear,
//        t T+84, 2051; s T+108, 2056; the flag becomes 1; with it set, t T+60, 2051 and the flag becomes 0;
//        then +960 = word(+808) + R mod 300 + 300 (stored before the sync call) and animGlobalSync.
//        Otherwise, unless the current signed halfword at +572 is 6, 13, 14, 15 or 16: t T+60 when the flag is
//        clear, else T+84, flags 2051; animGlobalSync.
//     5: t T+156 when the flag is set, else T+76, flags 3.
//     6: s T+148 when the flag is set, else T+28, flags 2056; word +972 = 0.
//     8: the state becomes the current signed halfword at +572 when the flag is set, or the halfword is 9, or it
//        is 6 with bit 3 of the halfword at +294 set; otherwise s T+124, flags 16.
//     9: t 0.3 flags 3: T+76 when the flag is set, else T+156.
//     10: dropTreasure__8CreatureG6Point3iffffffPc(object, copy of the 12 bytes at +16, signed halfword +56, 36.0,
//        2.0, 1.8, 0.4, 2.0, 0.25, null name); t T+116, 0.3, flags 6; then the virtual slot at byte offset 192.
//     13: s T+36, flags 8; +972 = 0.   14: s T+44, flags 8; +972 = 0.   15: s T+52, flags 2056; +972 = 1.
//     16: s T+92, flags 2056; +972 = 0; +968 = word(+808) + R mod 300 + 120.
//  3. Unless returned early, the low 16 bits of the state word go to +572.
// Unknown: the state names, what the flag and the words at +960, +968 and +972 mean, and the table layout.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int a, float f12, float f13, float f14,
                                              float f15, float f16, float f17, char* name);
}

namespace {
using namespace rtti_c;
const unsigned slow = 0x3e999999u;
const unsigned fast = 0x3dccccccu;
__attribute__((always_inline)) inline void* header(Address object, unsigned offset) {
    return pointer(read_word(address(pointer(read_word(object + 36u))) + offset));
}
__attribute__((always_inline)) inline void tr(Address object, unsigned offset, unsigned rate, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset), single_from_bits(rate), flags);
}
__attribute__((always_inline)) inline void os(Address object, unsigned offset, int flags) {
    const float value = single_from_bits(fast);
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, offset), value, value, flags);
}
__attribute__((always_inline)) inline unsigned roll(void* self) { return static_cast<unsigned>(eRand__10GameObject(self)); }
}

extern "C" void setState__6Cthului(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (static_cast<unsigned>(state)) {
    case 0u:
        write_word(object + 256u, read_word(object + 808u) + roll(self) % 120u);
        tr(object, read_word(object + 964u) != 0u ? 156u : 76u, fast, 3);
        break;
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        write_word(object + 256u, read_word(object + 808u) + roll(self) % 120u + 300u);
        tr(object, read_word(object + 964u) != 0u ? 84u : 68u, fast, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 3u:
        resetRouting__8Creature(self);
        if (signed_word(read_word(object + 960u)) < signed_word(read_word(object + 808u))) {
            if (read_word(object + 964u) == 0u) {
                tr(object, 84u, fast, 2051);
                os(object, 108u, 2056);
                write_word(object + 964u, 1u);
            } else {
                tr(object, 60u, fast, 2051);
                write_word(object + 964u, 0u);
            }
            write_word(object + 960u, read_word(object + 808u) + roll(self) % 300u + 300u);
            animGlobalSync__FP14AnimationState(bytes(object + 288u));
        } else {
            const int current = read_i16(object + 572u);
            if (current != 6 && current != 13 && current != 14 && current != 15 && current != 16) {
                tr(object, read_word(object + 964u) == 0u ? 60u : 84u, fast, 2051);
                animGlobalSync__FP14AnimationState(bytes(object + 288u));
            }
        }
        break;
    case 5u:
        tr(object, read_word(object + 964u) != 0u ? 156u : 76u, fast, 3);
        break;
    case 6u:
        os(object, read_word(object + 964u) != 0u ? 148u : 28u, 2056);
        write_word(object + 972u, 0u);
        break;
    case 8u: {
        const int current = read_i16(object + 572u);
        if (read_word(object + 964u) != 0u || current == 9 || (current == 6 && (read_u16(object + 294u) & 8u) != 0u)) state = current;
        else os(object, 124u, 16);
        break;
    }
    case 9u:
        tr(object, read_word(object + 964u) != 0u ? 76u : 156u, slow, 3);
        break;
    case 10u: {
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u),
                                                 single_from_bits(0x40000000u), single_from_bits(0x3fe66666u),
                                                 single_from_bits(0x3ecccccc), single_from_bits(0x40000000u),
                                                 single_from_bits(0x3e800000u), nullptr);
        tr(object, 116u, slow, 6);
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 13u:
        os(object, 36u, 8);
        write_word(object + 972u, 0u);
        break;
    case 14u:
        os(object, 44u, 8);
        write_word(object + 972u, 0u);
        break;
    case 15u:
        os(object, 52u, 2056);
        write_word(object + 972u, 1u);
        break;
    case 16u: {
        os(object, 92u, 2056);
        const unsigned r = roll(self) % 300u;
        write_word(object + 972u, 0u);
        write_word(object + 968u, read_word(object + 808u) + r + 120u);
        break;
    }
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
