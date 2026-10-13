// Observed setState__8Innoruuki at 0x001fbef8, 1724 bytes.
// setState(o, s): the requested state s is held in a local word that is passed by address to
// setStateProcessing__8CreaturePi, which may change it; a nonzero result returns at once without
// touching anything else. Otherwise the (possibly changed) value picks a case; 22 values (0..21) are
// handled, and every other value, plus 2, 4, 7, 9, 11, 12 and 13, only runs the final step.
// R is the resource block whose pointer is the word at o+36; the "animation state" is o+288;
// "transition(h, r, f)" is animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o+288, the
// word at R+h, r, f) and "oneshot(h, r, f)" is animAddOneShot__FP14AnimationStateP15AnimationHeaderffi
// (o+288, the word at R+h, r, r, f). K is the float 0x3dcccccc (0.1) unless noted; N is 0x3e999999
// (0.3). "rand" is eRand__10GameObject(o), an unsigned value reduced with unsigned division.
//   0: o+256 = word at o+808 + rand % 120; transition(116, K, 3) when the word at o+960 is set,
//      otherwise transition(124, K, 3).
//   1: resetRouting__8Creature(o); getSmartRandomDestination__8Creaturei(point, o, 200) and the 12
//      bytes it wrote go to o+272; o+256 = word at o+808 + 300 + rand % 120; transition(164 if the word
//      at o+960 is set else 156, N, 2051); animGlobalSync__FP14AnimationState(o+288).
//   3: transition(164 if the word at o+960 is set else 156, K, 2051).
//   5: transition(116 if the word at o+960 is set else 124, K, 3).
//   6: transition(116, K, 3); oneshot(84, K, 2056).      21: the same with oneshot(92, K, 2056).
//  17: transition(116, K, 3); oneshot(76, K, 16392).     18: the same with oneshot(140, K, 2056).
//  14: transition(52 + 8 * (rand % 3), K, 3); o+256 = word at o+808 + 300.
//  15: transition(124, K, 3); oneshot(132, K, 8).        16: transition(116, K, 3); oneshot(28, K, 8).
//  19: transition(116, K, 3); oneshot(100, K, 8).
//  20: soundPlaySequence__FP15AnimationHeaderP10GameObjectf(word at R+220, o, 1.0);
//      transition(124, K, 3); oneshot(100, K, 8).
//   8: words at o+972, o+976 and o+968 are cleared. A draw rand % 100 of 31 or more, or a zero word at
//      o+960, keeps the current state (s becomes the halfword at o+572). Otherwise, when that halfword
//      is 19 and a second draw rand % 100 is below 50: oneshot(44, K, 16) and the word at o+964 = 1.
//      Otherwise when the halfword is 20 the state is kept; otherwise, when the word at o+960 is set
//      and a third draw rand % 100 is below 20: oneshot(44, K, 16), o+964 = 1 and the word at o+996 =
//      word at o+808 + 3600; in all other cases oneshot(108, K, 16).
//  10: lumpFindResource__FPcT0("innoruuk.lmp", "hitreact1.anm") goes to soundPlaySequence(r, o, 1.0);
//      coreStartSwitchScene__Fi(10); Camera Push and SetMode(2) on gameCam; bit 28 of the 64-bit word
//      at o+160 is set; the word at o+1000 gets the word at o+808; the words at o+972, +976 and +968
//      are cleared; dropTreasure__8CreatureG6Point3iffffffPc(o, a copy of o+16, the signed halfword
//      at o+56, floats 36, 1, 10, 1, 6, 1, null text) runs; transition(36, N, 7) and finally the
//      table call at the halfword/function pair at table+192 with o plus that halfword.
// Final step (all paths): when the signed halfword at o+572 is 15 and s is not 15,
// soundStopSound__Fi(word at o+1216) runs and that word becomes -1. The halfword at o+572 then
// receives the low 16 bits of s.
// Inferred: states and fields are named only by offset. Unknown: what the states mean, the words at
// +256, +960, +964, +968, +972, +976, +996 and +1000, and the table slot at +192.
// Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void soundStopSound__Fi(int id);
void* lumpFindResource__FPcT0(char* lump, char* name);
void coreStartSwitchScene__Fi(int scene);
void Push__6Camera(void* camera);
void SetMode__6Camerai(void* camera, int mode);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const void* position, int a, float f0, float f1, float f2, float f3,
                                               float f4, float f5, char* text);
extern void* gameCam;
}

namespace {
const float K = 0x1.999998p-4f;
const float N = 0x1.333332p-2f;
typedef unsigned long long __attribute__((may_alias)) AliasedDouble;

struct Setup {
    unsigned char* o;
    unsigned& word(unsigned off) { return w10h::word_at(o, off); }
    unsigned char* block() { return w10h::from_word(w10h::word_at(o, 36)); }
    void* header(unsigned off) { return w10h::from_word(*reinterpret_cast<unsigned*>(block() + off)); }
    void transition(unsigned off, float rate, int flags) {
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, header(off), rate, flags);
    }
    void oneshot(unsigned off, float rate, int flags) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, header(off), rate, rate, flags);
    }
};
}  // namespace

