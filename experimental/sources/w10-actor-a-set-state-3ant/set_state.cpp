// Observed setState__3Anti at 0x00206628, 1100 bytes.
// State transition for Ant. The new state is held in a stack word whose address goes to the
// Creature state processing call (setStateProcessing__8CreaturePi), which may change it; a
// nonzero result returns at once without storing. Otherwise:
//  - If the signed byte at +968 is 2 and the state is not one of 0, 5, 6, 8 or 10, the state
//    becomes 5.
//  - A state of 14 or more only reaches the final store. Otherwise the state selects one case
//    (t is the word at +36, a table of animation header pointers; a is the member at +288):
//      0: +256 = word(+808) + eRand % 120; transition to header t[84] (0.3, 3).
//      1: reset routing; take a random destination (200) and copy it to +272; float +176 =
//         140.0; transition to t[108] (0.3, 3); global sync of a.
//      3: reset routing; float +176 = 140.0; transition to t[108] (0.3, 3); global sync.
//      5: transition to t[100] (0.1, 3).
//      6: transition to t[100] (0.1, 3); one-shot (0.1, 0.1, 8) of t[36], or when the byte at
//         +968 is 1 of t[36 + 8 * (eRand & 1)]; word +964 = 0; +256 = eRand % 15.
//      8: when the halfword +572 is 6 with bit 8 of halfword +294 set, or is 9, the state is
//         (short)halfword(+572); otherwise a one-shot (0.1, 0.1, 16) of t[76].
//      9: transition to t[84] (0.3, 3).
//     10: if the byte at +968 is 2, bit 0 of word +12 is cleared; word +976 = 0; when eRand % 3
//         is 0, loseBodyPart(eRand & 7); dropTreasure with the 12 bytes at +16, the halfword
//         at +56, zero, floats (36, 2, 1, 0.1, 1.2, 0.25) and a null name; transition to
//         t[52 + 8 * (eRand % 3)] (0.3, 6); then the table entry at (word +72) + 192 is called
//         with the object adjusted by its signed halfword.
//     13: one-shot (0.1, 0.1, 8) of t[116]; +980 = word(+808) + 180 + eRand % 60.
//      others (2, 4, 7, 11, 12): nothing.
//  - Finally the halfword at +572 is set to the state.
// Inferred: the table entries are animation headers from the callee types. Unknown: what the
// states, bytes and words mean. Nothing here ran on the EE.
struct Point3 { float x, y, z; };

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(Point3* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float blend, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void loseBodyPart__3Anti(void* self, int part);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* name);
}

#if defined(W10_ACTOR_A_HOST)
// Host builds keep 32-bit words as offsets from a base and function words as indexes.
extern unsigned char* w10_actor_a_host_base;
void w10_actor_a_host_call(unsigned function_word, void* object);
#endif

namespace {
unsigned char* from_word(unsigned value) {
#if defined(W10_ACTOR_A_HOST)
    return w10_actor_a_host_base + value;
#else
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(value));
#endif
}
void call_word(unsigned function_word, void* object) {
#if defined(W10_ACTOR_A_HOST)
    w10_actor_a_host_call(function_word, object);
#else
    reinterpret_cast<void (*)(void*)>(static_cast<__UINTPTR_TYPE__>(function_word))(object);
#endif
}
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
void* entry(unsigned char* o, unsigned offset) {
    unsigned char* table = from_word(word(o, 36));
    return from_word(*reinterpret_cast<unsigned*>(table + offset));
}
}  // namespace

extern "C" void setState__3Anti(void* self, int state) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* a = o + 288;
    if (setStateProcessing__8CreaturePi(self, &state)) return;
    if (static_cast<signed char>(o[968]) == 2) {
        if (state != 0 && state != 5 && state != 6 && state != 8 && state != 10) state = 5;
    }
    if (static_cast<unsigned>(state) < 14) {
        switch (state) {
        case 0: {
            unsigned r = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + r % 120;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, entry(o, 84), 0.3f, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            Point3 destination;
            getSmartRandomDestination__8Creaturei(&destination, self, 200);
            *reinterpret_cast<Point3*>(o + 272) = destination;
            *reinterpret_cast<float*>(o + 176) = 140.0f;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, entry(o, 108), 0.3f, 3);
            animGlobalSync__FP14AnimationState(a);
            break;
        }
        case 3:
            resetRouting__8Creature(self);
            *reinterpret_cast<float*>(o + 176) = 140.0f;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, entry(o, 108), 0.3f, 3);
            animGlobalSync__FP14AnimationState(a);
            break;
        case 5:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, entry(o, 100), 0.1f, 3);
            break;
        case 6:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, entry(o, 100), 0.1f, 3);
            if (static_cast<signed char>(o[968]) == 1) {
                unsigned r = eRand__10GameObject(self);
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, entry(o, 36 + 8 * (r & 1)), 0.1f, 0.1f, 8);
            } else {
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, entry(o, 36), 0.1f, 0.1f, 8);
            }
            {
                unsigned r = eRand__10GameObject(self);
                word(o, 964) = 0;
                word(o, 256) = r % 15;
            }
            break;
        case 8: {
            short current = *reinterpret_cast<short*>(o + 572);
            if ((current == 6 && (*reinterpret_cast<unsigned short*>(o + 294) & 8) != 0) || current == 9) {
                state = current;
            } else {
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, entry(o, 76), 0.1f, 0.1f, 16);
            }
            break;
        }
        case 9:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, entry(o, 84), 0.3f, 3);
            break;
        case 10: {
            if (static_cast<signed char>(o[968]) == 2) word(o, 12) &= ~1u;
            word(o, 976) = 0;
            if (eRand__10GameObject(self) % 3 == 0) loseBodyPart__3Anti(self, static_cast<int>(eRand__10GameObject(self) & 7));
            dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), *reinterpret_cast<short*>(o + 56), 36.0f, 2.0f, 1.0f, 0.1f, 1.2f, 0.25f, nullptr);
            unsigned r = eRand__10GameObject(self);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, entry(o, 52 + 8 * (r % 3)), 0.3f, 6);
            unsigned char* table = from_word(word(o, 72)) + 192;
            short adjust = *reinterpret_cast<short*>(table);
            call_word(*reinterpret_cast<unsigned*>(table + 4), o + adjust);
            break;
        }
        case 13: {
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, entry(o, 116), 0.1f, 0.1f, 8);
            unsigned r = eRand__10GameObject(self);
            word(o, 980) = word(o, 808) + r % 60 + 180;
            break;
        }
        default:
            break;
        }
    }
    *reinterpret_cast<short*>(o + 572) = static_cast<short>(state);
}
