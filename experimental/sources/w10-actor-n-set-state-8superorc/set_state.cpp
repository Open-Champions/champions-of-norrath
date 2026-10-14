// Observed setState__8SuperOrci at 0x0022ad00, 960 bytes. R is the block at the word object+36,
// "anim" the animation state at +288. A copy of the argument n goes to
// setStateProcessing__8CreaturePi (which may change it); a non-zero result ends the call with
// nothing else done. Otherwise n selects a case (n >= 17 and 2, 4, 7, 11, 12 do nothing more) and
// the low halfword of n is stored at +572 at the end. "transition" is animAddTransitionTo(anim,
// header, time, flags), "oneShot" is animAddOneShot(anim, header, 0.1, 0.1, 0x808).
//  0, 9: transition(R[68], 0.3, 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) is copied to +272..283;
//     transition(R[92], 0.3, 0x803); animGlobalSync(anim).
//  3: resetRouting; transition(R[92], 0.3, 0x803); animGlobalSync(anim).
//  5: transition(R[60], 0.1, 3).
//  6: transition(R[60], 0.1, 3); oneShot(R[28]).
//  8: with s the halfword at +572: when s is 15 the table entry at (word +72)+184 is called with 16
//     and n becomes 16; otherwise n becomes s.
//  10: transition(R[100], 0.1, 12); dropTreasure(this, point +16, halfword +56, 36, 2, 3, 1, 3, 1,
//      no name); the table entry at (word +72)+192 is called; soundFadeStream(0).
//  13: when the word at +576 is non-zero it names a record whose word at +12 points at a block
//      holding a point at +16; the word at +964 is set to 1 when the distance in x and y between
//      the point at +16 and that point exceeds 120.0, otherwise 0. The word at +960 is then set to
//      0; transition(R[60], 0.1, 3); oneShot(R[44]).
//  14: transition(R[60], 0.1, 3); oneShot(R[36]).
//  15: transition(R[60], 0.1, 3); oneShot(R[84]).
//  16: transition(R[60], 0.1, 3); oneShot(R[52]).
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-n-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float time, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);
void soundFadeStream__Fi(int stream);

void setState__8SuperOrci(void* self, int state) {
    using namespace w10n;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    auto transition = [&](unsigned offset, float time, int flag) { animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, header(offset), time, flag); };
    auto one_shot = [&](unsigned offset) { animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), 0.1f, 0.1f, 0x808); };
    switch (n) {
        case 0:
        case 9:
            transition(68, 0.3f, 3);
            break;
        case 1: {
            resetRouting__8Creature(self);
            float point[3];
            getSmartRandomDestination__8Creaturei(point, self, 200);
            real(o, 272) = point[0]; real(o, 276) = point[1]; real(o, 280) = point[2];
            transition(92, 0.3f, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 3:
            resetRouting__8Creature(self);
            transition(92, 0.3f, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        case 5:
            transition(60, 0.1f, 3);
            break;
        case 6:
            transition(60, 0.1f, 3);
            one_shot(28);
            break;
        case 8: {
            const int s = half(o, 572);
            if (s == 15) {
                call_entry(o, 184, 16);
                n = 16;
            } else {
                n = s;
            }
            break;
        }
        case 10:
            transition(100, 0.1f, 12);
            dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), half(o, 56), 36.0f, 2.0f, 3.0f, 1.0f, 3.0f, 1.0f, nullptr);
            call_entry(o, 192);
            soundFadeStream__Fi(0);
            break;
        case 13:
            if (word(o, 576) != 0u) {
                unsigned char* const target = from_word(word(from_word(word(o, 576)), 12)) + 16;
                const float dy = real(o, 20) - real(target, 4);
                const float dx = real(o, 16) - real(target, 0);
                word(o, 964) = 120.0f < __builtin_sqrtf(dx * dx + dy * dy) ? 1u : 0u;
            }
            word(o, 960) = 0u;
            transition(60, 0.1f, 3);
            one_shot(44);
            break;
        case 14:
            transition(60, 0.1f, 3);
            one_shot(36);
            break;
        case 15:
            transition(60, 0.1f, 3);
            one_shot(84);
            break;
        case 16:
            transition(60, 0.1f, 3);
            one_shot(52);
            break;
        default:
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
