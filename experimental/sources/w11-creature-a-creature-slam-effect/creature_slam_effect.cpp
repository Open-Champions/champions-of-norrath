// Observed creatureSlamEffect__8CreatureP8Creaturei at 0x00112438, 884 bytes.
// Arguments: the creature, another creature and an int k. A null other does nothing. The creature's
// model is the word at byte 4 of the 8-byte entry k of the table pointed to by creature+36. With
// n1 = modelGetNmShadowPoints(that model) and n2 = modelGetNmShadowPoints(the model returned by the
// virtual function at byte 4 of the table entry at (table at other+72) + 128, called on the other
// adjusted by the entry's signed halfword), 100 rounds are made. Each round:
//   * eRand__10GameObject(creature) % n1 selects a shadow point of the creature's model, taken with
//     modelGetShadowPos(model, animation at creature+288, creature position (+16), the creature's
//     halfword at +56, index, out a, scratch, null matrix, 1.0);
//   * the other's model is fetched again by the virtual call and eRand__10GameObject(creature) % n2
//     selects a shadow point of it, taken with the other's animation (+288), position (+16) and
//     halfword (+56), into out b;
//   * the velocity is (b - a) scaled by 30.0 / its length (the image's rsqrt.s with 1.0), and
//     P_AddParticle(slamParticle, &b, velocity, 0.0) is called.
// Afterwards the halfwords of the creature at +140, +142, +144, +146, +150 and +152 are set to 142,
// 58, 82, 64, 58 and 82. A 188-byte block from blockAlloc__Fi (null: return) is initialised with the
// LightEffect constructor, the point (creature x, creature y, creature z + 44.0), the floats
// 255.0, 103.0, 115.0, 0, 0, 0, 0, 0, 120.0, 60.0, 0.5 and 1.2 (0x3f999999), a null object and 0.
// Inferred: this is the impact flash and spray when one creature slams another.
// Unknown: the meaning of the shadow-point lookups' scratch float and matrix argument, and the halfwords.
#include "../w11-creature-a-common/words.hpp"

extern "C" {
extern unsigned char slamParticle[];
int eRand__10GameObject(void* self);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
int modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, w11_creature_a::Point3 position, short angle, int index, w11_creature_a::Point3* out, float* scratch, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, w11_creature_a::Point3* position, w11_creature_a::Point3* velocity, float value);
void* blockAlloc__Fi(int bytes);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const w11_creature_a::Point3* point,
    float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float p13, float p14, void* object, int flag);
}

extern "C" void creatureSlamEffect__8CreatureP8Creaturei(void* self, void* other, int k) {
    using namespace w11_creature_a;
    if (other == nullptr) return;
    unsigned char* entry = at<unsigned char*>(self, 36) + k * 8;
    const int self_points = modelGetNmShadowPoints__FP12_modelHeader(at<void*>(entry, 4));
    Slot model_slot = slot(other, 128);
    const int other_points = modelGetNmShadowPoints__FP12_modelHeader(reinterpret_cast<void* (*)(void*)>(model_slot.function)(model_slot.self));
    for (int round = 99; round >= 0; --round) {
        Point3 from, to, velocity;
        float scratch_from, scratch_to;
        const int pick_from = eRand__10GameObject(self) % self_points;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            at<void*>(at<unsigned char*>(self, 36) + k * 8, 4), static_cast<unsigned char*>(self) + 288, at<Point3>(self, 16), at<short>(self, 56), pick_from, &from, &scratch_from, nullptr, 1.0f);
        Slot again = slot(other, 128);
        void* other_model = reinterpret_cast<void* (*)(void*)>(again.function)(again.self);
        const int pick_to = eRand__10GameObject(self) % other_points;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            other_model, static_cast<unsigned char*>(other) + 288, at<Point3>(other, 16), at<short>(other, 56), pick_to, &to, &scratch_to, nullptr, 1.0f);
        velocity.x = to.x - from.x;
        velocity.y = to.y - from.y;
        velocity.z = to.z - from.z;
        float inverse;
        __asm__("rsqrt.s %0, %1, %2" : "=f"(inverse) : "f"(1.0f), "f"(velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z));
        velocity.x = velocity.x * inverse;
        velocity.y = velocity.y * inverse;
        velocity.z = velocity.z * inverse;
        velocity.x = velocity.x * 30.0f;
        velocity.y = velocity.y * 30.0f;
        velocity.z = velocity.z * 30.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(slamParticle, &to, &velocity, 0.0f);
    }
    at<short>(self, 140) = 142;
    at<short>(self, 142) = 58;
    at<short>(self, 144) = 82;
    at<short>(self, 146) = 64;
    at<short>(self, 150) = 58;
    at<short>(self, 152) = 82;
    void* block = blockAlloc__Fi(188);
    if (block == nullptr) return;
    Point3 offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 44.0f;
    Point3 point;
    point.x = at<float>(self, 16) + offset.x;
    point.y = at<float>(self, 20);
    point.z = at<float>(self, 24) + offset.z;
    __11LightEffectG6Point3ffffffffffffP10GameObjecti(block, &point, 255.0f, 103.0f, 115.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 120.0f, 60.0f, 0.5f, __builtin_bit_cast(float, 0x3f999999), nullptr, 0);
}
