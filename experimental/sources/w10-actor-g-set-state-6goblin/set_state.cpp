// Observed setState__6Goblini at 0x0022de28, 2412 bytes. The new state S is held in a stack word
// whose address goes to the Creature state processing call (setStateProcessing__8CreaturePi), which
// may change it; a nonzero result returns at once. Otherwise bit 9 (0x200) of the 64-bit field at
// +160 is cleared, and states 0..16 (unsigned) select a case through the table at 0x471fa0. t is the
// table at word +36 (animation header pointers), a the member at +288, P the word at +972, Q the word
// at +992, N = "single just under 0.3" (bits 0x3e999999) and M = "single just under 0.1" (bits
// 0x3dcccccc); every transition below is animAddTransitionTo(a, header, blend, flags).
//   0: word +256 = word(+808) + eRand % 60 + 120; transition t[60] (N, 3) when P is 4, else t[108]
//      (N, 3).
//   1: reset routing; random destination (range 300) copied to +272..283; float +176 = 60.0;
//      transition t[140] (N, 3); global sync of a.
//   2: reset routing; float +176 = 150.0; transition t[116] (N, 3); global sync.
//   3: reset routing; float +176 = 60.0; transition t[140] (N, 3); global sync.
//   4: float +176 = 150.0; transition t[116] (N, 3); when the halfword +572 is 3, word +256 =
//      word(+808) + eRand % 120 + 40; global sync.
//   5: transition t[124] (M, 3) when P is 5, else t[100] (M, 3).
//   6: P == 4: transition t[60] (M, 3) then one-shot t[52] (M, M, 8). P == 5: transition t[124] then
//      one-shot t[132]. Otherwise: byte +980 = 1 when Q is nonzero; eRand & 3 == 0 clears it; a
//      nonzero signed byte +980 gives transition t[148] (M, 3), else t[100]; then P == 0 gives one-shot
//      t[28], P in 1..3 gives one-shot t[28 + 8 * (eRand & 1)]. In every case word +256 = eRand % 60 +
//      20 when Q is nonzero, else eRand % 20.
//   8: halfword +572 (H) 9, or 6 with bit 3 (0x8) of halfword +294: S = H. Else when word +744 has
//      bits 0x3c0000: D = (short)(halfword +748 - halfword +56); when D < -26767 or D >= 26768,
//      one-shot t[164] (M, M, 2064). Else when bit 9 of +160 is set (cleared above, so not reached
//      in practice) one-shot t[156] (M, M, 16); else one-shot t[76 + 8 * (eRand % 3)] (M, M, 16).
//   9: transition t[60] (N, 3) when P is 4, else t[108] (N, 3).
//  10: findTagInt(object, "dead", 0): when zero, objects in the box +-96 around (+16) are listed with
//      objectFindInBox (capacity 128); the number of listed objects whose word +12 & 0x68 == 0x28 is
//      counted and word +1136 = 1 when it is 3 or more. When bit 1 (0x2) of word +12 is clear: bit 0
//      is cleared and dropItem(object, 1) and dropItem(object, 2) run. findTagInt "dead" nonzero sets
//      bit 0 of word +12. Transition t[68] (N, 6). When bit 32 of the field at +160 is clear:
//      dropTreasure with the 12 bytes at +16, the halfword +56, null name and floats (36, 2, 1.8, 0.6,
//      2, 0.25); then P == 4 and eRand < 6000 (unsigned) gives item dropItem(position, "arrows").
//      Finally the table entry at (word +72) + 192 is called with the adjusted object.
//  13: transition t[124] (M, 3) when P is 5, else t[100] (M, 3); word +256 = word(+808) + eRand %
//      240 + 240.
//  14: reset routing; transition t[116] (N, 3); float +176 = 150.0; word +256 = word(+808) + 60;
//      global sync.
//  15: transition t[148] (M, 3); word +256 = word(+808) + 120; bit 9 (0x200) of +160 set.
//  16: bits 3 and 0 of word +12 cleared (&= ~9); setWeapons(object); initType(object, 0);
//      objectAddToRunList(object); bit 32 of +160 set; halfwords +146 = 50, +150 = 128, +152 = 255,
//      +154 = 150, +140 = 0, +142 = 128, +144 = 255, +148 = 0; S becomes 0; then as state 0.
//   7, 11, 12 and 17 or more: nothing.
// Finally the halfword at +572 is set to the low 16 bits of S.
// Inferred: the box scan counts nearby objects of one kind. Unknown: what the states, the words
// +972/+992/+744 and most bits mean. dropItem__6Goblini, setWeapons__6Goblin, initType__6Goblinb and
// the callees below are unresolved imports. Nothing here ran on the EE.
#include "../w10-actor-g-common/words.hpp"

