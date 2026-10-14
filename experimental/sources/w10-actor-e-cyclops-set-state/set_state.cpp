#include "../w9-rtti-vtables-entities-c-bytes/bytes.hpp"
// Observed setState__7Cyclopsi at 0x2033e0, 1052 bytes. The animation state is at object+288 and the
// word at +36 points to a header table T. The requested state lives in a stack word.
//  1. setStateProcessing__8CreaturePi(object, &state); a nonzero result returns at once.
//  2. States of 17 and above, and 4, 11 and 12, only store. Otherwise the jump table at 0x46f710 selects.
//     "t" is animAddTransitionTo, "s" is animAddOneShot (both rates equal); R is an eRand__10GameObject
//     roll. 0.3 is bits 0x3e999999 and 0.1 is bits 0x3dcccccc.
//     0: +256 = word(+808) + R mod 60 + 120 (stored before the call); t T+108, 0.3, flags 3.
//     1: resetRouting; getSmartRandomDestination(range 300) copied to +272; word +176 = 0;
//        t T+132, 0.3, 2051; animGlobalSync.
//     2: resetRouting; +176 = 0; t T+132, 0.3, 2051; animGlobalSync.
//     3: resetRouting; when the current halfword at +572 is 3, 6, 13, 14, 15 or 16 only the store; otherwise
//        t T+132, 0.3, 2051; animGlobalSync.
//     5: t T+100, 0.1, 3.
//     6: s T+28, 0.1, 8.
//     7: resetRouting; t T+132, 0.3, 2051; animGlobalSync; +176 = 0; +256 = word(+808) + 60.
//     8: the state becomes the current signed halfword at +572 when it is 9, or 6 with bit 3 of the halfword
//        at +294 set; otherwise s T+92, 0.1, 16.
//     9: t T+108, 0.3, 3.
//     10: byte +40 = 120; t T+148, 0.3, 6; dropTreasure__8CreatureG6Point3iffffffPc(object, copy of the 12 bytes
//        at +16, signed halfword +56, 36.0, 2.0, 1.6, 0.6, 1.6, 0.25, null name); then the virtual slot at byte
//        offset 192.
//     13: when the word at +576 is non-zero: heading = iatan2(y, x) of the singles at +20 and +16 of the pointer
//        at (+576)+12 minus those of the object; when the signed 16-bit value heading - halfword(+56) is positive,
//        s T+44, 0.1, 8; otherwise s T+52, 0.1, 8.
//     14: s T+68, 0.1, 8.   15: s T+76, 0.1, 8.   16: s T+36, 0.1, 8; word +960 = word(+808) + 600.
//  3. Unless returned early, the low 16 bits of the state word go to +572.
// Unknown: the state names and the table layout.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
int eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
int iatan2__Fff(float y, float x);
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

extern "C" void setState__7Cyclopsi(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned r = static_cast<unsigned>(eRand__10GameObject(self)) % 60u;
        write_word(object + 256u, read_word(object + 808u) + r + 120u);
        tr(object, 108u, slow, 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char point[12];
        getSmartRandomDestination__8Creaturei(point, self, 300);
        for (unsigned i = 0; i < 12u; ++i) write_u8(object + 272u + i, point[i]);
        write_word(object + 176u, 0u);
        tr(object, 132u, slow, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 2u:
        resetRouting__8Creature(self);
        write_word(object + 176u, 0u);
        tr(object, 132u, slow, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    case 3u: {
        resetRouting__8Creature(self);
        const int current = read_i16(object + 572u);
        if (current != 3 && current != 6 && current != 13 && current != 14 && current != 15 && current != 16) {
            tr(object, 132u, slow, 2051);
            animGlobalSync__FP14AnimationState(bytes(object + 288u));
        }
        break;
    }
    case 5u:
        tr(object, 100u, fast, 3);
        break;
    case 6u:
        os(object, 28u, 8);
        break;
    case 7u:
        resetRouting__8Creature(self);
        tr(object, 132u, slow, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        write_word(object + 176u, 0u);
        write_word(object + 256u, read_word(object + 808u) + 60u);
        break;
    case 8u: {
        const int current = read_i16(object + 572u);
        if (current == 9 || (current == 6 && (read_u16(object + 294u) & 8u) != 0u)) state = current;
        else os(object, 92u, 16);
        break;
    }
    case 9u:
        tr(object, 108u, slow, 3);
        break;
    case 10u: {
        write_u8(object + 40u, 120u);
        tr(object, 148u, slow, 6);
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = static_cast<unsigned char>(read_u8(object + 16u + i));
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), single_from_bits(0x42100000u),
                                                 single_from_bits(0x40000000u), single_from_bits(0x3fcccccc),
                                                 single_from_bits(0x3f199999u), single_from_bits(0x3fcccccc),
                                                 single_from_bits(0x3e800000u), nullptr);
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 13u:
        if (read_word(object + 576u) != 0u) {
            const Address target = address(pointer(read_word(address(pointer(read_word(object + 576u))) + 12u)));
            const int heading = iatan2__Fff(read_single(target + 20u) - read_single(object + 20u),
                                            read_single(target + 16u) - read_single(object + 16u));
            const short difference = static_cast<short>(heading - read_u16(object + 56u));
            if (difference > 0) os(object, 44u, 8);
            else os(object, 52u, 8);
        }
        break;
    case 14u:
        os(object, 68u, 8);
        break;
    case 15u:
        os(object, 76u, 8);
        break;
    case 16u:
        os(object, 36u, 8);
        write_word(object + 960u, read_word(object + 808u) + 600u);
        break;
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
