// Observed superorcSlamEffect__8SuperOrcP8Creaturei at 0x0022b0c8, 884 bytes.
// The same shape as creatureSlamEffect__8CreatureP8Creaturei, applied to a SuperOrc, with these
// differences. Arguments: the SuperOrc, a creature and an int k. There is no null test on the
// creature. The particle definition is the file-local slamParticle at 0x007d7af0 (the Creature
// version uses a different local of the same name at 0x004e20d0; a third copy exists at
// 0x007cb6e0). The 100 rounds, shadow-point picks, 30.0 velocity scale and
// P_AddParticle(slamParticle, &b, velocity, 0.0) are as described there. Afterwards the SuperOrc's
// halfwords at +140, +142, +144, +146, +148, +150, +152 and +154 are set to 110, 183, 255, 100, 110,
// 183, 255 and 100. A 188-byte block from blockAlloc__Fi (null: return) is initialised with the
// LightEffect constructor, the point (x, y, z + 44.0) of the SuperOrc, the floats 110.0, 183.0,
// 255.0, 100.0, 0, 0, 0, 0, 120.0, 60.0, 0.5 and 1.2 (0x3f999999), a null object and 0.
// Inferred: the impact flash and spray when a super orc slams a creature.
// Unknown: the meaning of the halfwords and of the shadow-point scratch and matrix arguments.
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

extern "C" void superorcSlamEffect__8SuperOrcP8Creaturei(void* self, void* other, int k) {
    using namespace w11_creature_a;
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
    at<short>(self, 140) = 110;
    at<short>(self, 142) = 183;
    at<short>(self, 144) = 255;
    at<short>(self, 146) = 100;
    at<short>(self, 148) = 110;
    at<short>(self, 150) = 183;
    at<short>(self, 152) = 255;
    at<short>(self, 154) = 100;
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
    __11LightEffectG6Point3ffffffffffffP10GameObjecti(block, &point, 110.0f, 183.0f, 255.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 120.0f, 60.0f, 0.5f, __builtin_bit_cast(float, 0x3f999999), nullptr, 0);
}
