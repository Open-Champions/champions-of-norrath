// Observed setState__11VampireLordi at 0x00242d90, 2032 bytes. Offsets are used for fields. A is the
// animation state at +288, T the block that the word at +36 points to (it holds animation headers
// at the offsets used below), "now" is the word at +808, and the words +960, +964, +980, +988,
// +992, +256, +968 and +972 are flags and timers whose meaning is not established.
//  - setStateProcessing__8CreaturePi(this, &state) runs first (the state is passed by address and may
//    change); a nonzero result ends the function at once, writing nothing else.
//  - Bit 9 (0x200) of the 64-bit flag word at +160 is cleared, then a jump table on the state (0..22;
//    larger values and states 2, 4, 11, 12 and 22 do nothing) does the following. "Transition" is
//    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(A, header, rate, flags) and
//    "one-shot" is animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(A, header, rate, rate, flags).
//     0: +256 = now + 90 + draw % 90; transition T[52] (rate 0.1, flags 3).
//     1: resetRouting, a random destination (range 300) goes to the point at +272; transition T[156]
//        (0.1, 2051).
//     3: nothing when the halfword +572 is already 3; otherwise transition T[132] when +960 is set else
//        T[156] (0.1, 2051), then animGlobalSync(A).
//     5: transition T[60] (+960 set) or T[28] (0.1, 3).
//     6: transition T[28] (0.1, 3); +256 = 20.
//     7: sound T[196] (1.0), +968 = 0, transition T[148] (0.1, 6), +256 = now + 240.
//     8 and 9 (9 first sets the state to 8): when +572 is 6 and bit 3 of the halfword at +294 is set the
//        state becomes 6; else when +960 is set and flag bit 9 is set, one-shot T[84] (0.1, flags
//        2064); else the state becomes the current halfword +572.
//    10: sound T[196]; dropTreasure__8CreatureG6Point3iffffffPc(this, point at +16, halfword +56, 36, 2,
//        1.6, 0.6, 1.4, 0.25, "vampirefang"); +968 = 0; transition T[148] (0.1, 6); +256 = now + 180 +
//        draw % 180, and table byte offset 192 is called with the same value.
//    13: +992 = 0; transition T[52] (0.1, 3); one-shot T[68] (0.1, 2056); +972 = 0.
//    14..17: attack chain steps. With +980 zero: transition T[60] (0.1, 2051) and a one-shot
//        (T[108], T[116], T[124], T[100] for 14, 15, 16, 17; rate 0.2; flags 2056). Otherwise, when +980
//        is 1, +988 = now + 240 + 60 * (draw % 3) (state 17: draw & 3), then the same one-shot. In every case
//        +980 is decremented when nonzero. State 17 first clears +992.
//    18: transition T[36] (0.1, 6); +256 = now + 180 + draw % 120.
//    19: transition T[52] (0.5, 4); one-shot T[76] (0.1, 32); +972 = 0; +256 = now + 300 + draw % 120.
//    20: flag bit 9 is set; with +960 set, sound T[188] (when +980 >= 3) then transition T[92] (0.3, 3);
//        else transition T[28] (0.3, 3).
//    21: sound T[196]; +968 = 0; transition T[148] (0.4, 6); +256 = (int)((double)now + 90.0) through the
//        software double routines litodp, dpadd and dptoli.
//  - Finally, when +964 is nonzero and the state is neither 7 nor 10, convertToLord__11VampireLord(this)
//    runs; the low half of the state goes to the halfword +572.
// Draws use eRand__10GameObject (unsigned remainder). Unknown: what the states, flags and timers mean
// (the state number names are not established). Nothing here ran on the EE.
#include "../w10-actor-p-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* object);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);
void convertToLord__11VampireLord(void* self);
unsigned long long litodp(int value);
unsigned long long dpadd(unsigned long long left, unsigned long long right);
int dptoli(unsigned long long value);
}

namespace {
using namespace w10p;
unsigned char* header(unsigned char* o, unsigned offset) { return from_word(word(from_word(word(o, 36)), offset)); }
void transition(unsigned char* o, unsigned offset, unsigned rate, int flags) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, header(o, offset), bits(rate), flags);
}
void oneShot(unsigned char* o, unsigned offset, unsigned rate, int flags) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, header(o, offset), bits(rate), bits(rate), flags);
}
void sound(unsigned char* o, unsigned offset) { soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header(o, offset), o, 1.0f); }
const unsigned kTenth = 0x3dcccccc;
const unsigned kFifth = 0x3e4ccccc;
const unsigned kThreeTenths = 0x3e999999;

// Attack chain steps 14..17: `selector` picks the one-shot header and the draw modulus form.
void chain(unsigned char* o, unsigned oneShotOffset) {
    if (word(o, 980) == 0u) {
        transition(o, 60, kTenth, 2051);
        oneShot(o, oneShotOffset, kFifth, 2056);
    } else {
        if (sword(o, 980) == 1) sword(o, 988) = sword(o, 808) + static_cast<int>(eRand__10GameObject(o) % 3u) * 60 + 240;
        oneShot(o, oneShotOffset, kFifth, 2056);
    }
    if (word(o, 980) != 0u) word(o, 980) = word(o, 980) - 1u;
}
}  // namespace

