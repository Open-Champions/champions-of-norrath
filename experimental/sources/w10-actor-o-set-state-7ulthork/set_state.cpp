// Observed setState__7Ulthorki at 0x001f5c90, 1368 bytes. R is the block at the word object+36,
// "anim" the animation state at +288, h the signed halfword at +572, b the unsigned halfword at
// +294, wait the word at +256, base the word at +808. A copy n of the argument goes to
// setStateProcessing__8CreaturePi (which may change it); a non-zero result ends the call with
// nothing else done. Otherwise n selects a case (the jump table has 15 words; n >= 15 as an
// unsigned value and 2, 4, 7, 11, 12 do nothing more) and the low halfword of n is stored at +572 at
// the end. "transition" is animAddTransitionTo(anim, header, rate, flags) and "oneShot" is
// animAddOneShot(anim, header, a, b, flags); slow is the .lit4 word 0x3e999999, fast 0x3dcccccc.
//  14: bits 0 and 3 of the word at +12 are cleared and bit 32 of the doubleword at +160 is set;
//     setParameters(this, 1.4, 1.2, 1.0, 1.0, 1.0, 1); the word at +960 becomes 23 when the signed
//     byte at +1104 is 2, otherwise 19 + 2 * (eRand & 1); objectAddToRunList(this); halfwords at
//     +140, +142, +144, +146, +148, +150, +152, +154 become 0, 128, 255, 50, 0, 128, 255, 150; n
//     becomes 0 and the case 0 steps follow.
//  0: wait = base + eRand % 120; transition(R[116], slow, 3).
//  1: resetRouting; getSmartRandomDestination(point, this, 200) goes to +272..283; wait = base +
//     eRand % 120 + 300; transition(R[140], slow, 0x803); animGlobalSync(anim).
//  3: resetRouting; unless h is 3 or 6: transition(R[132], fast, 0x803); animGlobalSync(anim).
//  5: transition(R[124], fast, 3).
//  6: oneShot(R[(eRand % 3) * 8 + 44], fast, fast, 8).
//  8: unless (h is 6 and bit 3 of b is set) or h is 9: oneShot(R[(eRand & 3) * 8 + 84], fast, fast,
//     16). In the excepted cases n becomes h.
//  9: transition(R[116], slow, 3).
//  10: unless bit 32 of the doubleword at +160 is set: dropTreasure(this, point +16, h56, 36, 2, 1.8,
//     0.6, 2, 0.25, null). Then transition(R[(eRand & 1) * 8 + 68], slow, 6). When bit 1 of the word
//     at +12 is clear and the word at +960 is non-zero: modelGetCharAttachmentPos(R[12], copy of the
//     point at +16, (short)(h56 - 16384), anim, 0, &A, &B, 1.0) with A starting as a copy of the point;
//     a 584-byte block is allocated and, when non-null, PhProp is constructed in it with (A, R[0],
//     R[w * 8], R[w * 8 + 8], 1, 16641, null, the float at +224) where w is the word at +960; the
//     word at +960 becomes 0. The table entry at (word +72) + 192 is called.
//  13: transition(R[124], fast, 3); oneShot(R[148], fast, fast, 8); wait = (int)((float)(base + 180)
//     + (float)eRand * 2^-16 * 180.0).
// h56 is the halfword at +56. The meaning of the states is unknown. Nothing here ran on the EE.
#include "../w10-actor-o-common/access.hpp"

extern "C" {
int setStateProcessing__8CreaturePi(void* self, int* state);
void resetRouting__8Creature(void* self);
void getSmartRandomDestination__8Creaturei(void* result, void* self, int range);
void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int flag);
void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flag);
void animGlobalSync__FP14AnimationState(void* state);
unsigned eRand__10GameObject(void* object);
void setParameters__8Creaturefffffi(void* self, float a, float b, float c, float d, float e, int flag);
void objectAddToRunList__FP10GameObject(void* object);
void dropTreasure__8CreatureG6Point3iffffffPc(void* self, Point3 position, int a, float b, float c, float d, float e, float f, float g, char* name);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, Point3 point, short yaw, void* state, int index, Point3* first, Point3* second, float scale);
void* blockAlloc__Fi(int size);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long d, int e, void* description, float scale);

