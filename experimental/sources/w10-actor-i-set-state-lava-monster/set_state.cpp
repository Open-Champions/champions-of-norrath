// Observed setState__11LavaMonsteri at 0x001ed200, 992 bytes.
// The state is copied to a local word whose address goes to setStateProcessing__8CreaturePi(object,
// &state); a nonzero result returns at once, leaving the stored state alone. Otherwise the local state
// selects an action (a jump table of 16 entries, found at 0x46d810; 2, 4, 7, 11, 12, 13 and states
// of 16 or more do nothing). "block" is the resource block pointed to by the word at object+36 and
// "anim" the animation state at object+288. t(h, x, n) is animAddTransitionTo(anim, h, x, n) and
// o(h, x, y, n) is animAddOneShot(anim, h, x, y, n). Singles are the image's bit patterns: A is
// 0x3dcccccc (0.1), B is 0x3e4ccccc (0.2).
//   0: with the halfword at +572 equal to 13: t(block[84], 0.5, 3) then o(block[108], 0.0, A, 32);
//      otherwise t(block[84], A, 3). Then the word at +256 becomes the word at +808 plus eRand % 120.
//   1: resetRouting; a point from getSmartRandomDestination(object, 200) is stored at +272..+283;
//      t(block[212], A, 2051); animGlobalSync(anim).
//   3: resetRouting; when (float)(int)word(+964) is below the single at +560 and the signed byte at
//      +568 is nonzero: t(block[84], B, 3); otherwise t(block[212], B, 2051). Then animGlobalSync.
//   5 and 9: t(block[84], A, 3).
//   6: o(block[8 * (eRand % 3) + 156], B, A, 8).
//   8: with the current state (halfword +572) 6 and bit 8 of the halfword at +294 set, or 9 or 15, the
//      local state becomes the current state (the change is refused); otherwise o(block[60], A, A, 16).
//   10: dropTreasure(object, copy of the point at +16, halfword +56, 0, 36.0, 2.0, 2.8, 0.0, 3.0, 0.25)
//      with a null final pointer; then j = eRand & 1 is stored at +1052, t(block[8 * j + 44], A, 6)
//      and virtual slot 192 (entry at table+192: adjustment halfword, function word at +4) runs on the
//      adjusted object.
//   14: o(block[8 * (eRand & 1) + 140], B, A, 2056).
//   15: o(block[196], B, A, 2056).
// Finally the local state is stored as the halfword at +572, and unless it is 0 or 13 (as a signed
// halfword) bit 0x800 of the 64-bit word at +160 is cleared.
// Inferred: bits and block indexes name animation slots; slot 192 runs after a death-like state 10
// that drops treasure. Unknown: what the states mean. Nothing here ran on the EE.
extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* animation, void* header, float amount, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* animation, void* header, float first, float second, int flags);
void animGlobalSync__FP14AnimationState(void* animation);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* point, int heading, float a, float b, float c, float d, float e,
                                              float f, char* name);
}

namespace {
struct Point3 { float x, y, z; };
float bits(unsigned value) { return __builtin_bit_cast(float, value); }
void* slot(unsigned char* block, unsigned offset) { return *reinterpret_cast<void**>(block + offset); }
}

extern "C" void setState__11LavaMonsteri(void* self, int requested) {
    unsigned char* object = static_cast<unsigned char*>(self);
    int state = requested;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    void* const animation = object + 288;
    const float tenth = bits(0x3dcccccc);
    const float fifth = bits(0x3e4ccccc);
    unsigned char* block;
    switch (static_cast<unsigned>(state)) {
    case 0:
        block = *reinterpret_cast<unsigned char**>(object + 36);
        if (*reinterpret_cast<short*>(object + 572) == 13) {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block, 84), 0.5f, 3);
            block = *reinterpret_cast<unsigned char**>(object + 36);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, slot(block, 108), 0.0f, tenth, 32);
        } else {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block, 84), tenth, 3);
        }
        *reinterpret_cast<int*>(object + 256) = static_cast<int>(*reinterpret_cast<unsigned*>(object + 808) + eRand__10GameObject(self) % 120u);
        break;
    case 1: {
        resetRouting__8Creature(self);
        Point3 destination;
        getSmartRandomDestination__8Creaturei(&destination, self, 200);
        *reinterpret_cast<Point3*>(object + 272) = destination;
        block = *reinterpret_cast<unsigned char**>(object + 36);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block, 212), tenth, 2051);
        animGlobalSync__FP14AnimationState(animation);
        break;
    }
    case 3:
        resetRouting__8Creature(self);
        block = *reinterpret_cast<unsigned char**>(object + 36);
        if (static_cast<float>(*reinterpret_cast<int*>(object + 964)) < *reinterpret_cast<float*>(object + 560) &&
            *reinterpret_cast<signed char*>(object + 568) != 0) {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block, 84), fifth, 3);
        } else {
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block, 212), fifth, 2051);
        }
        animGlobalSync__FP14AnimationState(animation);
        break;
    case 5:
    case 9:
        block = *reinterpret_cast<unsigned char**>(object + 36);
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block, 84), tenth, 3);
        break;
    case 6: {
        const unsigned pick = eRand__10GameObject(self) % 3u;
        block = *reinterpret_cast<unsigned char**>(object + 36);
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, slot(block, pick * 8 + 156), fifth, tenth, 8);
        break;
    }
    case 8: {
        const int current = *reinterpret_cast<short*>(object + 572);
        if ((current == 6 && (*reinterpret_cast<unsigned short*>(object + 294) & 8u) != 0u) || current == 9 || current == 15) {
            state = current;
        } else {
            block = *reinterpret_cast<unsigned char**>(object + 36);
            animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, slot(block, 60), tenth, tenth, 16);
        }
        break;
    }
    case 10: {
        const Point3 position = *reinterpret_cast<const Point3*>(object + 16);
        dropTreasure__8CreatureG6Point3iffffffPc(self, &position, *reinterpret_cast<short*>(object + 56), bits(0x42100000), 2.0f,
                                                 bits(0x40333333), 0.0f, 3.0f, 0.25f, nullptr);
        const unsigned pick = eRand__10GameObject(self) & 1u;
        block = *reinterpret_cast<unsigned char**>(object + 36);
        *reinterpret_cast<unsigned*>(object + 1052) = pick;
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(animation, slot(block, pick * 8 + 44), tenth, 6);
        unsigned char* entry = *reinterpret_cast<unsigned char**>(object + 72) + 192;
        reinterpret_cast<void (*)(void*)>(*reinterpret_cast<unsigned*>(entry + 4))(object + *reinterpret_cast<short*>(entry));
        break;
    }
    case 14: {
        const unsigned pick = eRand__10GameObject(self) & 1u;
        block = *reinterpret_cast<unsigned char**>(object + 36);
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, slot(block, pick * 8 + 140), fifth, tenth, 2056);
        break;
    }
    case 15:
        block = *reinterpret_cast<unsigned char**>(object + 36);
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(animation, slot(block, 196), fifth, tenth, 2056);
        break;
    default:
        break;
    }
    *reinterpret_cast<short*>(object + 572) = static_cast<short>(state);
    const short stored = static_cast<short>(state);
    if (stored != 13 && stored != 0) *reinterpret_cast<unsigned long long*>(object + 160) &= ~0x800ull;
}
