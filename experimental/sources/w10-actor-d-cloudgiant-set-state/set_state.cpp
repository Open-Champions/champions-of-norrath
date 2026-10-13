// Observed setState__10CloudGianti at 0x001e8d38, 1448 bytes.
// Steps, in order (o is the object, m the pointer stored at o+36, A the AnimationState at o+288):
//  1. soundStopSound__Fi(word o+1308); word o+1308 = -1.
//  2. setStateProcessing__8CreaturePi(o, &state) with the state in a stack int; a nonzero result
//     returns at once (the halfword at +572 is then not written).
//  3. The state read back from the stack selects a case with an unsigned compare against 19; the
//     states 2, 4, 7, 9, 11, 12 and every value above 18 do nothing. Every path ends with the
//     halfword at +572 = the low half of that stack int.
//   0: eRand; word +256 = word +808 + (roll % 120); animAddTransitionTo(A, word m+132, 0.3, 3).
//   1: resetRouting; getSmartRandomDestination(result, o, 200) copied to +272..283; eRand; word
//      +256 = word +808 + (roll % 120) + 300; animAddTransitionTo(A, word m+76 if word +980 is
//      nonzero else m+172, 0.3, 2051); animGlobalSync(A).
//   3: resetRouting; unless the halfword +572 is 3, 6, 14 or 15, animAddTransitionTo(A, word m+76
//      if word +980 is nonzero else m+172, 0.1, 2051) and animGlobalSync(A). Then, if word +976 is
//      nonzero: the 15 bytes "CloudGiantKing" plus terminator go to bossMeterName, bossMeterMax =
//      (int)float +564 and bossMeterCurrent = (int)float +560.
//   5: animAddTransitionTo(A, word m+140, 0.1, 3).
//   6: eRand; animAddOneShot(A, word m+100+8*(roll&1), 0.1, 0.1, 2056).
//   8: eRand; animAddOneShot(A, word m+116+8*(roll&1), 0.1, 0.1, 16).
//   10: eRand; animAddTransitionTo(A, word m+84+8*(roll&1), 0.3, 6); dropTreasure(o, copy of the
//      Point3 at +16, (short)+56, 36.0, 2.0, 2.4, 0.6, 4.0, 0.75, null); word +980 = 0; word +1304
//      = 0; if bit 2 of word +12 is clear: clear bit 0 of it and, when word +984 is nonzero,
//      modelGetCharAttachmentPos(word m+12, copy of the Point3 at +16, (short)(halfword +56 -
//      16384), A, 0, P, Q, 1.0), then (allocation of 584 bytes permitting) construct a PhProp at P
//      with the char* words m+0, m+192, m+200, 1, 16385, null and the float +224; word +984 = 0.
//      Last, virtual slot byte 192 of the table at +72 is called (no second argument).
//   13: animAddTransitionTo(A, word m+44, 0.1, 3); animAddOneShot(A, word m+36, 0.1, 0.1, 8); eRand;
//      byte +964 = 0; word +968 = word +808 + 240; word +256 = roll % 20.
//   14: animAddOneShot(A, word m+156, 0.1, 0.1, 2056).     15: m+60 if word +980 is nonzero else
//      m+164, 0.1, 0.1, 2056.     16: m+148, 0.1, 0.1, 2056.     17: m+68.     18: m+52 (all 0.1).
// The single constants are the ones at gp-28316..gp-28248 (0.3, 0.1 and so on, as written).
// Inferred: this is the CloudGiant boss state machine and slot 192 is death; the PhProp is the
// attachment (a dropped item or weapon) the body spawns when it dies. Unknown: the names of the
// states and of most fields. Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

struct Point3 {
    float x, y, z;
};

extern "C" {
void soundStopSound__Fi(int value);
int setStateProcessing__8CreaturePi(void* self, int* state);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float in, float out, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
Point3* getSmartRandomDestination__8Creaturei(Point3* result, void* self, int radius);
int eRand__10GameObject(void* self);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const Point3* position, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* name);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, const Point3* position, short type, void* state, int a, Point3* out, Point3* second, float scale);
void* blockAlloc__Fi(int size);
void phProp_construct(void* self, const Point3* position, char* a, char* b, char* c, int d, int e, void* description, float f)
    __asm__("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
extern char bossMeterName[];
extern int bossMeterCurrent;
extern int bossMeterMax;
}

namespace {
template <typename T>
inline T& field(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }
inline unsigned char* block(unsigned char* o) { return w10_actor_d::word_at(o, 36); }
inline void* header(unsigned char* o, unsigned offset) { return w10_actor_d::word_at(block(o), offset); }
}  // namespace