struct Point3 { float x, y, z; };

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(Point3* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float blend, int flags);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags);
void animGlobalSync__FP14AnimationState(void* state);
void setWeapons__6Goblin(void* self);
void initType__6Goblinb(void* self, bool flag);
void objectAddToRunList__FP10GameObject(void* object);
int findTagInt__10GameObjectPci(void* self, char* tag, int occurrence);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
void dropItem__6Goblini(void* self, int which);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float f0, float f1, float f2, float f3, float f4, float f5, char* name);
void dropItem__4itemG6Point3Pc(Point3 position, char* name);
}

namespace {
typedef unsigned long long __attribute__((may_alias)) AliasedDouble;
const float N = 0x1.333332p-2f;
const float M = 0x1.999998p-4f;

void* header(void* object, unsigned offset) {
    unsigned char* table = w10g::from_word(w10g::word_at(object, 36));
    return w10g::from_word(*reinterpret_cast<unsigned*>(table + offset));
}
unsigned& word(unsigned char* o, unsigned offset) { return *reinterpret_cast<unsigned*>(o + offset); }
int& sword(unsigned char* o, unsigned offset) { return *reinterpret_cast<int*>(o + offset); }
float& flt(unsigned char* o, unsigned offset) { return *reinterpret_cast<float*>(o + offset); }
AliasedDouble& wide(unsigned char* o, unsigned offset) { return *reinterpret_cast<AliasedDouble*>(o + offset); }
}  // namespace

