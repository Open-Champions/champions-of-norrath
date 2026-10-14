// Observed setState__5Ghouli at 0x001f6e20, 1624 bytes. The new state S is held in a stack word whose
// address goes to the Creature state processing call (setStateProcessing__8CreaturePi), which may
// change it; a nonzero result returns at once. Otherwise states 0..17 (unsigned) select a case through
// the table at 0x46e770; t is the table at word +36 (animation header pointers) and a is the member
// at +288. Blends are written as in the image: B3 = the single with bits 0x3e999999 (just under 0.3)
// and B1 = bits 0x3dcccccc (just under 0.1).
//   0: word +256 = word(+808) + eRand % 90 + 90 (stored first); transition t[180] (B3, 3).
//   1: float +176 = 170.0; reset routing; random destination (range 300) copied to +272..283;
//      transition t[52] (B3, 3); global sync of a.
//   3: float +176 = 170.0; when the halfword +572 is not 3, transition t[52] (B3, 3); global sync of a.
//   5: transition t[60] (B1, 3).
//   6: transition t[60] (B1, 3); one-shot t[28 + 8 * (eRand & 1)] (B1, B1, 8).
//   8: when the halfword +572 (H) is 9: S = 9. Else when H is 6 or 17 and bit 3 (0x8) of halfword +294
//      is set: S = H. Else when H is 15: S = 15. Else, with R = eRand: R % 3 == 0 gives transition
//      t[220] (1.0, 3), one-shot t[124] (B1, B1, 20), word +960 = word(+808) + eRand % 180 + 120 and
//      S = 15; any other R gives a one-shot t[132] (B1, B1, 16).
//   9: transition t[180] (B3, 3).
//  10: when bit 1 (0x2) of word +12 is clear, bit 0 is cleared; byte +40 = 60; dropTreasure with the 12
//      bytes at +16, the halfword +56, floats (36, 2, 1.6, 0.6, 1.4, 0.25) and the text "ghoulflesh";
//      then when bit 1 of word +12 is set transition t[68] (B3, 6), otherwise t[124] (B3, 14); then
//      the table entry at (word +72) + 192 is called with the adjusted object.
//  13: float +176 = 170.0; reset routing; transition t[52] (B3, 3); global sync of a.
//  14: transition t[196] (B1, 3); word +256 = word(+808) + eRand % 240 + 240.
//  16: float +176 = 200.0; word +968 = 0 and transition t[52] (B1, 3); eRand R; findClearPath with the
//      12 bytes at +16, the halfword +56, R & 1 and 3.0 gives a halfword stored at +964; D = the
//      signed 16-bit difference of it and the halfword +56. Index I = 18 when -8192 <= D < 8192 or
//      D < -24575; 19 when 8192 <= D < 24576; 17 when D >= 24576; 20 otherwise. When I is 17 float
//      +176 = 250.0. One-shot t[4 + 8 * I] (0.01, B1, 8).
//  17: transition t[60] (B1, 3); one-shot t[44] (B1, B1, 8); word +256 = 20.
//   2, 4, 7, 11, 12, 15 and states of 18 or more: nothing.
// Finally: when S is 15 bit 0 of word +12 is set; otherwise, when the stored halfword +572 is 15 that
// bit is cleared; the halfword +572 is then set to the low 16 bits of S.
// Inferred: 15 is a state with a flag in word +12; the +176 float and +256/+960 words are speed and
// timer values. Unknown: what the states mean. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(Point3* result, void* self, int range);
short findClearPath__8CreatureG6Point3scf(void* self, Point3 position, short heading, char flag, float range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float blend, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* name);
}

namespace {
const float kB3 = 0x1.333332p-2f;
const float kB1 = 0x1.999998p-4f;

void* header(void* object, unsigned offset) {
    unsigned char* table = w10g::from_word(w10g::word_at(object, 36));
    return w10g::from_word(*reinterpret_cast<unsigned*>(table + offset));
}
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
}  // namespace

