#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__5Demoni at 0x1eae30, 1064 bytes. The animation state is at object+288 and the word at
// +36 points to a header table T. Unlike the other Creature setState routines checked so far it does not call
// setStateProcessing. The requested state is the second argument.
//  1. Bit 9 (0x200) of the 64-bit value at +160 is cleared on every call.
//  2. States of 18 and above, and 2, 4, 7, 11 and 12, only store. Otherwise the jump table at 0x46d560
//     selects. "t" is animAddTransitionTo, "s" is animAddOneShot (both rates equal); R is an
//     eRand__10GameObject roll. 0.3 is bits 0x3e999999 and 0.1 is bits 0x3dcccccc.
//     0: +256 = word(+808) + R mod 120 (stored before the call); t T+68, 0.3, flags 3.
//     1: resetRouting; getSmartRandomDestination(range 200) copied to +272; +256 = word(+808) + R mod 120 + 300
//        (stored before the call); t T+108, 0.3, 2051; animGlobalSync.
//     3: resetRouting; when the current signed halfword at +572 is 6 or 8 only the store; otherwise
//        t T+100, 0.1, 2051; animGlobalSync.
//     5: t T+52, 0.1, 3.
//     6: s T+28, 0.1, 8.   16: s T+36, 0.1, 8.   17: s T+44, 0.1, 8.
//     8: the state becomes the current signed halfword at +572 when it is 9, or 6 with bit 3 of the halfword
//        at +294 set; otherwise s T+60, 0.1, 16.
//     9: t T+92, 0.1, 3.
//     10: word +12 |= 0x4000; +256 = word(+808); dropTreasure__8CreatureG6Point3iffffffPc(object, copy of the
//        12 bytes at +16, signed halfword +56, 36.0, 2.0, 1.8, 0.6, 2.0, 0.25, null name); t T+92, 0.3, 6;
//        then the virtual slot at byte offset 192.
//     13: t T+124, 0.1, 3; s T+116, 0.1, 8.
//     14: t T+52, 0.1, 3; bit 9 of the 64-bit value at +160 is set; +256 = word(+808) + R mod 60 + 60.
//     15: t T+84, 0.1, 3; s T+28, 0.1, 8; a second roll: +256 = word(+808) + (R as signed single * 2^-16 * 180.0,
//        truncated) + 240.
//  3. The low 16 bits of the state word go to +572.
// Unknown: the state names and the table layout.
extern "C" {
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
}

extern "C" void setState__5Demoni(void* self, int state) {
    const Address object = address(self);
    write_u64(object + 160u, read_u64(object + 160u) & ~0x200ull);
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned r = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        write_word(object + 256u, read_word(object + 808u) + r);
        tr(object, 68u, slow, 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 200);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        const unsigned r = static_cast<unsigned>(eRand__10GameObject(self)) % 120u;
        write_word(object + 256u, read_word(object + 808u) + r + 300u);
        tr(object, 108u, slow, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 3u: {
        resetRouting__8Creature(self);
        const int current = read_i16(object + 572u);
        if (current != 6 && current != 8) {
            tr(object, 100u, fast, 2051);
            animGlobalSync__FP14AnimationState(bytes(object + 288u));
        }
        break;
    }
    case 5u:
        tr(object, 52u, fast, 3);
        break;
    case 6u:
        os(object, 28u, 8);
        break;
    case 8u: {
        const int current = read_i16(object + 572u);
        if (current == 9 || (current == 6 && (read_u16(object + 294u) & 8u) != 0u)) state = current;
        else os(object, 60u, 16);
        break;
    }
    case 9u:
        tr(object, 92u, fast, 3);
        break;
    case 10u: {
        write_word(object + 12u, read_word(object + 12u) | 0x4000u);
        write_word(object + 256u, read_word(object + 808u));
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u),
                                                 single_from_bits(0x40000000u), single_from_bits(0x3fe66666u),
                                                 single_from_bits(0x3f199999u), single_from_bits(0x40000000u),
                                                 single_from_bits(0x3e800000u), nullptr);
        tr(object, 92u, slow, 6);
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 13u:
        tr(object, 124u, fast, 3);
        os(object, 116u, 8);
        break;
    case 14u: {
        tr(object, 52u, fast, 3);
        const unsigned r = static_cast<unsigned>(eRand__10GameObject(self)) % 60u;
        write_u64(object + 160u, read_u64(object + 160u) | 0x200ull);
        write_word(object + 256u, read_word(object + 808u) + r + 60u);
        break;
    }
    case 15u: {
        tr(object, 84u, fast, 3);
        os(object, 28u, 8);
        const float scaled = static_cast<float>(eRand__10GameObject(self)) * single_from_bits(0x37800000u) * single_from_bits(0x43340000u);
        write_word(object + 256u, read_word(object + 808u) + static_cast<unsigned>(static_cast<int>(scaled)) + 240u);
        break;
    }
    case 16u:
        os(object, 36u, 8);
        break;
    case 17u:
        os(object, 44u, 8);
        break;
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
