#include "../w10-actor-b-common/fields.hpp"
// Observed setState__8AntQueeni at 0x207898, 2068 bytes. The state goes to
// setStateProcessing(object, &state) first; a nonzero result ends the call. Otherwise an
// unsigned compare against 20 selects a block from a table at 0x46fce0 and the halfword at +572
// is finally set to the state. "Headers" are animation-header words of the record whose address
// is the word at +36; "anim" is the AnimationState at object+288. Singles are given by their
// bit patterns: S3 = 0x3e999999 (0.3), S1 = 0x3dcccccc (0.1), T = 0x3eaaaaab (one third).
//  0:  +256 = word at +808 + eRand % 120; transition(header +124, S3, flags 3).
//  1:  resetRouting; getSmartRandomDestination(200) copied to the point at +272; single at +176
//      = 140.0; transition(header +172, S3, 3).
//  3:  unless the current state (signed halfword +572) is 8, 6 or 19: resetRouting, single +176 =
//      140.0 and three animAddHairy calls (headers +172 {0, 0, T, 1, 0; 515}, +180 and +188
//      {0, 0, 0, 1, 0; 513}); in every case the 64-bit value at +160 gets 0x10000000 ORed in.
//  5:  transition(header +132, S1, 3).
//  6:  one-shot(header +28, S1, flags 8); word +964 = 0.
//  8:  keeps 13, 14, 15 or 9 when that is the current state, keeps 6 when the current state is 6
//      and bit 8 of the halfword at +294 is set; otherwise a one-shot (flags 16, rate S1)
//      chosen by the signed halfword at +748: [-8191, 8191] header +84, [8193, 24575] header +108,
//      [-24575, -8193] header +100, anything else header +92. The state stays 8 there.
//  9:  transition(header +124, S3, 3).
//  10: dropTreasure(point at +16, signed halfword +56, 36, 2, 5, 1, 5, 1, no name); transition
//      (header +196, 0.4, flags 22); when animGetAnimComponent(anim, header +60) is null a one-shot
//      (header +164, S1, 8); then the virtual entry at byte offset 192 of the table at +72.
//  13: single +176 = 240.0; +972 = word +808 + eRand % 120 + 180; resetRouting; the point at +272
//      = findEvadeDestination(); unless the current state (+572) is 14 or 15: three animAddHairy
//      (headers +60 {0, 0, T, 1, 0; 515}, +68, +76 {0, 0, 0, 1, 0; 513}) and a one-shot
//      (header +164, S1, 8).
//  14: single +176 = 240.0; resetRouting; unless the current state is 13 or 15: +972 = word +808
//      + eRand % 180 + 300 and the same four animations as state 13.
//  15, 16: one-shot(header +156, S1, 8); word +964 = 0.
//  17: +972 = word +808 + eRand % 180 + 180; transition(header +132, S1, 3); one-shot(header +148, S1, 8).
//  18: +976 = word +808 + eRand % 180 + 180; transition(header +132, S1, 3); one-shot(header +140,
//      S1, 8); +256 = word +808 + eRand % 60 + 120.
//  19: when the word at +576 is nonzero: alternateEnemyInZone(0, single +28 + 40, angle+32767, 6000)
//      (angle = halfword +56); with a result, the heading to it via iatan2 relative to the angle
//      picks header +36 (within [-2000, 2000]), +52 (above) or +44 (below); no result uses +36;
//      then a one-shot of that header (S1, flags 8).
//  2, 4, 7, 11, 12 and states of 20 or more: only the halfword store.
// Unknown: the state names and the fields at +160, +176, +256, +576, +748, +964, +972, +976.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(void* state, void* header, float a, float b, float c,
                                                             float d, float e, int flags);
void* animGetAnimComponent__FP14AnimationStateP15AnimationHeader(void* state, void* header);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(float* result, void* self, int range);
void findEvadeDestination__8AntQueen(float* result, void* self);
void* alternateEnemyInZone__8Creatureffss(void* self, float a, float b, short angle, short range);
int iatan2__Fff(float y, float x);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const float* point, int angle, float a, float b, float c,
                                              float d, float e, float f, char* name);
}

namespace {
using namespace actor_b;
using rtti_c::single_from_bits;
typedef unsigned long long Double __attribute__((may_alias));

__attribute__((always_inline)) inline void* header(void* self, unsigned offset) {
    return link(link(self, 36), offset);
}
__attribute__((always_inline)) inline void* anim(void* self) { return static_cast<unsigned char*>(self) + 288; }
__attribute__((always_inline)) inline void one_shot(void* self, void* h, float rate, int flags) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim(self), h, rate, rate, flags);
}
__attribute__((always_inline)) inline void transition(void* self, unsigned offset, float rate, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim(self), header(self, offset), rate, flags);
}
__attribute__((always_inline)) inline void copy_point(void* self, const float* point) {
    const Word* bits = reinterpret_cast<const Word*>(point);
    field<Word>(self, 272) = bits[0];
    field<Word>(self, 276) = bits[1];
    field<Word>(self, 280) = bits[2];
}
// The three-part "hairy" sequence used by the dodge states; the first header's third single is a third.
__attribute__((always_inline)) inline void hairy_sequence(void* self, unsigned first, unsigned second, unsigned third, int first_flags) {
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(anim(self), header(self, first), 0.0f, 0.0f, single_from_bits(0x3eaaaaabu), 1.0f, 0.0f, first_flags);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(anim(self), header(self, second), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
    animAddHairy__FP14AnimationStateP15AnimationHeaderfffffi(anim(self), header(self, third), 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 513);
}
}

