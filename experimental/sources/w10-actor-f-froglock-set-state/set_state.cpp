#include "../vslots-b-bytes/bytes.hpp"
// Observed setState__8Froglocki at 0x00200ad8, 1548 bytes. The requested state is kept in a stack word;
// setStateProcessing is called with the object and that word's address, and a non-zero result returns without
// storing anything. Otherwise the word, which the call or the cases below may have changed, selects a path
// through a 16-word jump table at 0x46f1a0 (values 16 and above, and 2, 4, 11, 12, only store). Every path
// ends by storing the low 16 bits of the word at object+572. g is the word at object+36 (re-read before each
// use), anim the animation state at object+288, at(n) the word at g+n (an animation header), cur the signed
// halfword at +572 before the store, clock the word at +808, r an eRand(object) result (unsigned), P the signed
// byte at +568 and Q the signed byte at +1105 (only whether they are zero matters). Rates: S 0.1 (0x3dcccccc),
// T 0.3 (0x3e999999); every rate in the transitions of this function is S except where T is named.
// "pick" below means header at(116) when P and Q are both non-zero, otherwise at(124).
//  0: w256 = clock + r % 120 (stored before the call); transition(P != 0 ? at(84) : at(60), T, 3).
//  1: resetRouting; the 12-byte point from getSmartRandomDestination(object, 200) is stored at +272;
//     transition(pick, S, 2051); animGlobalSync(anim).
//  3: resetRouting; with w960 equal to 16: z = enemyInZone(object, 400.0, 6000); when P and Q are non-zero the
//     header is at(84) if z else at(116); otherwise at(76) if z else at(124). With w960 not 16 the header is
//     pick. transition(header, S, 2051); animGlobalSync(anim).
//  5: transition(w960 == 16 ? at(68) : at(76), S, 3).
//  6: one-shot(at(100), S, S, 8).
//  7: transition(pick, S, 2051); animGlobalSync(anim); w256 = clock + 180.
//  8: when cur is 9 or 15, or cur is 6 with bit 3 (8) of the unsigned halfword at +294 set, the word becomes
//     cur. Otherwise one-shot(at(52), S, S, 16).
//  9: transition(P != 0 ? at(84) : at(60), T, 3).
// 10: transition(the header at g + 36 + 8 * (r & 1), T, 6); dropTreasure with a copy of the position, the
//     signed halfword at +56, 36.0, 2.0, 1.8, 0.6, 2.0, 0.25 and a null name. When bit 1 of the word at +12 is
//     clear and the word at +960 is non-zero: modelGetCharAttachmentPos(the model at g+12, a copy of the
//     position, (short)(unsigned halfword at +56 - 16384), anim, 0, first, second, 1.0), then a 584-byte block
//     from blockAlloc; when it is non-null, the PhProp constructor runs on it with the first point, the words
//     at g and at g + 8 * w960, the word at that entry + 16, 1, 16385, null and the float at +224; then the
//     word at +960 is cleared (also when the allocation fails). Finally virtual slot byte 192 on this object
//     (adjusted).
// 13: transition(at(76), S, 3); one-shot(at(28), S, S, 8); w256 = r % 15.
// 14: transition(at(84), S, 3); one-shot(at(108), S, S, 8); w256 = r % 15.
// 15: transition(pick, S, 2051); one-shot(at(92), S, S, 2064).
// Unknown: what the states, the bytes P and Q, the fields at +256, +960 and +224, and the zone check mean.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* object);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
int enemyInZone__8Creaturefs(void* self, float distance, short count);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int angle, float f12, float f13, float f14,
                                              float f15, float f16, float f17, char* name);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const void* position, short angle, void* state, int index, void* out, void* other, float scale);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, const void* position, char* first, char* second, char* third,
                                                    long a, int b, void* description, float scale);
}

namespace {
using namespace vslots_b;
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
const unsigned rate_s = 0x3dccccccu;
const unsigned rate_t = 0x3e999999u;
void* header(Address object, unsigned offset) { return pointer(read_word(address(pointer(read_word(object + 36u))) + offset)); }
void transition(Address object, unsigned offset, unsigned rate_bits, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(bytes(object + 288u), header(object, offset),
                                                               constant(rate_bits), flags);
}
void one_shot(Address object, unsigned offset, int flags) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(bytes(object + 288u), header(object, offset),
                                                           constant(rate_s), constant(rate_s), flags);
}
bool both_set(Address object) { return read_u8(object + 568u) != 0u && read_u8(object + 1105u) != 0u; }
unsigned pick(Address object) { return both_set(object) ? 116u : 124u; }
}  // namespace

