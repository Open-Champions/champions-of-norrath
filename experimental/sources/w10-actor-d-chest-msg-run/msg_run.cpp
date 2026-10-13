// Observed msg_run__5Chest at 0x002189e8, 1228 bytes.
// First advances the AnimationState at object+92 with animAdvanceAnimation__FP14AnimationStateP6
// Point3P6Point2Ps (the Point3 at object+16, null, null). Then, on the signed byte at +88:
//  Nonzero (the opened chest): m is the pointer stored at +36.
//   1. modelGetCharAttachmentPos(model = word at m+12, a copy of the Point3 at +16, short +56,
//      null state, 0, out = local Point3 P, null, 1.0) fills P.
//   2. Two particles: each draws a random point V in the unit ball by rejection (three eRand calls,
//      each (float)r * 2^-16 doubled minus 1.0; retry while 1.0 < x*x + y*y + z*z, summed in that
//      order), makes V.z non-negative, then scales V and calls P_AddParticle(def, &P, &V, 0.0):
//      the first uses the 80-byte def at 0x7d7720 with V scaled by (5, 5, 25), the second the def
//      at 0x7d7770 with V scaled by (1.28, 1.28, 1.28 * 24.72) (the two singles at gp-25608 and
//      gp-25604).
//   3. Only if (float)(frameNm - word +80) is greater than 90.0: Q = (+16, +20, +24 + 14.7)
//      (single at gp-25600); the "fx.lmp" lump entry "spel_we_explarrow_hit.anm" is played with
//      soundPlaySequence(header, object, 1.0); makeExplosion(Q, 2.0); objectRadialDamage(object,
//      copy of the Point3 at +16, (int)((worldDifficulty - 2.0) * 4.0 + 15.0), 40.0, 180.0,
//      0x40800, 0, 0); a Point3 (+16, +20, +24 + 1.0) goes to floorAdd(point, (short)eRand(), 48.0,
//      9, 0); a 188-byte LightEffect is built (when the allocation succeeds) at a copy of the
//      Point3 at +16 with floats 128, 100, 78, -32, 0, 0, 0, 0, 168, 60, 1.0 and 0.35 (the single
//      at gp-25596), no object and 0; then the object is deleted through virtual slot byte 8 of the
//      table at +72 with the int 3 (the destructor slot, flags 3).
//  Zero: if bit 0x80 of the unsigned halfword at +98 is set, word +132 = 2, then
//   objectDoMyActions(object, 1) and objectRemoveFromRunList(object).
// The singles that are not exact decimals are written as hexadecimal float literals because the
// reviewed compiler truncates decimal literals. Inferred: this is the Chest's per-frame body while it bursts open and then disappears;
// the two statics are particle definitions owned by the Chest translation unit's static
// initializer, which is not part of this package. Unknown: field names, and what +80, +88 and
// +98 hold. Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

struct Point3 {
    float x, y, z;
};

extern "C" {
void animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, Point3* position, void* second, short* third);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(void* model, const Point3* position, short type, void* state, int a, Point3* out, Point3* second, float scale);
int eRand__10GameObject(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float value);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void makeExplosion__FG6Point3f(const Point3* position, float size);
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* object, const Point3* position, int a, float b, float c, int d, int e, int f);
void floorAdd__FG6Point3sfii(const Point3* position, short a, float b, int c, int d);
void* blockAlloc__Fi(int size);
void lightEffect_construct(void* self, const Point3* position, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7,
                           float f8, float f9, float f10, float f11, void* object, int flags)
    __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void objectDoMyActions__FP10GameObjecti(void* object, int value);
void objectRemoveFromRunList__FP10GameObject(void* object);
extern int frameNm;
extern float worldDifficulty;
extern unsigned char norrath_w10_actor_d_view_7d7720[80];
extern unsigned char norrath_w10_actor_d_view_7d7770[80];
}

namespace {
template <typename T>
inline T& field(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }

inline __attribute__((always_inline)) void random_in_unit_ball(void* self, Point3& v) {
    do {
        float t = static_cast<float>(eRand__10GameObject(self)) * 1.52587890625e-05f;
        v.x = t + t - 1.0f;
        t = static_cast<float>(eRand__10GameObject(self)) * 1.52587890625e-05f;
        v.y = t + t - 1.0f;
        t = static_cast<float>(eRand__10GameObject(self)) * 1.52587890625e-05f;
        v.z = t + t - 1.0f;
    } while (1.0f < v.x * v.x + v.y * v.y + v.z * v.z);
    if (v.z < 0.0f) v.z = -v.z;
}
}  // namespace

extern "C" void msg_run__5Chest(void* self) {
    unsigned char* o = static_cast<unsigned char*>(self);
    Point3* position = reinterpret_cast<Point3*>(o + 16);
    animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(o + 92, position, nullptr, nullptr);
    if (static_cast<signed char>(o[88]) != 0) {
        Point3 attach;
        const Point3 positionCopy = *position;
        modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
            w10_actor_d::word_at(w10_actor_d::word_at(o, 36), 12), &positionCopy, field<short>(o, 56), nullptr, 0, &attach, nullptr, 1.0f);
        Point3 velocity;
        random_in_unit_ball(self, velocity);
        velocity.x *= 5.0f;
        velocity.y *= 5.0f;
        velocity.z = velocity.z * 5.0f * 5.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w10_actor_d_view_7d7720, &attach, &velocity, 0.0f);
        random_in_unit_ball(self, velocity);
        velocity.x *= 0x1.47ae14p+0f;
        velocity.y *= 0x1.47ae14p+0f;
        velocity.z = velocity.z * 0x1.47ae14p+0f * 0x1.8b851ep+4f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_w10_actor_d_view_7d7770, &attach, &velocity, 0.0f);
        if (90.0f < static_cast<float>(frameNm - field<int>(o, 80))) {
            Point3 burst = {position->x, position->y, position->z + 0x1.d66666p+3f};
            void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_we_explarrow_hit.anm"));
            soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, self, 1.0f);
            makeExplosion__FG6Point3f(&burst, 2.0f);
            const Point3 damagePoint = *position;
            objectRadialDamage__FP10GameObjectG6Point3iffiii(self, &damagePoint, static_cast<int>((worldDifficulty - 2.0f) * 4.0f + 15.0f), 40.0f, 180.0f, 0x40800, 0, 0);
            Point3 floorPoint = {position->x, position->y, position->z + 1.0f};
            floorAdd__FG6Point3sfii(&floorPoint, static_cast<short>(eRand__10GameObject(self)), 48.0f, 9, 0);
            void* light = blockAlloc__Fi(188);
            if (light != nullptr) {
                const Point3 lightPoint = *position;
                lightEffect_construct(light, &lightPoint, 128.0f, 100.0f, 78.0f, -32.0f, 0.0f, 0.0f, 0.0f, 0.0f, 168.0f, 60.0f, 1.0f, 0x1.666666p-2f, nullptr, 0);
            }
            if (self != nullptr) w10_actor_d::call_slot(self, 8, 3);
        }
    } else if ((field<unsigned short>(o, 98) & 0x80u) != 0) {
        field<int>(o, 132) = 2;
        objectDoMyActions__FP10GameObjecti(self, 1);
        objectRemoveFromRunList__FP10GameObject(self);
    }
}