extern "C" void setState__6Goblini(void* self, int state) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* a = o + 288;
    if (setStateProcessing__8CreaturePi(self, &state)) return;
    wide(o, 160) = wide(o, 160) & ~0x200ull;
    if (static_cast<unsigned>(state) < 17u) {
        switch (state) {
        case 16:
            word(o, 12) = word(o, 12) & ~9u;
            setWeapons__6Goblin(self);
            initType__6Goblinb(self, false);
            objectAddToRunList__FP10GameObject(self);
            wide(o, 160) = wide(o, 160) | 0x100000000ull;
            *reinterpret_cast<short*>(o + 146) = 50;
            *reinterpret_cast<short*>(o + 150) = 128;
            *reinterpret_cast<short*>(o + 152) = 255;
            *reinterpret_cast<short*>(o + 154) = 150;
            state = 0;
            *reinterpret_cast<short*>(o + 140) = 0;
            *reinterpret_cast<short*>(o + 142) = 128;
            *reinterpret_cast<short*>(o + 144) = 255;
            *reinterpret_cast<short*>(o + 148) = 0;
            // fall into the state 0 sequence
        case 0: {
            const unsigned roll = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + roll % 60u + 120u;
            if (word(o, 972) == 4u) animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 60), N, 3);
            else animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 108), N, 3);
            break;
        }
        case 1: {
            resetRouting__8Creature(self);
            Point3 destination;
            getSmartRandomDestination__8Creaturei(&destination, self, 300);
            *reinterpret_cast<Point3*>(o + 272) = destination;
            flt(o, 176) = 60.0f;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 140), N, 3);
            animGlobalSync__FP14AnimationState(a);
            break;
        }
        case 2:
            resetRouting__8Creature(self);
            flt(o, 176) = 150.0f;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 116), N, 3);
            animGlobalSync__FP14AnimationState(a);
            break;
        case 3:
            resetRouting__8Creature(self);
            flt(o, 176) = 60.0f;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 140), N, 3);
            animGlobalSync__FP14AnimationState(a);
            break;
        case 4:
            flt(o, 176) = 150.0f;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 116), N, 3);
            if (*reinterpret_cast<short*>(o + 572) == 3) {
                const unsigned roll = eRand__10GameObject(self);
                word(o, 256) = word(o, 808) + roll % 120u + 40u;
            }
            animGlobalSync__FP14AnimationState(a);
            break;
        case 5:
            if (word(o, 972) == 5u) animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 124), M, 3);
            else animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 100), M, 3);
            break;
        case 13: {
            if (word(o, 972) == 5u) animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 124), M, 3);
            else animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 100), M, 3);
            const unsigned roll = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + roll % 240u + 240u;
            break;
        }
        case 14:
            resetRouting__8Creature(self);
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 116), N, 3);
            flt(o, 176) = 150.0f;
            word(o, 256) = word(o, 808) + 60u;
            animGlobalSync__FP14AnimationState(a);
            break;
        case 15:
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 148), M, 3);
            word(o, 256) = word(o, 808) + 120u;
            wide(o, 160) = wide(o, 160) | 0x200ull;
            break;
        case 6: {
            if (word(o, 972) == 4u) {
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 60), M, 3);
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 52), M, M, 8);
            } else if (word(o, 972) == 5u) {
                animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 124), M, 3);
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 132), M, M, 8);
            } else {
                if (word(o, 992) != 0) o[980] = 1;
                const unsigned roll = eRand__10GameObject(self);
                if ((roll & 3u) == 0) o[980] = 0;
                if (static_cast<signed char>(o[980]) != 0) {
                    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 148), M, 3);
                } else {
                    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 100), M, 3);
                }
                if (word(o, 972) == 0) {
                    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 28), M, M, 8);
                } else if (word(o, 972) - 1u < 3u) {
                    const unsigned pick = eRand__10GameObject(self);
                    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 28 + 8 * (pick & 1u)), M, M, 8);
                }
            }
            if (word(o, 992) != 0) {
                const unsigned roll = eRand__10GameObject(self);
                word(o, 256) = roll % 60u + 20u;
            } else {
                const unsigned roll = eRand__10GameObject(self);
                word(o, 256) = roll % 20u;
            }
            break;
        }
        case 9:
            if (word(o, 972) == 4u) animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 60), N, 3);
            else animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 108), N, 3);
            break;
        case 8: {
            const short current = *reinterpret_cast<short*>(o + 572);
            if (current == 9 || (current == 6 && (*reinterpret_cast<unsigned short*>(o + 294) & 0x8u) != 0)) {
                state = current;
            } else if ((word(o, 744) & 0x3c0000u) != 0) {
                const short d = static_cast<short>(static_cast<unsigned>(*reinterpret_cast<unsigned short*>(o + 748)) -
                                                   *reinterpret_cast<unsigned short*>(o + 56));
                if (d < -26767 || !(d < 26768)) {
                    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 164), M, M, 2064);
                }
            } else if ((wide(o, 160) & 0x200ull) != 0) {
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 156), M, M, 16);
            } else {
                const unsigned roll = eRand__10GameObject(self);
                animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(a, header(self, 76 + 8 * (roll % 3u)), M, M, 16);
            }
            break;
        }
        case 10: {
            char dead[] = "dead";
            if (findTagInt__10GameObjectPci(self, dead, 0) == 0) {
                const Point3 p = *reinterpret_cast<Point3*>(o + 16);
                Point3 low, high;
                low.x = p.x - 96.0f; low.y = p.y - 96.0f; low.z = p.z - 96.0f;
                high.x = p.x + 96.0f; high.y = p.y + 96.0f; high.z = p.z + 96.0f;
                void* found[128];
                objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 128);
                int count = 0;
                if (found[0] != nullptr) {
                    unsigned i = 0;
                    void* next;
                    do {
                        void* current = found[i];
                        ++i;
                        next = found[i];
                        if (((*reinterpret_cast<unsigned*>(static_cast<unsigned char*>(current) + 12) & 0x68u) ^ 0x28u) == 0) ++count;
                    } while (next != nullptr);
                }
                if (!(count < 3)) word(o, 1136) = 1;
            }
            if ((word(o, 12) & 2u) == 0) {
                word(o, 12) = word(o, 12) & ~1u;
                dropItem__6Goblini(self, 1);
                dropItem__6Goblini(self, 2);
            }
            if (findTagInt__10GameObjectPci(self, dead, 0) != 0) word(o, 12) = word(o, 12) | 1u;
            animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(a, header(self, 68), N, 6);
            if ((wide(o, 160) & 0x100000000ull) == 0) {
                dropTreasure__8CreatureG6Point3iffffffPc(self, *reinterpret_cast<Point3*>(o + 16), *reinterpret_cast<short*>(o + 56),
                                                         36.0f, 2.0f, 0x1.ccccccp+0f, 0x1.333332p-1f, 2.0f, 0.25f, nullptr);
                if (word(o, 972) == 4u && eRand__10GameObject(self) < 6000u) {
                    char arrows[] = "arrows";
                    dropItem__4itemG6Point3Pc(*reinterpret_cast<Point3*>(o + 16), arrows);
                }
            }
            w10g::fn_from_word<void (*)(void*)>(w10g::entry_function(self, 192))(w10g::adjusted(self, 192));
            break;
        }
        default:
            break;
        }
    }
    *reinterpret_cast<unsigned short*>(o + 572) = static_cast<unsigned short>(state);
}
