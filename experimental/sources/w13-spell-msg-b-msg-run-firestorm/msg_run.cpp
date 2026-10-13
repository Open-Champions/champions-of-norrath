// Observed msg_run__9FireStorm at 0x0029c0a0, 2204 bytes. O is the object, frameNm the global frame counter. Words of O:
// start = the signed word at +152, end = the signed word at +148, Q = the word at +76 (an object), W = the object at word
// Q+12, A = the word at +140, B = the object at word +144, R = the signed word at +156, S = the float at +160. "entry k" is
// the entry at table byte k of the table whose address is the word at an object's +72 (or, for B, at +96), called on that
// object adjusted by the entry's signed halfword. "unit vector" means a vector drawn inside the unit ball by rejection with
// eRand__10GameObject(O) (each component float(roll) * 2^-16 doubled minus 1, drawn x, y, z in that order, redrawn while
// 1.0 < x*x + y*y + z*z). Steps in order:
//  1. Nothing happens unless start is below frameNm. When frameNm is not below end, or Q is zero: entry 8 of O is called
//     with 3 and that ends it.
//  2. rem = end - frameNm. When S is below float(R): n = B's entry 112 called with (2, A, O+76); when frameNm - (end - 60n)
//     is 31 or more, S gains 1.0. Then the scale F: for rem of 61 or more, F = 2S + float((int)((isin(frameNm * 3000) +
//     isin(frameNm * 1500)) * (S / float(R) * 12.0))) (S read again), else F = 2S * (float(rem) * 0.016666668). The floats at
//     +108 and +116 become F, those at +112 and +120 become 1.0 / (F * F).
//  3. Every 15th frame: P = (u.x * float(R) + O.x, u.y * float(R) + O.y, 240.022 + O.z) for a unit vector u; E = a second unit
//     vector times float(R) plus O+16, with E.z replaced by worldFindHeight__FP12_worldHeaderG6Point3f(world, E, 12.0); the
//     vector V = E - P is normalised with the EE reciprocal square root and scaled by 15.0007; the size z = float(A) * c * 0.5
//     + 0.3, c being 0.05 when Q is non-zero and W's word at +12 has bit 0x40 set, else 0.02. A 112-byte block from
//     blockAlloc__Fi, when it succeeds, is constructed with the GameObject Point3 constructor (position P, no resource map, no
//     arguments), then: word +84 = B (read before the constructor), flags (word +12) |= 0x421d, word +72 = the _vt$14FireStormFlare
//     table, word +76 = A (read before the constructor), position +16 = P, word +88 = 0, halfword +92 = 0, word +108 = frameNm +
//     120, vector +96 = V, float +28 = z * 12.0, float +80 = z; then objectUpdateInGrid__FP10GameObject and
//     objectAddToRunList__FP10GameObject.
//  4. Every 30th frame: the objects in the box O+16 -/+ float(R) are found with objectFindInBox__FG6Point3T0PP10GameObjecti
//     (capacity 64). For each listed T whose W entry 160 (called on W with T) is non-zero: h = B's entry 112 called with (0, A,
//     O+76), halved toward zero; a local item description (halfwords at +16..+22 and +24..+30 zeroed, bytes at +32 and +33 = -1,
//     four 8-byte slots at +36 starting -1) gets in its first free slot {21, h, h + (int)(float(h) * float(r1) * 2^-16 * 0.2), 0}
//     (halfwords +0, +2, +4, +6), r1 = eRand__10GameObject(O); gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(T,
//     W, 0.0, 0.0, 100000, 0, 0, 64, description, false) runs. When at least one T was hit,
//     lumpFindResource__FPcT0("fx.lmp", "spel_ew_firestorm_hit.anm") and soundPlaySequence__FP15AnimationHeaderP10GameObjectf(that, O, 1.0).
// The image tests O for null before the entry 8 call; that test cannot be true here. Inferred: a fire storm spell that
// spawns flares and damages what is in range. Unknown: what the words at +140, +144, +148, +152 and +156 hold exactly.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern int frameNm;
extern void* world;
extern unsigned char vt_14FireStormFlare[] __asm__("_vt$14FireStormFlare");
unsigned eRand__10GameObject(void* self);
float isin__Fi(int angle);
float worldFindHeight__FP12_worldHeaderG6Point3f(void* world, Point3 position, float radius);
void* blockAlloc__Fi(int size);
unsigned __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(void* self, Point3 position, void* resourceMap, char** arguments);
void objectUpdateInGrid__FP10GameObject(void* object);
void objectAddToRunList__FP10GameObject(void* object);
void objectFindInBox__FG6Point3T0PP10GameObjecti(Point3 low, Point3 high, void** found, int capacity);
int gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(void* a, void* b, float x, float y, int p, int q, int r, int s, void* description,
                                                                      bool flag);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);

namespace {
unsigned roll(unsigned char* o) { return eRand__10GameObject(o); }
}  // namespace

