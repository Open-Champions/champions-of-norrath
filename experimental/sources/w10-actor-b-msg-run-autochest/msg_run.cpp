#include "../w10-actor-b-common/fields.hpp"
// Observed msg_run__9AutoChest at 0x21f1a8, 1328 bytes. Fields used: single position at
// +16/+20/+24, halfword angle +56, model word +144, AnimationState +152, word +192
// (the result of the animation step), word +216 (open phase), word +224 and +232
// (frame stamps), signed byte +228 (a destruction flag), halfword +158.
// First, the word at +192 receives animAdvanceAnimation(+152, position, null, null).
// Then, by the signed byte at +228:
//  nonzero (the chest is being destroyed): nothing more happens until the signed word at
//   +232 is below frameNm. After that each call: the anchor is the position, or, when a model
//   word is set at +144, the model's character-attachment position for the position and
//   angle. Two particle bursts follow at the anchor: each draws a vector inside the unit
//   ball by rejection (components eRand * 2^-16 * 2 - 1, retried while the squared length
//   exceeds 1.0), flips a negative third component, and scales it (fuse particle: 5, 5, 25;
//   smoke particle: 1.28, 1.28, 1.28 * 24.72). Then, only when (float)(frameNm - word at
//   +224) exceeds 90.0: the explosion sound "spel_we_explarrow_hit.anm" from "fx.lmp",
//   makeExplosion at the position raised by 14.7 (scale 2.0), radial damage around the
//   position (base 15, 60 when levelFlags has 0x10000, 220 when it has 0x8000; times 3 when
//   gameDifficulty is 2 and times 6 when it is 3; radii 40 and 180, flags 0x40800), a floor
//   decal one unit higher with a random angle (extent 48, kind 9), a 188-byte light effect
//   at the position, and finally objectAddToDeleteList(object).
//  zero: unless the phase +216 is already 2, and only once the signed word at +232 is below
//   frameNm, +216 becomes 2, objectDoMyActions(object, 1) runs, then spawnItems. Then, when
//   bit 0x80 of the halfword at +158 is set, the object leaves the run list.
// Inferred: +228 marks a chest destroyed by an explosion, +216 = 2 means its items were spawned.
// Unknown: what the two frame stamps and the halfword at +158 mean; particle definitions are
// file-local to the original unit and are imported by placeholder names here.
extern "C" {
extern int frameNm;
extern int levelFlags;
extern int gameDifficulty;
extern unsigned char norrath_auto_chest_msg_run_fuseParticle_bytes[80];
extern unsigned char norrath_auto_chest_msg_run_smokeParticle_bytes[80];
int animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(void* state, void* point, void* other, short* extra);
void modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
    void* model, const float* point, short angle, void* state, int index, float* out, float* other, float scale);
unsigned eRand__10GameObject(void* self);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, float* position, float* velocity, float age);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* sound, void* object, float volume);
void makeExplosion__FG6Point3f(const float* position, float scale);
void objectRadialDamage__FP10GameObjectG6Point3iffiii(void* self, const float* position, int damage, float a, float b,
                                                      int c, int d, int e);
void floorAdd__FG6Point3sfii(const float* position, short angle, float extent, int kind, int extra);
void* blockAlloc__Fi(int bytes);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float p13, float p14, void* object, int flag);
void objectAddToDeleteList__FP10GameObject(void* object);
void objectDoMyActions__FP10GameObjecti(void* object, int on);
void spawnItems__9AutoChest(void* self);
void objectRemoveFromRunList__FP10GameObject(void* object);
}

namespace {
using namespace actor_b;

__attribute__((always_inline)) inline void copy_position(void* self, float* out) {
    out[0] = field<Single>(self, 16);
    out[1] = field<Single>(self, 20);
    out[2] = field<Single>(self, 24);
}

__attribute__((always_inline)) inline float unit_component(void* self) {
    float value = static_cast<float>(static_cast<int>(eRand__10GameObject(self)));
    value = value * rtti_c::single_from_bits(0x37800000u);
    value = value + value;
    return value - 1.0f;
}

// Redraws until the vector lies inside the unit ball (squared length 1.0 allowed).
__attribute__((always_inline)) inline void random_ball(void* self, float* out) {
    do {
        out[0] = unit_component(self);
        out[1] = unit_component(self);
        out[2] = unit_component(self);
    } while (1.0f < out[0] * out[0] + out[1] * out[1] + out[2] * out[2]);
}
}

