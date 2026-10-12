#pragma once
#include "../w8-actor-virtuals-c-bytes/bytes.hpp"
// Shared body of msg_run__8HateInto (0x225910, 472 bytes) and msg_run__8HateGate
// (0x225ae8, 472 bytes). The two images differ only in three constants: the frame
// mask and index step (1 and 2 for HateInto, 7 and 8 for HateGate) and the
// particle definition they add. Neither image calls through a shared routine;
// this header is a source-level convenience.
//
// Observed sequence: msg_run__8AutoProp(object); n = modelGetNmShadowPoints(model
// at +144); then for i = frameNm & mask, while i < n (signed), stepping by step:
//   modelGetShadowPos(model, object+152, copy of the position at +16, signed
//     halfword at +56, i, &shadow, &scratch, null, 1.0) fills the Point3 `shadow`;
//   three eRand(object) results are each converted to float, multiplied by 2^-16,
//   reduced by 0.5 and doubled, giving a velocity-like offset (x, y, z) that is
//   added to `shadow`; P_AddParticle(definition, &shadow, {0, 0, 0}, 0.0) is
//   called. The zero point is rebuilt for every pass. The index passed to
//   modelGetShadowPos is the value before the step.
// Inferred: the callee names come from the mangled symbols of the image; the
// random routine is the already defined norrath_actor_random (eRand).
// Unknown: what the shadow points are, why the mask and step differ, and the
// layout of the particle definitions (their storage is owned elsewhere).
extern "C" void msg_run__8AutoProp(void* self);
extern "C" int modelGetNmShadowPoints__FP12_modelHeader(void* model);
extern "C" int modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* state, const void* point, short angle, int index, void* out, float* scratch,
    void* matrix, float scale);
extern "C" unsigned norrath_actor_random(void* object);
extern "C" void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float age);
extern "C" int frameNm;

namespace w8c {
__attribute__((always_inline)) inline float random_offset(void* object) {
    float value = static_cast<float>(signed_word(norrath_actor_random(object)));
    value = value * single_from_bits(0x37800000u);
    value = value - single_from_bits(0x3f000000u);
    return value + value;
}

__attribute__((always_inline)) inline void hate_emit_particles(void* self, unsigned mask, int step, void* definition) {
    const Address base = address(self);
    void* const model = pointer(read_word(base + 144u));
    msg_run__8AutoProp(self);
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model);
    int index = frameNm & static_cast<int>(mask);
    while (index < count) {
        float zero[3] = {0.0f, 0.0f, 0.0f};
        float shadow[3];
        float scratch;
        float copy[3] = {read_single(base + 16u), read_single(base + 20u), read_single(base + 24u)};
        const int used = index;
        index += step;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, static_cast<unsigned char*>(self) + 152, copy, static_cast<short>(read_i16(base + 56u)),
            used, shadow, &scratch, nullptr, 1.0f);
        const float x = random_offset(self);
        const float y = random_offset(self);
        const float z = random_offset(self);
        shadow[0] = shadow[0] + x;
        shadow[1] = shadow[1] + y;
        shadow[2] = shadow[2] + z;
        P_AddParticle__FP11ParticleDefP6Point3T1f(definition, shadow, zero, 0.0f);
    }
}
}
