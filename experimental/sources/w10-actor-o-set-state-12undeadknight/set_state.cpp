// Observed setState__12UndeadKnighti at 0x0023d128, 1648 bytes. R is the block at the word object+36,
// "anim" the animation state at +288, h the signed halfword at +572, b the unsigned halfword at
// +294, wait the word at +256, base the word at +808, h56 the signed halfword at +56. A copy n of
// the argument goes to setStateProcessing__8CreaturePi (which may change it); a non-zero result ends
// the call with nothing else done. Otherwise bit 9 of the doubleword at +160 is cleared (also for
// out-of-range n), and n selects a case (the jump table has 21 words). "transition" is
// animAddTransitionTo(anim, header, 0.1, flags) and "oneShot" is animAddOneShot(anim, header, 0.1,
// 0.1, 0x808); every rate is the .lit4 word 0x3dcccccc unless noted. "setState(3)" is the table entry
// at (word +72) + 184 called with 3. At the end the low halfword of n is stored at +572.
//  0: wait = base + eRand % 120; transition(R[84], 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) goes to +272..283;
//     transition(R[108], 0x803); animGlobalSync(anim).
//  3: resetRouting; transition(R[108], 0x803); animGlobalSync(anim).
//  5: transition(R[60], 3).
//  8: unless (h is 6 and bit 3 of b is set), h is 9, or the word at +960 is non-zero: oneShot(R[76], flags
//     16). In those cases n becomes h.
//  9: with the word at +960 non-zero n becomes h; otherwise transition(R[84], rate 0x3e999999, 3).
//  10: dropTreasure(this, point +16, h56, 36, 2, 1.8, 0.6, 2, 0.25, null); transition(R[124], 6); when
//      the word at +964 is non-zero: gameAttackResolver(the word at (that word) + 12, this, 5000.0,
//      5000.0, 10000, 0, 0, 64, null, false). The table entry at (word +72) + 192 is called.
//  13: oneShot(R[116], 0x808 with rate 0.1 and no transition).
//  14: transition(R[60], 3); oneShot(R[28]); wait = eRand % 20.
//  15: transition(R[60], 3); oneShot(R[36]); wait = eRand % 20.
//  16: transition(R[60], 3); oneShot(R[44]); wait = eRand % 20.
//  17: transition(R[60], 3); oneShot(R[52]); wait = eRand % 20.
//  18: bit 9 of the doubleword at +160 is set; transition(R[60], 3).
//  19: with e the word at +576 non-zero and t the word at e + 12: the point P is the point at t +
//      16 plus {36 cos(h56 - 32767), 36 sin(h56 - 32767), 0}; otherwise P is the point at +16 plus
//      {120 cos(h56), 120 sin(h56), 0}. A 2704-byte block is allocated and, when non-null,
//      UndeadKnightClone is constructed in it with (P, h56, null); bit 1 of its word at +12 is set and
//      it is linked into the list at +964 through listOnewayLink(list, block, block). Then
//      transition(R[84], 3); oneShot(R[68]).
//  20: transition(R[60], 3); oneShot(R[100]).
//  2, 4, 6, 7, 11, 12 and n >= 21 (unsigned): setState(3) is called and n is kept.
// The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
unsigned eRand__10GameObject(void* object);
float icos__Fi(int angle);
float isin__Fi(int angle);
void* blockAlloc__Fi(int size);
void __17UndeadKnightCloneG6Point3sPPc(void* self, Point3 position, short angle, char** arguments);
void* listOnewayLink__FP8ListHeadT0Pv(void* head, void* second, void* value);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);
void gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
    void* target, void* source, float a, float b, int c, int d, int e, int f, void* description, bool flag);