extern "C" void setState__8Innoruuki(void* self, int requested) {
    int state = requested;
    Setup s = {static_cast<unsigned char*>(self)};
    unsigned char* o = s.o;
    if (setStateProcessing__8CreaturePi(self, &state)) return;
    switch (static_cast<unsigned>(state) < 22u ? state : -1) {
        case 0: {
            s.word(256) = s.word(808) + eRand__10GameObject(self) % 120u;
            if (s.word(960) != 0) s.transition(116, K, 3); else s.transition(124, K, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            unsigned char point[12];
            getSmartRandomDestination__8Creaturei(point, self, 200);
            w10h::copy_bytes(o + 272, point, 12);
            s.word(256) = s.word(808) + 300u + eRand__10GameObject(self) % 120u;
            if (s.word(960) != 0) s.transition(164, N, 2051); else s.transition(156, N, 2051);
            animGlobalSync__FP14AnimationState(o + 288);
            break;
        }
        case 3:
            if (s.word(960) != 0) s.transition(164, K, 2051); else s.transition(156, K, 2051);
            break;
        case 5:
            if (s.word(960) != 0) s.transition(116, K, 3); else s.transition(124, K, 3);
            break;
        case 6: s.transition(116, K, 3); s.oneshot(84, K, 2056); break;
        case 21: s.transition(116, K, 3); s.oneshot(92, K, 2056); break;
        case 17: s.transition(116, K, 3); s.oneshot(76, K, 16392); break;
        case 18: s.transition(116, K, 3); s.oneshot(140, K, 2056); break;
        case 14: {
            unsigned pick = eRand__10GameObject(self) % 3u;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, s.header(52 + 8 * pick), K, 3);
            s.word(256) = s.word(808) + 300u;
            break;
        }
        case 15: s.transition(124, K, 3); s.oneshot(132, K, 8); break;
        case 16: s.transition(116, K, 3); s.oneshot(28, K, 8); break;
        case 19: s.transition(116, K, 3); s.oneshot(100, K, 8); break;
        case 20:
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(s.header(220), self, 1.0f);
            s.transition(124, K, 3);
            s.oneshot(100, K, 8);
            break;
        case 8: {
            s.word(972) = 0;
            s.word(976) = 0;
            s.word(968) = 0;
            if (eRand__10GameObject(self) % 100u >= 31u || s.word(960) == 0) {
                state = *reinterpret_cast<short*>(o + 572);
                break;
            }
            if (*reinterpret_cast<short*>(o + 572) == 19) {
                if (eRand__10GameObject(self) % 100u < 50u) {
                    s.oneshot(44, K, 16);
                    s.word(964) = 1;
                    break;
                }
            }
            const short current = *reinterpret_cast<short*>(o + 572);
            if (current == 20) {
                state = current;
                break;
            }
            if (s.word(960) != 0) {
                if (eRand__10GameObject(self) % 100u < 20u) {
                    s.oneshot(44, K, 16);
                    s.word(964) = 1;
                    s.word(996) = s.word(808) + 3600u;
                    break;
                }
            }
            s.oneshot(108, K, 16);
            break;
        }
        case 10: {
            void* resource = lumpFindResource__FPcT0(const_cast<char*>("innoruuk.lmp"), const_cast<char*>("hitreact1.anm"));
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(resource, self, 1.0f);
            coreStartSwitchScene__Fi(10);
            Push__6Camera(gameCam);
            SetMode__6Camerai(gameCam, 2);
            *reinterpret_cast<AliasedDouble*>(o + 160) = *reinterpret_cast<AliasedDouble*>(o + 160) | 0x10000000ull;
            s.word(1000) = s.word(808);
            s.word(972) = 0;
            s.word(976) = 0;
            s.word(968) = 0;
            unsigned char point[12];
            w10h::copy_bytes(point, o + 16, 12);
            dropTreasure__8CreatureG6Point3iffffffPc(self, point, *reinterpret_cast<short*>(o + 56), 36.0f, 1.0f, 10.0f, 1.0f, 6.0f, 1.0f, nullptr);
            s.transition(36, N, 7);
            typedef void (*Slot)(void*);
            w10h::fn_from_word<Slot>(w10h::entry_function(self, 192))(w10h::adjusted(self, 192));
            break;
        }
        default: break;
    }
    if (*reinterpret_cast<short*>(o + 572) == 15 && state != 15) {
        soundStopSound__Fi(static_cast<int>(s.word(1216)));
        s.word(1216) = static_cast<unsigned>(-1);
    }
    *reinterpret_cast<unsigned short*>(o + 572) = static_cast<unsigned short>(state);
}
