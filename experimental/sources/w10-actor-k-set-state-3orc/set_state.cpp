// Observed setState__3Orci at 0x00228ba8, 3244 bytes. The requested state is held in a local; the
// halfword at object+572 (the state) ends as that local, which some cases change. Words used:
// type = word +1116, now = word +808, nextAt = word +256, t = the pointer at +36 (read as an array
// of 8-byte records, t + 8*i + k holding a header word). Helpers, all on the animation state at
// object+288: ATT(h, f) is animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(state, h, f,
// 3); ONE(h, f, n) is animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(state, h, f, f, n);
// SYNC is animGlobalSync__FP14AnimationState(state). r is a fresh eRand__10GameObject(object).
// Common steps, in order:
//  1. setStateProcessing__8CreaturePi(object, &local) runs; a nonzero result returns at once with the
//     state halfword untouched.
//  2. Bit 9 (0x200) of the 64-bit word at +160 is cleared; a local at or above 18 (unsigned) does
//     nothing more except the final halfword store.
// Per local value (0.3, 0.1 and so on are the float words of the executable's small-data pool,
// written below as bit patterns):
//  17: word +12 &= 0xfffffff6; setWeapon__3Orc; initType__3Orcb(object, 0); objectAddToRunList;
//      bit 32 of the +160 word set; halfwords +146=50, +150=128, +152=255, +154=150, +140=0,
//      +142=128, +144=255, +148=0; the local becomes 0; then state 0.
//   0: nextAt = now + eRand%60 + 120, then the tail T.   9: the tail T alone.
//      T: type 4: ATT(t[316]); other types: ATT(t[16*(r%3)+252]); both with 0.3.
//   1: resetRouting; the point from getSmartRandomDestination(object, 300) goes to +272..+280;
//      then block A when type is 2, 3 or 5, else block B.
//   2: resetRouting; block A for those types, else float +176 = 170.0 and ATT(t[8*(r&1)+140]); SYNC.
//   3: resetRouting; block A for those types; type 0 as state 2; any other type block B.
//      A: float +176 = 90.0, ATT(t[8*(r&1)+332]); SYNC.  B: float +176 = 80.0, ATT(t[8*(r&1)+348]); SYNC.
//   4: type 2, 3 or 5: float +176 = 130.0, ATT(t[8*(r&1)+124]); else 170.0 and t[8*(r&1)+140]; then, when
//      the current halfword +572 is 3, nextAt = now + eRand%120 + 40; SYNC.
//   5: ATT with 0.1: type 4 t[308]; type 5 t[324]; word +1112 nonzero t[228]; else t[16*(r%3)+244].
//   6: type 4: ATT(t[308]) then ONE(t[180], 8); type 5: ATT(t[324]) then ONE(t[188], 8); otherwise
//      ATT(t[228]) when word +1112 is nonzero else ATT(t[16*(r%3)+244]), then, from word +972: 1, 3 or
//      5 gives ONE(t[8*(r&1)+156], 8); else a "near" test (type 1 with eRand%3 == 0, or type 2 or 3
//      with the low bit of eRand clear) gives ONE(t[8*(r%3)+196], 8) and failing it ONE(t[8*(r%3)+156], 8);
//      finally nextAt = eRand%10 + 10. All with 0.1.
//   8: when the current halfword +572 is 9, or is 6 with bit 3 of the halfword +294 set, the local
//      becomes that halfword; else ONE(t[8*(r&3)+92], 16) with 0.1.
//  10: deleteCommander (type 2 or 3) or leaveCommander; byte +40 = 70; dropItem__3Orci(object, 6) and
//      (object, 7); when blood level is 2 or more or the network mode is nonzero, word +744 is 8 and
//      eRand%100 < 25, a = eRand%5 and dropItem(object, a+1) unless (word +244 bit 22 and a-3 < 2) or
//      (bit 23 and a-1 < 2), which make the drop kind 0; the animation index is 3 for type 2 or 3,
//      6, 4, 5, 7, 8 for kinds 1..5, else eRand%5 + 4; ATT(t[8*index+4], 0.3) with mode 6; when bit 32
//      of the +160 word is clear dropTreasure__8CreatureG6Point3iffffffPc runs with the position,
//      halfword +56 and six floats, and for type 4 with eRand < 8000 a dropItem__4itemG6Point3Pc of
//      the text "arrows"; bit 0 of word +12 is cleared when bit 1 is clear; the table entry at
//      (word +72) + 192 is called on the object.
//  13: ATT(t[16*(r%3)+252]) with 0.1; nextAt = now + eRand%240 + 240.
//  14: resetRouting; when word +576 is nonzero and findCommander returns an object, +272..+280 becomes
//      the commander's position (+16) minus 150 times the unit vector from it to the enemy position
//      (the object at word 12 of word +576, position at +16), via rsqrt.s; with no commander +272 and
//      +276 become 0; then type 5: float +176 = 130.0, ATT(t[8*(r&1)+124]); other types: 170.0 and
//      t[8*(r&1)+140]; nextAt = now + 60; SYNC.
//  16: ATT(t[228]) with 0.1; nextAt = now + 120; bit 9 of the +160 word set.
// States 7, 11, 12 and 15 and values outside 0..17 do nothing more.
// Nothing here ran on the EE; what the fields mean is unknown.
#include "../w10-actor-k-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void setWeapon__3Orc(void* self);
void initType__3Orcb(void* self, int flag);
void objectAddToRunList__FP10GameObject(void* object);
unsigned eRand__10GameObject(void* self);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(Point3* out, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float blend, int mode);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int mode);
void animGlobalSync__FP14AnimationState(void* state);
unsigned char* findCommander__3Orc(void* self);
void deleteCommander__3Orc(void* self);
void leaveCommander__3Orc(void* self);
void dropItem__3Orci(void* self, int kind);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int angle, float a, float b, float c, float d, float e, float f, char* name);
void dropItem__4itemG6Point3Pc(Point3 position, char* name);
extern int g_BloodLevel;
extern signed char netNetworkMode;
}

