#include "../vslots-b-bytes/bytes.hpp"
// Observed setState__10FireBeetlei at 0x00211090, 1400 bytes. The requested state is kept in a stack word;
// setStateProcessing is called with the object and that word's address, and a non-zero result returns without
// storing anything. Otherwise the word, which the call or the cases below may have changed, selects a path
// through a 15-word jump table at 0x470520 (values 15 and above, and 2, 4, 7, 11, 12, only store). Every path
// ends by storing the low 16 bits of the word at object+572. g is the word at object+36 (re-read before each
// use), "anim" the animation state at object+288, "at(n)" the word at g+n (an animation header), cur the signed
// halfword at +572 before the store, clock the word at +808, r an eRand(object) result (unsigned), and the
// transition and one-shot calls are animAddTransitionTo(anim, header, rate, flags) and
// animAddOneShot(anim, header, rate1, rate2, flags). Rates: A 0.5, B 1.0, C 0.01 (0x3c23d70a), D 0.1
// (0x3dcccccc), E 2.0.
//  0: w256 = clock + r % 60 + 120 (stored before the call); transition(at(84), A, 3).
//  1: resetRouting; a 12-byte point from getSmartRandomDestination(object, 300) is stored at +272. When cur is
//     13: the 64-bit word at +160 loses bit 0x100000, transition(at(124), B, 3), one-shot(at(108), C, D, 8);
//     otherwise transition(at(124), D, 3). Then the float at +176 becomes 160.0.
//  3: the float at +176 becomes 160.0, then when cur is 13: as in 1 (clear the bit, transition(at(124), B, 3),
//     one-shot(at(108), C, D, 8)); when cur is 3: nothing; otherwise transition(at(124), D, 3).
//  5: transition(at(100), D, 3).
//  6: transition(at(100), D, 3); one-shot(at(28), D, D, 8); w256 = r % 20 + 20.
//  8: when cur is 9, or cur is 6 with bit 3 (8) of the unsigned halfword at +294 set, the word becomes cur.
//     Otherwise, when cur is 13: w256 = clock + r % 60 + 120, transition(at(84), E, 3), one-shot(at(108), C, D, 8)
//     and the word becomes 0. Otherwise, unless cur is 8, one-shot(at(76), D, D, 16).
//  9: transition(at(84), A, 3).
// 10: when r % 3 is 0, a second roll q picks a part k = q % 6 + 2 (5 becomes 4, 7 becomes 6) for
//     loseBodyPart. dropTreasure with a copy of the position, the signed halfword at +56, 36.0, 2.0, 2.8, 0.0,
//     1.0, 0.25 and the text "fire_beetle_eye". A new roll then selects transition(header, D, 6) where the
//     header is the word at g + 36 + 8 * (roll & 3). When bit 1 of the word at +12 is set, animSetToEnd(anim).
//     Then virtual slot byte 192 on this object (adjusted).
// 13: the 64-bit word at +160 gains 0x100000; the float at +176 becomes 180.0; the word at +960 becomes
//     clock + 120; transition(at(68), E, 3); one-shot(at(116), C, D, 8).
// 14: transition(at(100), D, 3); one-shot(at(28), D, D, 8); the word at +964 becomes clock + 300 (stored before
//     the last roll is used); w256 = r % 20 + 20.
// Unknown: what the states, the fields at +256, +960, +964, +176 and the 64-bit flag bits mean.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* object);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animSetToEnd__FP14AnimationState(void* state);
void loseBodyPart__10FireBeetlei(void* self, int part);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int angle, float f12, float f13, float f14,
                                              float f15, float f16, float f17, char* name);
}

namespace {
using namespace vslots_b;
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
void* header(Address object, unsigned offset) { return pointer(read_word(address(pointer(read_word(object + 36u))) + offset)); }
void transition(Address object, unsigned offset, unsigned rate_bits, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset),
                                                               constant(rate_bits), flags);
}
void one_shot(Address object, unsigned offset, unsigned first_bits, unsigned second_bits, int flags) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, offset),
                                                           constant(first_bits), constant(second_bits), flags);
}
}  // namespace

