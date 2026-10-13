// Observed setState__7Vampirei at 0x00240210, 1196 bytes. R is the block at the word object+36,
// "anim" the animation state at +288, h the signed halfword at +572, and b the unsigned halfword at
// +294. A copy n of the argument goes to setStateProcessing__8CreaturePi (which may change it); a
// non-zero result ends the call with nothing else done. Otherwise n selects a case (the jump table
// has 17 words; n >= 17 as an unsigned value and 2, 4, 7, 11, 12 do nothing more) and the low
// halfword of n is stored at +572 at the end. "transition" is animAddTransitionTo(anim, header,
// rate, flags); "oneShot" is animAddOneShot(anim, header, a, b, flags). The rates are the .lit4
// words 0x3e999999 (called 0.3 below) and 0x3dcccccc (0.1). wait is the word at +256, base the
// word at +808.
//  0: wait = base + eRand % 90 + 90; transition(R[132], 0.3, 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 300) goes to +272..283;
//     transition(R[52], 0.3, 0x803); animGlobalSync(anim).
//  3: when h is not 3, transition(R[52], 0.1, 0x803); then animGlobalSync(anim).
//  5: transition(R[132], 0.1, 3).
//  6: transition(R[132], 0.1, 3); oneShot(R[44], 0.1, 0.1, 0x808); wait = 20.
//  8, 9: 9 first sets n to 8. With h = 6, n becomes 6 when bit 3 of b is set; with h = 16 and
//     bit 3 of b set, n becomes 16 and nothing else happens. Otherwise oneShot(R[(eRand & 1) * 8
//     + 108], 0.1, 0.1, 0x810).
//  10: dropTreasure(this, point +16, h56, 36, 2, 1.6, 0.6, 1.4, 0.25, "vampirefang"); r = eRand % 3;
//     transition(R[r * 8 + 60], 0.3, 0x806); wait = base; bit 0x4000 of the word at +12 is set; the
//     table entry at (word +72) + 192 is called.
//  13: resetRouting; transition(R[52], 0.3, 0x803); animGlobalSync(anim).
//  14: transition(R[148], 0.1, 3); wait = base + eRand % 240 + 240.
//  15: transition(R[132], 0.1, 3); d = findClearPath(this, point +16, h56, (char)(eRand & 1), 3.0)
//     - h56, as a short; the index i is 10 for d in 8192..24575, 11 for d in -24575..-8193, else 12;
//     oneShot(R[i * 8 + 4], 0.01, 0.1, 0x808).
//  16: transition(R[124], 0.1, 3); oneShot(R[(eRand & 1) * 8 + 28], 0.1, 0.1, 0x808); wait = 20.
// Here h56 is the halfword at +56. The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
unsigned eRand__10GameObject(void* object);
short findClearPath__8CreatureG6Point3scf(void* self, Point3 position, short heading, char flag, float range);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);

void setState__7Vampirei(void* self, int state) {
    using namespace w10o;
    unsigned char* const o = static_cast<unsigned char*>(self);
    int n = state;
    if (setStateProcessing__8CreaturePi(self, &n) != 0) return;
    unsigned char* const block = from_word(word(o, 36));
    void* const anim = o + 288;
    auto header = [&](unsigned offset) { return from_word(word(block, offset)); };
    auto transition = [&](unsigned offset, unsigned rate, int flag) {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(anim, header(offset), bits(rate), flag);
    };
    auto one_shot = [&](unsigned offset, unsigned a, unsigned b, int flag) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), bits(a), bits(b), flag);
    };
    const unsigned slow = 0x3e999999u;
    const unsigned fast = 0x3dccccccu;
    switch (static_cast<unsigned>(n)) {
        case 0: {
            const unsigned r = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + r % 90u + 90u;
            transition(132, slow, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            Point3 point;
            getSmartRandomDestination__8Creaturei(&point, self, 300);
            real(o, 272) = point.x; real(o, 276) = point.y; real(o, 280) = point.z;
            transition(52, slow, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 13:
            resetRouting__8Creature(self);
            transition(52, slow, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        case 3:
            if (half(o, 572) != 3) transition(52, fast, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        case 5:
            transition(132, fast, 3);
            break;
        case 6:
            transition(132, fast, 3);
            one_shot(44, fast, fast, 0x808);
            word(o, 256) = 20;
            break;
        case 16: {
            transition(124, fast, 3);
            const unsigned r = eRand__10GameObject(self);
            one_shot((r & 1u) * 8u + 28u, fast, fast, 0x808);
            word(o, 256) = 20;
            break;
        }
        case 9:
        case 8: {
            if (n == 9) n = 8;
            const int h = half(o, 572);
            const bool flag = (*reinterpret_cast<unsigned short*>(o + 294) & 8u) != 0;
            if (h == 6 && flag) n = h;
            if (h == 16 && flag) {
                n = h;
            } else {
                const unsigned r = eRand__10GameObject(self);
                one_shot((r & 1u) * 8u + 108u, fast, fast, 0x810);
            }
            break;
        }
        case 10: {
            const Point3 position = *reinterpret_cast<Point3*>(o + 16);
            dropTreasure__8CreatureG6Point3iffffffPc(self, position, half(o, 56), 36.0f, 2.0f, bits(0x3fcccccc), bits(0x3f199999), bits(0x3fb33333), 0.25f,
                                                       const_cast<char*>("vampirefang"));
            const unsigned r = eRand__10GameObject(self) % 3u;
            transition(r * 8u + 60u, slow, 0x806);
            word(o, 256) = word(o, 808);
            word(o, 12) = word(o, 12) | 0x4000u;
            call_entry(o, 192);
            break;
        }
        case 14: {
            transition(148, fast, 3);
            const unsigned r = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + r % 240u + 240u;
            break;
        }
        case 15: {
            transition(132, fast, 3);
            const unsigned r = eRand__10GameObject(self);
            const Point3 position = *reinterpret_cast<Point3*>(o + 16);
            const short path = findClearPath__8CreatureG6Point3scf(self, position, half(o, 56), static_cast<char>(r & 1u), 3.0f);
            const int d = static_cast<short>(path - static_cast<unsigned short>(half(o, 56)));
            unsigned index;
            if (d < -8192) index = d < -24575 ? 12u : 11u;
            else index = static_cast<unsigned>(d - 8192) < 16384u ? 10u : 12u;
            one_shot(index * 8u + 4u, 0x3c23d70au, fast, 0x808);
            break;
        }
        default:
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