namespace {
using w10k::bits;
using w10k::half;
using w10k::real;
using w10k::word;

const float kA = 0.3f;

void* header(unsigned char* o, unsigned offset) {
    return w10k::from_word(word(w10k::from_word(word(o, 36)), offset));
}
unsigned char* block(unsigned char* o) { return w10k::from_word(word(o, 36)); }

void attach(unsigned char* o, unsigned offset, float blend) {
    animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, header(o, offset), blend, 3);
}
void one_shot(unsigned char* o, unsigned offset, float blend, int mode) {
    animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(o + 288, header(o, offset), blend, blend, mode);
}

float rsqrt_one(float length_squared) {
#if defined(__mips__)
    float result;
    const float one = 1.0f;
    __asm__("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}
}  // namespace

extern "C" void setState__3Orci(void* self, int requested) {
    unsigned char* const o = static_cast<unsigned char*>(self);
    int state = requested;
    int type;
    unsigned r;
    unsigned drop;
    unsigned a;
    unsigned char* commander;
    unsigned char* enemy;
    if (setStateProcessing__8CreaturePi(self, &state)) return;
    w10k::dword(o, 160) &= ~0x200ull;
    switch (static_cast<unsigned>(state) < 18u ? state : 99) {
    case 17:
        word(o, 12) &= 0xfffffff6u;
        setWeapon__3Orc(self);
        initType__3Orcb(self, 0);
        objectAddToRunList__FP10GameObject(self);
        w10k::dword(o, 160) |= 0x100000000ull;
        half(o, 146) = 50;
        half(o, 150) = 128;
        half(o, 152) = 255;
        half(o, 154) = 150;
        state = 0;
        half(o, 140) = 0;
        half(o, 142) = 128;
        half(o, 144) = 255;
        half(o, 148) = 0;
        // fall through to state 0
    case 0:
        word(o, 256) = word(o, 808) + eRand__10GameObject(self) % 60u + 120u;
        goto tail;
    case 9:
    tail:
        if (word(o, 1116) == 4) {
            attach(o, 316, bits(0x3e999999u));
        } else {
            r = eRand__10GameObject(self);
            attach(o, 16 * (r % 3u) + 252, bits(0x3e999999u));
        }
        break;
    case 1:
        resetRouting__8Creature(self);
        {
            Point3 p;
            getSmartRandomDestination__8Creaturei(&p, self, 300);
            *reinterpret_cast<Point3*>(o + 272) = p;
        }
        type = static_cast<int>(word(o, 1116));
        if (static_cast<unsigned>(type - 2) < 2u || type == 5) goto block_a;
        goto block_b;
    case 2:
        resetRouting__8Creature(self);
        type = static_cast<int>(word(o, 1116));
        if (static_cast<unsigned>(type - 2) < 2u || type == 5) goto block_a;
        real(o, 176) = bits(0x432a0000u);
        r = eRand__10GameObject(self);
        attach(o, 8 * (r & 1u) + 140, bits(0x3e999999u));
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    case 3:
        resetRouting__8Creature(self);
        type = static_cast<int>(word(o, 1116));
        if (static_cast<unsigned>(type - 2) < 2u || type == 5) goto block_a;
        if (type != 0) goto block_b;
        real(o, 176) = bits(0x432a0000u);
        r = eRand__10GameObject(self);
        attach(o, 8 * (r & 1u) + 140, bits(0x3e999999u));
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    block_a:
        real(o, 176) = bits(0x42b40000u);
        r = eRand__10GameObject(self);
        attach(o, 8 * (r & 1u) + 332, bits(0x3e999999u));
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    block_b:
        real(o, 176) = bits(0x42a00000u);
        r = eRand__10GameObject(self);
        attach(o, 8 * (r & 1u) + 348, bits(0x3e999999u));
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    case 4:
        type = static_cast<int>(word(o, 1116));
        if (static_cast<unsigned>(type - 2) < 2u || type == 5) {
            real(o, 176) = bits(0x43020000u);
            r = eRand__10GameObject(self);
            attach(o, 8 * (r & 1u) + 124, bits(0x3e999999u));
        } else {
            real(o, 176) = bits(0x432a0000u);
            r = eRand__10GameObject(self);
            attach(o, 8 * (r & 1u) + 140, bits(0x3e999999u));
        }
        if (half(o, 572) == 3) word(o, 256) = word(o, 808) + eRand__10GameObject(self) % 120u + 40u;
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    case 13:
        r = eRand__10GameObject(self);
        attach(o, 16 * (r % 3u) + 252, bits(0x3dcccccc));
        word(o, 256) = word(o, 808) + eRand__10GameObject(self) % 240u + 240u;
        break;
    case 5:
        type = static_cast<int>(word(o, 1116));
        if (type == 4) {
            attach(o, 308, bits(0x3dcccccc));
        } else if (type == 5) {
            attach(o, 324, bits(0x3dcccccc));
        } else if (word(o, 1112) != 0) {
            attach(o, 228, bits(0x3dcccccc));
        } else {
            r = eRand__10GameObject(self);
            attach(o, 16 * (r % 3u) + 244, bits(0x3dcccccc));
        }
        break;
    case 16:
        attach(o, 228, bits(0x3dcccccc));
        word(o, 256) = word(o, 808) + 120u;
        w10k::dword(o, 160) |= 0x200ull;
        break;
    case 6:
        type = static_cast<int>(word(o, 1116));
        if (type == 4) {
            attach(o, 308, bits(0x3dcccccc));
            one_shot(o, 180, bits(0x3dcccccc), 8);
        } else if (type == 5) {
            attach(o, 324, bits(0x3dcccccc));
            one_shot(o, 188, bits(0x3dcccccc), 8);
        } else {
            if (word(o, 1112) != 0) {
                attach(o, 228, bits(0x3dcccccc));
            } else {
                r = eRand__10GameObject(self);
                attach(o, 16 * (r % 3u) + 244, bits(0x3dcccccc));
            }
            type = static_cast<int>(word(o, 972));
            if (type == 1 || type == 3 || type == 5) {
                r = eRand__10GameObject(self);
                one_shot(o, 8 * (r & 1u) + 156, bits(0x3dcccccc), 8);
            } else {
                bool near = false;
                if (word(o, 1116) == 1 && eRand__10GameObject(self) % 3u == 0) {
                    near = true;
                } else if (static_cast<unsigned>(word(o, 1116) - 2u) < 2u && (eRand__10GameObject(self) & 1u) == 0) {
                    near = true;
                }
                if (near) {
                    r = eRand__10GameObject(self);
                    one_shot(o, 8 * (r % 3u) + 196, bits(0x3dcccccc), 8);
                } else {
                    r = eRand__10GameObject(self);
                    one_shot(o, 8 * (r % 3u) + 156, bits(0x3dcccccc), 8);
                }
            }
        }
        word(o, 256) = eRand__10GameObject(self) % 10u + 10u;
        break;
    case 8:
        if (half(o, 572) == 9 || (half(o, 572) == 6 && (*reinterpret_cast<unsigned short*>(o + 294) & 8) != 0)) {
            state = half(o, 572);
        } else {
            r = eRand__10GameObject(self);
            one_shot(o, 8 * (r & 3u) + 92, bits(0x3dcccccc), 16);
        }
        break;
    case 14:
        resetRouting__8Creature(self);
        if (word(o, 576) != 0) {
            commander = findCommander__3Orc(self);
            if (commander != nullptr) {
                *reinterpret_cast<Point3*>(o + 272) = *reinterpret_cast<Point3*>(commander + 16);
                enemy = w10k::from_word(word(w10k::from_word(word(o, 576)), 12)) + 16;
                const float dz = *reinterpret_cast<float*>(enemy + 8) - *reinterpret_cast<float*>(commander + 24);
                const float dx = *reinterpret_cast<float*>(enemy + 0) - *reinterpret_cast<float*>(commander + 16);
                const float dy = *reinterpret_cast<float*>(enemy + 4) - *reinterpret_cast<float*>(commander + 20);
                const float length_squared = dx * dx + dy * dy + dz * dz;
                const float inverse = rsqrt_one(length_squared);
                const float ux = dx * inverse;
                const float uy = dy * inverse;
                const float uz = dz * inverse;
                const float ky = uy * bits(0x43160000u);
                const float kz = uz * bits(0x43160000u);
                const float kx = ux * bits(0x43160000u);
                real(o, 276) = real(o, 276) - ky;
                real(o, 280) = real(o, 280) - kz;
                real(o, 272) = real(o, 272) - kx;
            } else {
                real(o, 272) = 0.0f;
                real(o, 276) = 0.0f;
            }
        }
        if (word(o, 1116) == 5) {
            real(o, 176) = bits(0x43020000u);
            r = eRand__10GameObject(self);
            attach(o, 8 * (r & 1u) + 124, bits(0x3e999999u));
        } else {
            real(o, 176) = bits(0x432a0000u);
            r = eRand__10GameObject(self);
            attach(o, 8 * (r & 1u) + 140, bits(0x3e999999u));
        }
        word(o, 256) = word(o, 808) + 60u;
        animGlobalSync__FP14AnimationState(o + 288);
        break;
    case 10:
        if (static_cast<unsigned>(word(o, 1116) - 2u) < 2u) deleteCommander__3Orc(self); else leaveCommander__3Orc(self);
        o[40] = 70;
        dropItem__3Orci(self, 6);
        drop = 0;
        dropItem__3Orci(self, 7);
        if ((g_BloodLevel >= 2 || netNetworkMode != 0) && word(o, 744) == 8 && eRand__10GameObject(self) % 100u < 25u) {
            a = eRand__10GameObject(self) % 5u & 0xffffu;
            drop = a + 1;
            if ((word(o, 244) & 0x400000u) != 0 && a - 3u < 2u) {
                drop = 0;
            } else if ((word(o, 244) & 0x800000u) != 0 && a - 1u < 2u) {
                drop = 0;
            } else {
                dropItem__3Orci(self, static_cast<int>(drop));
            }
        }
        if (static_cast<unsigned>(word(o, 1116) - 2u) < 2u) {
            a = 3;
        } else if (drop == 1) {
            a = 6;
        } else if (drop == 2) {
            a = 4;
        } else if (drop == 3) {
            a = 5;
        } else if (drop == 4) {
            a = 7;
        } else if (drop == 5) {
            a = 8;
        } else {
            a = eRand__10GameObject(self) % 5u + 4u;
        }
        animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(o + 288, w10k::from_word(word(block(o), 8 * a + 4)), bits(0x3e999999u), 6);
        if ((w10k::dword(o, 160) & 0x100000000ull) == 0) {
            const Point3 here = *reinterpret_cast<Point3*>(o + 16);
            dropTreasure__8CreatureG6Point3iffffffPc(self, here, half(o, 56), bits(0x42100000u), bits(0x40000000u), bits(0x40333333u),
                bits(0x3f199999u), bits(0x40199999u), bits(0x3e800000u), nullptr);
            if (word(o, 1116) == 4 && eRand__10GameObject(self) < 8000u) {
                const Point3 at = *reinterpret_cast<Point3*>(o + 16);
                dropItem__4itemG6Point3Pc(at, const_cast<char*>("arrows"));
            }
        }
        if ((word(o, 12) & 2u) == 0) word(o, 12) &= ~1u;
        w10k::call_entry(o, 192);
        break;
    default:
        break;
    }
    half(o, 572) = static_cast<short>(state);
}