extern "C" void setState__8Froglocki(void* self, int requested) {
    const Address object = address(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (static_cast<unsigned>(state)) {
    case 0u: {
        const unsigned roll = eRand__10GameObject(self);
        write_word(object + 256u, read_word(object + 808u) + roll % 120u);
        if (read_u8(object + 568u) != 0u) transition(object, 84u, rate_t, 3);
        else transition(object, 60u, rate_t, 3);
        break;
    }
    case 1u: {
        resetRouting__8Creature(self);
        unsigned char destination[12];
        getSmartRandomDestination__8Creaturei(destination, self, 200);
        for (unsigned i = 0; i < 12u; ++i) bytes(object + 272u)[i] = destination[i];
        transition(object, pick(object), rate_s, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 3u: {
        resetRouting__8Creature(self);
        unsigned offset;
        if (read_word(object + 960u) == 16u) {
            const bool zone = enemyInZone__8Creaturefs(self, constant(0x43c80000u), 6000) != 0;
            if (both_set(object)) offset = zone ? 84u : 116u;
            else offset = zone ? 76u : 124u;
        } else {
            offset = pick(object);
        }
        transition(object, offset, rate_s, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        break;
    }
    case 5u:
        transition(object, read_word(object + 960u) == 16u ? 68u : 76u, rate_s, 3);
        break;
    case 6u:
        one_shot(object, 100u, 8);
        break;
    case 7u:
        transition(object, pick(object), rate_s, 2051);
        animGlobalSync__FP14AnimationState(bytes(object + 288u));
        write_word(object + 256u, read_word(object + 808u) + 180u);
        break;
    case 8u: {
        const short current = read_i16(object + 572u);
        if (current == 9 || current == 15 || (current == 6 && (read_u16(object + 294u) & 8u) != 0u)) state = current;
        else one_shot(object, 52u, 16);
        break;
    }
    case 9u:
        if (read_u8(object + 568u) != 0u) transition(object, 84u, rate_t, 3);
        else transition(object, 60u, rate_t, 3);
        break;
    case 10u: {
        const unsigned roll = eRand__10GameObject(self);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(
            bytes(object + 288u), pointer(read_word(address(pointer(read_word(object + 36u))) + (roll & 1u) * 8u + 36u)),
            constant(rate_t), 6);
        unsigned char point[12];
        for (unsigned i = 0; i < 12u; ++i) point[i] = bytes(object + 16u)[i];
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, read_i16(object + 56u), constant(0x42100000u),
                                                 constant(0x40000000u), constant(0x3fe66666u), constant(0x3f199999u),
                                                 constant(0x40000000u), constant(0x3e800000u), nullptr);
        if ((read_word(object + 12u) & 2u) == 0u && read_word(object + 960u) != 0u) {
            const Address group = address(pointer(read_word(object + 36u)));
            unsigned char first[12];
            unsigned char second[12];
            unsigned char start[12];
            for (unsigned i = 0; i < 12u; ++i) start[i] = bytes(object + 16u)[i];
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                pointer(read_word(group + 12u)), start,
                static_cast<short>(static_cast<int>(read_u16(object + 56u)) - 16384), bytes(object + 288u), 0, first,
                second, constant(0x3f800000u));
            void* block = blockAlloc__Fi(584);
            if (block != nullptr) {
                const Address groupAgain = address(pointer(read_word(object + 36u)));
                const Address entry = groupAgain + read_word(object + 960u) * 8u;
                __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                    block, first, static_cast<char*>(pointer(read_word(groupAgain))),
                    static_cast<char*>(pointer(read_word(entry))), static_cast<char*>(pointer(read_word(entry + 16u))), 1,
                    16385, nullptr, read_single(object + 224u));
            }
            write_word(object + 960u, 0u);
        }
        typedef void (*SlotCall)(void*);
        void* adjusted;
        SlotCall call = slot_function<SlotCall>(self, 192u, &adjusted);
        call(adjusted);
        break;
    }
    case 13u: {
        transition(object, 76u, rate_s, 3);
        one_shot(object, 28u, 8);
        write_word(object + 256u, eRand__10GameObject(self) % 15u);
        break;
    }
    case 14u: {
        transition(object, 84u, rate_s, 3);
        one_shot(object, 108u, 8);
        write_word(object + 256u, eRand__10GameObject(self) % 15u);
        break;
    }
    case 15u:
        transition(object, pick(object), rate_s, 2051);
        one_shot(object, 92u, 2064);
        break;
    default:
        break;
    }
    write_u16(object + 572u, static_cast<unsigned>(state));
}