extern "C" void setState__11VampireLordi(void* self, int newState) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    int state = newState;
    if (setStateProcessing__8CreaturePi(self, &state) != 0) return;
    dword(o, 160) = dword(o, 160) & ~0x200ull;
    switch (static_cast<unsigned>(state)) {
        case 0:
            sword(o, 256) = sword(o, 808) + static_cast<int>(eRand__10GameObject(o) % 90u) + 90;
            transition(o, 52, kTenth, 3);
            break;
        case 1: {
            resetRouting__8Creature(self);
            Point3 destination;
            getSmartRandomDestination__8Creaturei(&destination, self, 300);
            *reinterpret_cast<Point3*>(o + 272) = destination;
            transition(o, 156, kTenth, 2051);
            break;
        }
        case 21:
            sound(o, 196);
            word(o, 968) = 0u;
            transition(o, 148, 0x3ecccccc, 6);
            sword(o, 256) = dptoli(dpadd(litodp(sword(o, 808)), 0x4056800000000000ull));
            break;
        case 7:
            sound(o, 196);
            word(o, 968) = 0u;
            transition(o, 148, kTenth, 6);
            sword(o, 256) = sword(o, 808) + 240;
            break;
        case 3:
            if (half(o, 572) != 3) {
                if (word(o, 960) != 0u) transition(o, 132, kTenth, 2051);
                else transition(o, 156, kTenth, 2051);
                animGlobalSync__FP14AnimationState(o + 288);
            }
            break;
        case 5:
            if (word(o, 960) != 0u) transition(o, 60, kTenth, 3);
            else transition(o, 28, kTenth, 3);
            break;
        case 18:
            transition(o, 36, kTenth, 6);
            sword(o, 256) = sword(o, 808) + static_cast<int>(eRand__10GameObject(o) % 120u) + 180;
            break;
        case 19:
            transition(o, 52, 0x3f000000, 4);
            oneShot(o, 76, kTenth, 32);
            word(o, 972) = 0u;
            sword(o, 256) = sword(o, 808) + static_cast<int>(eRand__10GameObject(o) % 120u) + 300;
            break;
        case 6:
            transition(o, 28, kTenth, 3);
            sword(o, 256) = 20;
            break;
        case 20:
            dword(o, 160) = dword(o, 160) | 0x200ull;
            if (word(o, 960) != 0u) {
                if (!(sword(o, 980) < 3)) sound(o, 188);
                transition(o, 92, kThreeTenths, 3);
            } else {
                transition(o, 28, kThreeTenths, 3);
            }
            break;
        case 13:
            word(o, 992) = 0u;
            transition(o, 52, kTenth, 3);
            oneShot(o, 68, kTenth, 2056);
            word(o, 972) = 0u;
            break;
        case 14:
            chain(o, 108);
            break;
        case 15:
            chain(o, 116);
            break;
        case 16:
            chain(o, 124);
            break;
        case 17:
            word(o, 992) = 0u;
            if (word(o, 980) == 0u) {
                transition(o, 60, kTenth, 2051);
                oneShot(o, 100, kFifth, 2056);
            } else {
                if (sword(o, 980) == 1) sword(o, 988) = sword(o, 808) + static_cast<int>(eRand__10GameObject(o) & 3u) * 60 + 240;
                oneShot(o, 100, kFifth, 2056);
            }
            if (word(o, 980) != 0u) word(o, 980) = word(o, 980) - 1u;
            break;
        case 9:
            state = 8;
            /* fall through */
        case 8: {
            const int current = half(o, 572);
            if (current == 6 && (*reinterpret_cast<unsigned short*>(o + 294) & 8u) != 0u) {
                state = current;
            } else if (word(o, 960) != 0u && (dword(o, 160) & 0x200ull) != 0ull) {
                oneShot(o, 84, kTenth, 2064);
            } else {
                state = current;
            }
            break;
        }
        case 10: {
            sound(o, 196);
            dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), half(o, 56), bits(0x42100000u), bits(0x40000000u),
                                                       bits(0x3fcccccc), bits(0x3f199999u), bits(0x3fb33333u), bits(0x3e800000u),
                                                       const_cast<char*>("vampirefang"));
            word(o, 968) = 0u;
            transition(o, 148, kTenth, 6);
            const int value = sword(o, 808) + static_cast<int>(eRand__10GameObject(o) % 180u) + 180;
            sword(o, 256) = value;
            w10p::call_entry(o, 192, static_cast<unsigned long>(value));
            break;
        }
        default:
            break;
    }
    if (word(o, 964) != 0u && state != 7 && state != 10) convertToLord__11VampireLord(self);
    half(o, 572) = static_cast<short>(state);
}