void setState__7Ulthorki(void* self, int state) {
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
    auto one_shot = [&](unsigned offset, unsigned rate, int flag) {
        animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(anim, header(offset), bits(rate), bits(rate), flag);
    };
    const unsigned slow = 0x3e999999u;
    const unsigned fast = 0x3dccccccu;
    auto case_zero = [&]() {
        const unsigned r = eRand__10GameObject(self);
        word(o, 256) = word(o, 808) + r % 120u;
        transition(116, slow, 3);
    };
    switch (static_cast<unsigned>(n)) {
        case 14: {
            const unsigned flags = word(o, 12) & ~9u;
            const unsigned long long extra = dword(o, 160) | (0x8000ull << 17);
            word(o, 12) = flags;
            dword(o, 160) = extra;
            setParameters__8Creaturefffffi(self, bits(0x3fb33333u), bits(0x3f999999u), 1.0f, 1.0f, 1.0f, 1);
            if (static_cast<signed char>(o[1104]) == 2) word(o, 960) = 23;
            else word(o, 960) = 19u + ((eRand__10GameObject(self) & 1u) << 1);
            objectAddToRunList__FP10GameObject(self);
            half(o, 140) = 0;
            half(o, 146) = 50;
            half(o, 150) = 128;
            half(o, 152) = 255;
            half(o, 154) = 150;
            n = 0;
            half(o, 142) = 128;
            half(o, 144) = 255;
            half(o, 148) = 0;
            case_zero();
            break;
        }
        case 0:
            case_zero();
            break;
        case 1: {
            resetRouting__8Creature(self);
            Point3 point;
            getSmartRandomDestination__8Creaturei(&point, self, 200);
            real(o, 272) = point.x; real(o, 276) = point.y; real(o, 280) = point.z;
            const unsigned r = eRand__10GameObject(self);
            word(o, 256) = word(o, 808) + r % 120u + 300u;
            transition(140, slow, 0x803);
            animGlobalSync__FP14AnimationState(anim);
            break;
        }
        case 3:
            resetRouting__8Creature(self);
            if (half(o, 572) != 3 && half(o, 572) != 6) {
                transition(132, fast, 0x803);
                animGlobalSync__FP14AnimationState(anim);
            }
            break;
        case 5:
            transition(124, fast, 3);
            break;
        case 6: {
            const unsigned r = eRand__10GameObject(self);
            one_shot((r % 3u) * 8u + 44u, fast, 8);
            break;
        }
        case 13: {
            transition(124, fast, 3);
            one_shot(148, fast, 8);
            float wait = static_cast<float>(static_cast<int>(word(o, 808)) + 180);
            const unsigned r = eRand__10GameObject(self);
            wait = wait + static_cast<float>(static_cast<int>(r)) * bits(0x37800000u) * 180.0f;
            sword(o, 256) = static_cast<int>(wait);
            break;
        }
        case 8: {
            const int h = half(o, 572);
            const bool flag = (*reinterpret_cast<unsigned short*>(o + 294) & 8u) != 0;
            if ((h == 6 && flag) || h == 9) {
                n = h;
            } else {
                const unsigned r = eRand__10GameObject(self);
                one_shot((r & 3u) * 8u + 84u, fast, 16);
            }
            break;
        }
        case 9:
            transition(116, slow, 3);
            break;
        case 10: {
            if ((dword(o, 160) & (0x8000ull << 17)) == 0) {
                const Point3 position = *reinterpret_cast<Point3*>(o + 16);
                dropTreasure__8CreatureG6Point3iffffffPc(self, position, half(o, 56), 36.0f, 2.0f, bits(0x3fe66666u), bits(0x3f199999u), 2.0f, 0.25f, nullptr);
            }
            const unsigned r = eRand__10GameObject(self);
            transition((r & 1u) * 8u + 68u, slow, 6);
            if ((word(o, 12) & 2u) == 0 && word(o, 960) != 0) {
                Point3 first = *reinterpret_cast<Point3*>(o + 16);
                Point3 second;
                const Point3 query = *reinterpret_cast<Point3*>(o + 16);
                modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                    header(12), query, static_cast<short>(static_cast<unsigned short>(half(o, 56)) - 16384u), anim, 0, &first, &second, 1.0f);
                void* const prop = blockAlloc__Fi(584);
                if (prop != nullptr) {
                    unsigned char* const entry = block + word(o, 960) * 8u;
                    __6PhPropG6Point3PcN22liPQ24item11descriptionf(
                        prop, first, reinterpret_cast<char*>(from_word(word(block, 0))), reinterpret_cast<char*>(from_word(word(entry, 0))),
                        reinterpret_cast<char*>(from_word(word(entry, 8))), 1, 16641, nullptr, real(o, 224));
                }
                word(o, 960) = 0;
            }
            call_entry(o, 192);
            break;
        }
        default:
            break;
    }
    half(o, 572) = static_cast<short>(n);
}
}