extern "C" void setState__8AntQueeni(void* self, int state) {
    const float s3 = single_from_bits(0x3e999999u);
    const float s1 = single_from_bits(0x3dccccccu);
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (state) {
    case 0:
        field<Word>(self, 256) = field<Word>(self, 808) + eRand__10GameObject(self) % 120u;
        transition(self, 124, s3, 3);
        break;
    case 1: {
        resetRouting__8Creature(self);
        float destination[3] __attribute__((may_alias));
        getSmartRandomDestination__8Creaturei(destination, self, 200);
        copy_point(self, destination);
        field<Single>(self, 176) = single_from_bits(0x430c0000u);
        transition(self, 172, s3, 3);
        break;
    }
    case 3: {
        const int current = field<SHalf>(self, 572);
        if (current != 8 && current != 6 && current != 19) {
            resetRouting__8Creature(self);
            field<Single>(self, 176) = single_from_bits(0x430c0000u);
            hairy_sequence(self, 172, 180, 188, 515);
        }
        field<Double>(self, 160) = field<Double>(self, 160) | 0x10000000ull;
        break;
    }
    case 5:
        transition(self, 132, s1, 3);
        break;
    case 6:
        one_shot(self, header(self, 28), s1, 8);
        field<Word>(self, 964) = 0;
        break;
    case 8: {
        const unsigned current = field<Half>(self, 572);
        if (current - 13u < 3u) {
            state = static_cast<short>(current);
            break;
        }
        if (static_cast<short>(current) == 9) {
            state = 9;
            break;
        }
        if (static_cast<short>(current) == 6 && (field<Half>(self, 294) & 8u) != 0) {
            state = 6;
            break;
        }
        const unsigned angle = field<Half>(self, 748);
        if (static_cast<unsigned short>(angle + 8191u) < 16383u) {
            one_shot(self, header(self, 84), s1, 16);
        } else if (static_cast<unsigned short>(angle - 8193u) < 16383u) {
            one_shot(self, header(self, 108), s1, 16);
        } else {
            const int signed_angle = static_cast<short>(angle);
            if (signed_angle < -8192 && !(signed_angle < -24575)) one_shot(self, header(self, 100), s1, 16);
            else one_shot(self, header(self, 92), s1, 16);
        }
        break;
    }
    case 9:
        transition(self, 124, s3, 3);
        break;
    case 10: {
        float point[3] __attribute__((may_alias));
        const Word* source = &field<Word>(self, 16);
        Word* copy = reinterpret_cast<Word*>(point);
        copy[0] = source[0];
        copy[1] = source[1];
        copy[2] = source[2];
        dropTreasure__8CreatureG6Point3iffffffPc(self, point, field<SHalf>(self, 56), single_from_bits(0x42100000u), 2.0f, 5.0f, 1.0f, 5.0f, 1.0f, nullptr);
        transition(self, 196, single_from_bits(0x3ecccccc), 22);
        if (animGetAnimComponent__FP14AnimationStateP15AnimationHeader(anim(self), header(self, 60)) == nullptr) {
            one_shot(self, header(self, 164), s1, 8);
        }
        void* adjusted;
        rtti_c::slot_function<void (*)(void*)>(self, 192, &adjusted)(adjusted);
        break;
    }
    case 13: {
        field<Single>(self, 176) = single_from_bits(0x43700000u);
        field<Word>(self, 972) = field<Word>(self, 808) + eRand__10GameObject(self) % 120u + 180u;
        resetRouting__8Creature(self);
        float destination[3] __attribute__((may_alias));
        findEvadeDestination__8AntQueen(destination, self);
        copy_point(self, destination);
        if (field<Half>(self, 572) - 14u < 2u) break;
        hairy_sequence(self, 60, 68, 76, 515);
        one_shot(self, header(self, 164), s1, 8);
        break;
    }
    case 14: {
        field<Single>(self, 176) = single_from_bits(0x43700000u);
        resetRouting__8Creature(self);
        const int current = field<SHalf>(self, 572);
        if (current == 13 || current == 15) break;
        field<Word>(self, 972) = field<Word>(self, 808) + eRand__10GameObject(self) % 180u + 300u;
        hairy_sequence(self, 60, 68, 76, 515);
        one_shot(self, header(self, 164), s1, 8);
        break;
    }
    case 15:
    case 16:
        one_shot(self, header(self, 156), s1, 8);
        field<Word>(self, 964) = 0;
        break;
    case 17:
        field<Word>(self, 972) = field<Word>(self, 808) + eRand__10GameObject(self) % 180u + 180u;
        transition(self, 132, s1, 3);
        one_shot(self, header(self, 148), s1, 8);
        break;
    case 18:
        field<Word>(self, 976) = field<Word>(self, 808) + eRand__10GameObject(self) % 180u + 180u;
        transition(self, 132, s1, 3);
        one_shot(self, header(self, 140), s1, 8);
        field<Word>(self, 256) = field<Word>(self, 808) + eRand__10GameObject(self) % 60u + 120u;
        break;
    case 19: {
        if (field<Word>(self, 576) == 0) break;
        int slot = 4;
        void* enemy = alternateEnemyInZone__8Creatureffss(self, 0.0f, field<Single>(self, 28) + 40.0f,
                                                          static_cast<short>(field<Half>(self, 56) - 32767u), 6000);
        if (enemy != nullptr) {
            const float dy = field<Single>(enemy, 20) - field<Single>(self, 20);
            const float dx = field<Single>(enemy, 16) - field<Single>(self, 16);
            const int heading = iatan2__Fff(dy, dx);
            const int difference = static_cast<short>(heading - field<Half>(self, 56) + 32767);
            if (difference >= 2001) slot = 6;
            if (difference < -2000) slot = 5;
        }
        one_shot(self, header(self, 8 * slot + 4), s1, 8);
        break;
    }
    default:
        break;
    }
    field<Half>(self, 572) = static_cast<unsigned short>(state);
}