extern "C" void msg_run__9AutoChest(void* self) {
    field<SWord>(self, 192) = animAdvanceAnimation__FP14AnimationStateP6Point3P6Point2Ps(
        static_cast<unsigned char*>(self) + 152, static_cast<unsigned char*>(self) + 16, nullptr, nullptr);
    if (field<SByte>(self, 228) != 0) {
        if (!(field<SWord>(self, 232) < frameNm)) return;
        float anchor[3];
        copy_position(self, anchor);
        if (field<Word>(self, 144) != 0) {
            float at[3];
            copy_position(self, at);
            modelGetCharAttachmentPos__FP12_modelHeaderG6Point3sP14AnimationStateiP6Point3T5f(
                link(self, 144), at, field<SHalf>(self, 56), nullptr, 0, anchor, nullptr, 1.0f);
        }
        float velocity[3];
        random_ball(self, velocity);
        if (velocity[2] < 0.0f) velocity[2] = -velocity[2];
        velocity[0] = velocity[0] * 5.0f;
        velocity[1] = velocity[1] * 5.0f;
        velocity[2] = velocity[2] * 5.0f;
        velocity[2] = velocity[2] * 5.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_auto_chest_msg_run_fuseParticle_bytes, anchor, velocity, 0.0f);
        random_ball(self, velocity);
        if (velocity[2] < 0.0f) velocity[2] = -velocity[2];
        velocity[0] = velocity[0] * 1.28f;
        velocity[1] = velocity[1] * 1.28f;
        velocity[2] = velocity[2] * 1.28f;
        velocity[2] = velocity[2] * 24.72f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(norrath_auto_chest_msg_run_smokeParticle_bytes, anchor, velocity, 0.0f);
        if (!(90.0f < static_cast<float>(frameNm - static_cast<int>(field<Word>(self, 224))))) return;
        float blast[3] = {field<Single>(self, 16), field<Single>(self, 20), 0.0f};
        blast[2] = field<Single>(self, 24) + 14.7f;
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
            lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("spel_we_explarrow_hit.anm")), self, 1.0f);
        makeExplosion__FG6Point3f(blast, 2.0f);
        int damage = 15;
        if ((levelFlags & 0x10000) != 0) damage = 60;
        if ((levelFlags & 0x8000) != 0) damage = 220;
        if (gameDifficulty == 2) damage = damage * 3;
        else if (gameDifficulty == 3) damage = damage * 6;
        float centre[3];
        copy_position(self, centre);
        objectRadialDamage__FP10GameObjectG6Point3iffiii(self, centre, damage, 40.0f, 180.0f, 0x40800, 0, 0);
        float decal[3] = {field<Single>(self, 16), field<Single>(self, 20), 0.0f};
        decal[2] = field<Single>(self, 24) + 1.0f;
        floorAdd__FG6Point3sfii(decal, static_cast<short>(eRand__10GameObject(self)), 48.0f, 9, 0);
        void* block = blockAlloc__Fi(188);
        if (block != nullptr) {
            float where[3];
            copy_position(self, where);
            __11LightEffectG6Point3ffffffffffffP10GameObjecti(block, where, 128.0f, 100.0f, 78.0f, -32.0f, 0.0f, 0.0f,
                                                              0.0f, 0.0f, 168.0f, 60.0f, 1.0f, 0.35f, nullptr, 0);
        }
        objectAddToDeleteList__FP10GameObject(self);
        return;
    }
    if (field<SWord>(self, 216) != 2 && field<SWord>(self, 232) < frameNm) {
        field<Word>(self, 216) = 2;
        objectDoMyActions__FP10GameObjecti(self, 1);
        spawnItems__9AutoChest(self);
    }
    if ((field<Half>(self, 158) & 0x80) != 0) objectRemoveFromRunList__FP10GameObject(self);
}