void msg_run__9FireStorm(void* self) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (!(sword(o, 152) < frameNm)) return;
    if (!(frameNm < sword(o, 148)) || word(o, 76) == 0u) {
        call_entry(o, 8, 3);
        return;
    }
    const int rem = sword(o, 148) - frameNm;
    float s = real(o, 160);
    if (s < static_cast<float>(sword(o, 156))) {
        const int n = static_cast<int>(call_entry_from(from_word(word(o, 144)), 96, 112, 2, word(o, 140), to_word(o + 76)));
        if (frameNm - (sword(o, 148) - n * 60) >= 31) {
            real(o, 160) = real(o, 160) + 1.0f;
            s = real(o, 160);
        } else {
            s = real(o, 160);
        }
    }
    float scale;
    if (rem >= 61) {
        const float twice = s + s;
        float t = isin__Fi(frameNm * 3000);
        t = t + isin__Fi(frameNm * 1500);
        float ratio = real(o, 160) / static_cast<float>(sword(o, 156));
        ratio = ratio * 12.0f;
        t = t * ratio;
        scale = twice + static_cast<float>(static_cast<int>(t));
    } else {
        const float twice = s + s;
        scale = twice * (static_cast<float>(rem) * bits(0x3c888888u));
    }
    real(o, 108) = scale;
    real(o, 116) = scale;
    const float inverse = 1.0f / (scale * scale);
    real(o, 120) = inverse;
    real(o, 112) = inverse;
    if (frameNm % 15 == 0) {
        Point3 d;
        unit_ball(o, d, roll);
        const float r = static_cast<float>(sword(o, 156));
        Point3 position;
        position.x = d.x * r + real(o, 16);
        position.y = d.y * r + real(o, 20);
        position.z = bits(0x437005b8u) + real(o, 24);
        Point3 e;
        unit_ball(o, e, roll);
        const float r2 = static_cast<float>(sword(o, 156));
        e.x = e.x * r2 + real(o, 16);
        e.y = e.y * r2 + real(o, 20);
        e.z = e.z * r2 + real(o, 24);
        const float height = worldFindHeight__FP12_worldHeaderG6Point3f(world, e, 12.0f);
        e.z = height;
        Point3 v;
        v.x = e.x - position.x;
        v.y = e.y - position.y;
        v.z = e.z - position.z;
        const float inverse_length_v = inverse_length(v.x * v.x + v.y * v.y + v.z * v.z);
        v.x = v.x * inverse_length_v;
        v.y = v.y * inverse_length_v;
        v.z = v.z * inverse_length_v;
        v.x = v.x * bits(0x417002d2u);
        v.y = v.y * bits(0x417002d2u);
        v.z = v.z * bits(0x417002d2u);
        float c;
        if (word(o, 76) != 0u && (word(from_word(word(from_word(word(o, 76)), 12)), 12) & 0x40u)) c = bits(0x3d4cccccu);
        else c = bits(0x3ca3d70au);
        const float size = static_cast<float>(sword(o, 140)) * c * 0.5f + bits(0x3e999999u);
        unsigned char* const block = static_cast<unsigned char*>(blockAlloc__Fi(112));
        if (block != nullptr) {
            Point3 where = position;
            Point3 heading = v;
            const unsigned a = word(o, 140);
            const unsigned b = word(o, 144);
            __10GameObjectG6Point3PQ210GameObject11resourceMapPPc(block, where, nullptr, nullptr);
            word(block, 84) = b;
            const unsigned flags = word(block, 12) | 0x421du;
            word(block, 72) = to_word(vt_14FireStormFlare);
            word(block, 12) = flags;
            word(block, 76) = a;
            *reinterpret_cast<Point3*>(block + 16) = where;
            real(block, 28) = size * 12.0f;
            word(block, 88) = 0u;
            half(block, 92) = 0;
            word(block, 108) = static_cast<unsigned>(frameNm + 120);
            *reinterpret_cast<Point3*>(block + 96) = heading;
            real(block, 80) = size;
            objectUpdateInGrid__FP10GameObject(block);
            objectAddToRunList__FP10GameObject(block);
        }
    }
    if (frameNm % 30 != 0) return;
    const float r = static_cast<float>(sword(o, 156));
    Point3 low, high;
    low.x = real(o, 16) - r;
    low.y = real(o, 20) - r;
    low.z = real(o, 24) - r;
    high.x = real(o, 16) + r;
    high.y = real(o, 20) + r;
    high.z = real(o, 24) + r;
    void* found[64];
    objectFindInBox__FG6Point3T0PP10GameObjecti(low, high, found, 64);
    bool hit = false;
    if (found[0] != nullptr) {
        unsigned char* const w = from_word(word(from_word(word(o, 76)), 12));
        int index = 0;
        do {
            void* const target = found[index];
            if (call_entry(w, 160, to_word(static_cast<unsigned char*>(target))) != 0u) {
                const int amount = static_cast<int>(call_entry_from(from_word(word(o, 144)), 96, 112, 0, word(o, 140), to_word(o + 76)));
                const int half_amount = amount / 2;
                unsigned char description[80];
                for (int k = 0; k < 4; ++k) half(description, 60 - 8 * k) = -1;
                description[33] = 0xff;
                description[32] = 0xff;
                for (int k = 0; k < 4; ++k) {
                    half(description, 16 + 2 * k) = 0;
                    half(description, 24 + 2 * k) = 0;
                }
                float f = static_cast<float>(half_amount);
                const unsigned r1 = eRand__10GameObject(o);
                f = f * (static_cast<float>(static_cast<int>(r1)) * bits(0x37800000u));
                f = f * bits(0x3e4cccccu);
                const int upper = half_amount + static_cast<int>(f);
                for (int k = 0; k < 4; ++k) {
                    unsigned char* const slot = description + 36 + 8 * k;
                    if (half(slot, 0) == -1 || half(slot, 0) == 21) {
                        half(slot, 0) = 21;
                        half(slot, 6) = 0;
                        half(slot, 2) = static_cast<short>(half_amount);
                        half(slot, 4) = static_cast<short>(upper);
                        break;
                    }
                }
                gameAttackResolver__FP10GameObjectT0ffiiiiPQ24item11descriptionb(target, w, 0.0f, 0.0f, 100000, 0, 0, 64, description, false);
                hit = true;
            }
            ++index;
        } while (found[index] != nullptr);
    }
    if (hit) {
        void* const sequence = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_ew_firestorm_hit.anm"));
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(sequence, o, 1.0f);
    }
}
}