extern "C" void setState__5Ghouli(void* self, int state) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* a = o + 288;
    if (setStateProcessing__8CreaturePi(self, &state)) return;
    if (static_cast<unsigned>(state) < 18u) {
        switch (state) {
        case 0: {
            const unsigned roll = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + roll % 90u + 90u;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 180), kB3, 3);
            break;
        }
        case 1: {
            *reinterpret_cast<float*>(o + 176) = 170.0f;
            resetRouting__8Creature(self);
            Point3 destination;
            getSmartRandomDestination__8Creaturei(&destination, self, 300);
            *reinterpret_cast<Point3*>(o + 272) = destination;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 52), kB3, 3);
            animGlobalSync__FP14AnimationState(a);
            break;
        }
        case 13:
            *reinterpret_cast<float*>(o + 176) = 170.0f;
            resetRouting__8Creature(self);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 52), kB3, 3);
            animGlobalSync__FP14AnimationState(a);
            break;
        case 3:
            *reinterpret_cast<float*>(o + 176) = 170.0f;
            if (*reinterpret_cast<short*>(o + 572) != 3) {
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 52), kB3, 3);
            }
            animGlobalSync__FP14AnimationState(a);
            break;
        case 16: {
            *reinterpret_cast<float*>(o + 176) = 200.0f;
            word(o, 968) = 0;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 52), kB1, 3);
            const unsigned roll = eRand__10GameObject(self);
            const short path = findClearPath__8CreatureG6Point3scf(self, *reinterpret_cast<Point3*>(o + 16),
                                                                   *reinterpret_cast<short*>(o + 56),
                                                                   static_cast<char>(roll & 1u), 3.0f);
            *reinterpret_cast<short*>(o + 964) = path;
            const short d = static_cast<short>(static_cast<unsigned>(path) - *reinterpret_cast<unsigned short*>(o + 56));
            unsigned index;
            if (d >= -8192) {
                if (d < 8192) index = 18;
                else index = (static_cast<unsigned>(d) - 8192u) < 16384u ? 19u : 17u;
            } else {
                index = d >= -24575 ? 20u : 18u;
            }
            if (index == 17) *reinterpret_cast<float*>(o + 176) = 250.0f;
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 4 + 8 * index), 0.01f, kB1, 8);
            break;
        }
        case 14: {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 196), kB1, 3);
            const unsigned roll = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + roll % 240u + 240u;
            break;
        }
        case 5:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 60), kB1, 3);
            break;
        case 6: {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 60), kB1, 3);
            const unsigned roll = eRand__10GameObject(self);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 28 + 8 * (roll & 1u)), kB1, kB1, 8);
            break;
        }
        case 17:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 60), kB1, 3);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 44), kB1, kB1, 8);
            word(o, 256) = 20;
            break;
        case 9:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 180), kB3, 3);
            break;
        case 8: {
            const short current = *reinterpret_cast<short*>(o + 572);
            const bool bit3 = (*reinterpret_cast<unsigned short*>(o + 294) & 0x8u) != 0;
            if (current == 9) {
                state = 9;
            } else if (current == 6 && bit3) {
                state = 6;
            } else if (current == 17 && bit3) {
                state = 17;
            } else if (current == 15) {
                state = 15;
            } else {
                const unsigned roll = eRand__10GameObject(self);
                if (roll % 3u == 0) {
                    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 220), 1.0f, 3);
                    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 124), kB1, kB1, 20);
                    const unsigned second = eRand__10GameObject(self);
                    state = 15;
                    word(o, 960) = word(o, 808) + second % 180u + 120u;
                } else {
                    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 132), kB1, kB1, 16);
                }
            }
            break;
        }
        case 10: {
            if ((word(o, 12) & 2u) == 0) word(o, 12) = word(o, 12) & ~1u;
            o[40] = 60;
            char name[] = "ghoulflesh";
            dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), *reinterpret_cast<short*>(o + 56),
                                                     36.0f, 2.0f, 0x1.999998p+0f, 0x1.333332p-1f, 1.4f, 0.25f, name);
            if ((word(o, 12) & 2u) != 0) {
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 68), kB3, 6);
            } else {
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 124), kB3, 14);
            }
            w10g::fn_from_word<void (*)(void*)>(w10g::entry_function(self, 192))(w10g::adjusted(self, 192));
            break;
        }
        default:
            break;
        }
    }
    if (state == 15) {
        word(o, 12) = word(o, 12) | 1u;
    } else if (*reinterpret_cast<short*>(o + 572) == 15) {
        word(o, 12) = word(o, 12) & ~1u;
    }
    *reinterpret_cast<unsigned short*>(o + 572) = static_cast<unsigned short>(state);
}