extern "C" void setState__10CloudGianti(void* self, int newState) {
    unsigned char* o = static_cast<unsigned char*>(self);
    int state = newState;
    void* const A = o + 288;
    soundStopSound__Fi(field<int>(o, 1308));
    field<int>(o, 1308) = -1;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    switch (static_cast<unsigned>(state)) {
        case 0: {
            const int roll = eRand__10GameObject(self);
            field<int>(o, 256) = field<int>(o, 808) + static_cast<int>(static_cast<unsigned>(roll) % 120u);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 132), 0.3f, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            Point3 destination;
            getSmartRandomDestination__8Creaturei(&destination, self, 200);
            field<Point3>(o, 272) = destination;
            const int roll = eRand__10GameObject(self);
            field<int>(o, 256) = field<int>(o, 808) + static_cast<int>(static_cast<unsigned>(roll) % 120u) + 300;
            if (field<int>(o, 980) != 0)
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 76), 0.3f, 2051);
            else
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 172), 0.3f, 2051);
            animGlobalSync__FP14AnimationState(A);
            break;
        }
        case 3: {
            resetRouting__8Creature(self);
            const short current = field<short>(o, 572);
            if (current != 3 && current != 6 && current != 14 && current != 15) {
                if (field<int>(o, 980) != 0)
                    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 76), 0.1f, 2051);
                else
                    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 172), 0.1f, 2051);
                animGlobalSync__FP14AnimationState(A);
            }
            if (field<int>(o, 976) != 0) {
                static const char bossName[15] = {'C', 'l', 'o', 'u', 'd', 'G', 'i', 'a', 'n', 't', 'K', 'i', 'n', 'g', '\0'};
                for (int i = 0; i < 15; ++i) bossMeterName[i] = bossName[i];
                bossMeterMax = static_cast<int>(field<float>(o, 564));
                bossMeterCurrent = static_cast<int>(field<float>(o, 560));
            }
            break;
        }
        case 5:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 140), 0.1f, 3);
            break;
        case 6: {
            const int roll = eRand__10GameObject(self);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, 100 + 8 * (roll & 1)), 0.1f, 0.1f, 2056);
            break;
        }
        case 8: {
            const int roll = eRand__10GameObject(self);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, 116 + 8 * (roll & 1)), 0.1f, 0.1f, 16);
            break;
        }
        case 10: {
            const int roll = eRand__10GameObject(self);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 84 + 8 * (roll & 1)), 0.3f, 6);
            const Point3 positionCopy = field<Point3>(o, 16);
            dropTreasure__8CreatureG6Point3iffffffPc(self, &positionCopy, field<short>(o, 56), 36.0f, 2.0f, 2.4f, 0.6f, 4.0f, 0.75f, nullptr);
            field<int>(o, 980) = 0;
            field<int>(o, 1304) = 0;
            const unsigned flags = field<unsigned>(o, 12);
            if ((flags & 2u) == 0) {
                field<unsigned>(o, 12) = flags & 0xfffffffeu;
                if (field<int>(o, 984) != 0) {
                    Point3 attach;
                    Point3 second;
                    const Point3 attachCopy = field<Point3>(o, 16);
                    attach = field<Point3>(o, 16);
                    modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                        w10_actor_d::word_at(block(o), 12), &attachCopy, static_cast<short>(field<unsigned short>(o, 56) - 16384), A, 0, &attach, &second, 1.0f);
                    void* prop = blockAlloc__Fi(584);
                    if (prop != nullptr)
                        phProp_construct(prop, &attach, reinterpret_cast<char*>(w10_actor_d::word_at(block(o), 0)), reinterpret_cast<char*>(w10_actor_d::word_at(block(o), 192)),
                                         reinterpret_cast<char*>(w10_actor_d::word_at(block(o), 200)), 1, 16385, nullptr, field<float>(o, 224));
                    field<int>(o, 984) = 0;
                }
            }
            w10_actor_d::call_slot(self, 192, 0);
            break;
        }
        case 13:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header(o, 44), 0.1f, 3);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, 36), 0.1f, 0.1f, 8);
            {
                const int roll = eRand__10GameObject(self);
                o[964] = 0;
                field<int>(o, 968) = field<int>(o, 808) + 240;
                field<int>(o, 256) = static_cast<int>(static_cast<unsigned>(roll) % 20u);
            }
            break;
        case 14:
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, 156), 0.1f, 0.1f, 2056);
            break;
        case 15:
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, field<int>(o, 980) != 0 ? 60 : 164), 0.1f, 0.1f, 2056);
            break;
        case 16:
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, 148), 0.1f, 0.1f, 2056);
            break;
        case 17:
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, 68), 0.1f, 0.1f, 2056);
            break;
        case 18:
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header(o, 52), 0.1f, 0.1f, 2056);
            break;
        default:
            break;
    }
    field<unsigned short>(o, 572) = static_cast<unsigned short>(state);
}