void setState__12UndeadKnighti(void* self, int state) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    dword(o, 160) = dword(o, 160) & ~0x200ull;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    const unsigned fast = 0x3dccccccu;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    auto transition = [&](unsigned offset, unsigned rate, int flag) {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, header(offset), bits(rate), flag);
    };
    auto one_shot = [&](unsigned offset, unsigned rate, int flag) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), bits(rate), bits(rate), flag);
    };
    auto roll_wait = [&]() {
        const unsigned r = eRand__10GameObject(self);
        word(o, 256) = r % 20u;
    };
    switch (static_cast<unsigned>(n)) {
        case 0: {
            const unsigned r = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + r % 120u;
            transition(84, fast, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            Point3 point;
            getSmartRandomDestination__8Creaturei(&point, self, 200);
            real(o, 272) = point.x; real(o, 276) = point.y; real(o, 280) = point.z;
            transition(108, fast, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 3:
            resetRouting__8Creature(self);
            transition(108, fast, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        case 5:
            transition(60, fast, 3);
            break;
        case 14:
            transition(60, fast, 3);
            one_shot(28, fast, 0x808);
            roll_wait();
            break;
        case 15:
            transition(60, fast, 3);
            one_shot(36, fast, 0x808);
            roll_wait();
            break;
        case 17:
            transition(60, fast, 3);
            one_shot(52, fast, 0x808);
            roll_wait();
            break;
        case 16:
            transition(60, fast, 3);
            one_shot(44, fast, 0x808);
            roll_wait();
            break;
        case 13:
            one_shot(116, fast, 0x808);
            break;
        case 18:
            dword(o, 160) = dword(o, 160) | 0x200ull;
            transition(60, fast, 3);
            break;
        case 19: {
            Point3 target;
            if (word(o, 576) != 0) {
                unsigned char* const t = from_word(word(from_word(word(o, 576)), 12));
                target = *reinterpret_cast<Point3*>(t + 16);
                const float x = icos__Fi(half(o, 56) - 32767) * 36.0f;
                const float y = isin__Fi(half(o, 56) - 32767) * 36.0f;
                target.x = target.x + x;
                target.y = target.y + y;
                target.z = target.z + 0.0f;
            } else {
                const float x = icos__Fi(half(o, 56)) * 120.0f;
                const float y = isin__Fi(half(o, 56)) * 120.0f;
                target.x = x + real(o, 16);
                target.y = y + real(o, 20);
                target.z = 0.0f + real(o, 24);
            }
            const short angle = half(o, 56);
            void* const clone = blockAlloc__Fi(2704);
            if (clone != nullptr) {
                __17UndeadKnightCloneG6Point3sPPc(clone, target, angle, nullptr);
                if (clone != nullptr) {
                    word(static_cast<unsigned char*>(clone), 12) = word(static_cast<unsigned char*>(clone), 12) | 2u;
                    listOnewayLink__FP8ListHeadT0Pv(o + 964, clone, clone);
                }
            }
            transition(84, fast, 3);
            one_shot(68, fast, 0x808);
            break;
        }
        case 20:
            transition(60, fast, 3);
            one_shot(100, fast, 0x808);
            break;
        case 8: {
            const int h = half(o, 572);
            const bool flag = (*reinterpret_cast<unsigned short*>(o + 294) & 8u) != 0;
            if ((h == 6 && flag) || h == 9 || word(o, 960) != 0) {
                n = h;
            } else {
                one_shot(76, fast, 16);
            }
            break;
        }
        case 9:
            if (word(o, 960) != 0) n = half(o, 572);
            else transition(84, 0x3e999999u, 3);
            break;
        case 10: {
            const Point3 position = *reinterpret_cast<Point3*>(o + 16);
            dropTreasure__8CreatureG6Point3iffffffPc(self, position, half(o, 56), 36.0f, 2.0f, bits(0x3fe66666u), bits(0x3f199999u), 2.0f, 0.25f, nullptr);
            transition(124, fast, 6);
            if (word(o, 964) != 0) {
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(
                    from_word(word(from_word(word(o, 964)), 12)), self, 5000.0f, 5000.0f, 10000, 0, 0, 64, nullptr, false);
            }
            call_entry(o, 192);
            break;
        }
        default:
            call_entry(o, 184, 3);
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