extern "C" void setState__10FireBeetlei(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    const unsigned rate_half = 0x3f000000u, rate_one = 0x3f800000u, rate_small = 0x3c23d70au, rate_tenth = 0x3dccccccu,
                   rate_two = 0x40000000u;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned roll = eRand__10GameObject(self);
        write_word(object + 256u, read_word(object + 808u) + roll % 60u + 120u);
        transition(object, 84u, rate_half, 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char destination[12];
        getSmartRandomDestination__8Creaturei(destination, self, 300);
        for (unsigned i = 0; i < 12u; ++i) bytes(object + 272u)[i] = destination[i];
        if (read_i16(object + 572u) == 13) {
            write_u64(object + 160u, read_u64(object + 160u) & 0xffffffffffefffffull);
            transition(object, 124u, rate_one, 3);
            one_shot(object, 108u, rate_small, rate_tenth, 8);
        } else {
            transition(object, 124u, rate_tenth, 3);
        }
        write_word(object + 176u, 0x43200000u);
        break;
    }
    case 3u: {
        write_word(object + 176u, 0x43200000u);
        const short current = read_i16(object + 572u);
        if (current == 13) {
            write_u64(object + 160u, read_u64(object + 160u) & 0xffffffffffefffffull);
            transition(object, 124u, rate_one, 3);
            one_shot(object, 108u, rate_small, rate_tenth, 8);
        } else if (current != 3) {
            transition(object, 124u, rate_tenth, 3);
        }
        break;
    }
    case 5u:
        transition(object, 100u, rate_tenth, 3);
        break;
    case 6u: {
        transition(object, 100u, rate_tenth, 3);
        one_shot(object, 28u, rate_tenth, rate_tenth, 8);
        write_word(object + 256u, eRand__10GameObject(self) % 20u + 20u);
        break;
    }
    case 8u: {
        const short current = read_i16(object + 572u);
        if (current == 9 || (current == 6 && (read_u16(object + 294u) & 8u) != 0u)) {
            state = current;
        } else if (read_i16(object + 572u) == 13) {
            const unsigned roll = eRand__10GameObject(self);
            write_word(object + 256u, read_word(object + 808u) + roll % 60u + 120u);
            transition(object, 84u, rate_two, 3);
            one_shot(object, 108u, rate_small, rate_tenth, 8);
            state = 0;
        } else if (read_i16(object + 572u) != 8) {
            one_shot(object, 76u, rate_tenth, rate_tenth, 16);
        }
        break;
    }
    case 9u:
        transition(object, 84u, rate_half, 3);
        break;
    case 10u: {
        if (eRand__10GameObject(self) % 3u == 0u) {
            const unsigned roll = eRand__10GameObject(self);
            int part = static_cast<int>(roll % 6u) + 2;
            if (part == 5) part = 4;
            else if (part == 7) part = 6;
            loseBodyPart__10FireBeetlei(self, part);
        }
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = bytes(object + 16u)[i];
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), constant(0x42100000u),
                                                 constant(0x40000000u), constant(0x40333333u), constant(0u),
                                                 constant(0x3f800000u), constant(0x3e800000u),
                                                 const_cast<char*>("fire_beetle_eye"));
        const unsigned roll = eRand__10GameObject(self);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            bytes(object + 288u), pointer(read_word(address(pointer(read_word(object + 36u))) + (roll & 3u) * 8u + 36u)),
            constant(rate_tenth), 6);
        if ((read_word(object + 12u) & 2u) != 0u) animSetToEnd__FP14AnimationState(bytes(object + 288u));
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 13u: {
        write_u64(object + 160u, read_u64(object + 160u) | 0x100000ull);
        write_word(object + 176u, 0x43340000u);
        write_word(object + 960u, read_word(object + 808u) + 120u);
        transition(object, 68u, rate_two, 3);
        one_shot(object, 116u, rate_small, rate_tenth, 8);
        break;
    }
    case 14u: {
        transition(object, 100u, rate_tenth, 3);
        one_shot(object, 28u, rate_tenth, rate_tenth, 8);
        const unsigned roll = eRand__10GameObject(self);
        write_word(object + 964u, read_word(object + 808u) + 300u);
        write_word(object + 256u, roll % 20u + 20u);
        break;
    }
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
