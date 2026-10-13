// Observed attack__14SpellHarmTouchP10GameObjectPiT2 at 0x0029aa00, 944 bytes. this is the spell
// object, then the target object and two int result slots (damage, total). In the image's order:
//   1. Let f = min(1.0, float at this+24) * 128 (as an int). The record the word at this+12 points
//      to gets shorts 113 at +146 and f at +142 and +150. When the target's flag word at +12 does
//      not have bit 0x20 set the function ends here.
//   2. Otherwise 102 rounds of sparks: r = eRand(this); the target's vtable slot at (word at +72)
//      + 128 (a short adjustment, a function word) is called to get a model header, whose
//      modelGetNmShadowPoints count n gives index = r % n (signed remainder); the same slot is
//      called again and modelGetShadowPos(model, target+288, copy of target position, target heading
//      short at +56, index, out point, scratch floats, null, 1.0) gives a point. The direction is
//      (out.x - record.x(+16), out.y - record.y(+20), 0) normalised with 1/sqrt and scaled by
//      53.435 in x and y; eRand(this) is called once more and its value dropped; P_AddParticle(harm1,
//      out, direction with z 0, 0.0) and then P_AddParticle(harm2, out, direction scaled by 0.137 in
//      all three components, 0.0).
//   3. A 188-byte block from blockAlloc, when not null, becomes a LightEffect at (target x + 0, target
//      y, target z + 73) with parameters 83, 218, 73, 0, 0, 0, 0, 0, 216, 60, 0.5, 0.366, no object
//      and flag 0.
//   4. d = the call of this+8 object's vtable slot (word at +96) + 104 with (0, signed byte at
//      this+0, record at this+12); *damage = d. Then a float x = d * 0.2 * (eRand(this) / 65536);
//      *total = *damage + (int)x; the slot at (word at this+20) + 8 is called with (adjusted
//      this, 3).
// Inferred: a touch attack with a spark shower from random model shadow points and a flash. The
// constants 53.435, 0.137, 0.366 and 0.2 are unnamed read-only floats of the image (bits
// 0x4255bd8f, 0x3e0c49ba, 0x3ebb63b2, 0x3e4ccccc). Unknown: the meaning of the shorts at the record
// (+142, +146, +150), of the scratch floats of modelGetShadowPos, and of slot argument 3.
// Nothing here ran on the EE.
#ifndef NORRATH_HOST_BASE
#define NORRATH_HOST_BASE 0
#endif
extern "C" {
extern unsigned char harm1[];
extern unsigned char harm2[];
int eRand__5Spell(void* spell);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, const float* position, short heading, int index, float* out, float* scratch,
    void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, float* position, float* velocity, float age);
void* blockAlloc__Fi(int size);
void __11LightEffectG6Point3ffffffffffffP10GameObjecti(
    void* block, const float* point, float p3, float p4, float p5, float p6, float p7, float p8, float p9, float p10,
    float p11, float p12, float p13, float p14, void* object, int flag);
}

#ifdef NORRATH_HOST_CONTRACT
extern void* host_function_table[];
#endif
namespace {
inline float constant(unsigned bits) { return __builtin_bit_cast(float, bits); }
inline unsigned char* word_pointer(unsigned char* base, int offset) {
    return reinterpret_cast<unsigned char*>(static_cast<__UINTPTR_TYPE__>(*reinterpret_cast<unsigned*>(base + offset)) + NORRATH_HOST_BASE);
}
#ifdef NORRATH_HOST_CONTRACT
inline void* function_word(unsigned char* slot) { return host_function_table[*reinterpret_cast<unsigned*>(slot + 4)]; }
#else
inline void* function_word(unsigned char* slot) { return word_pointer(slot, 4); }
#endif
// A slot is a short adjustment at +0 and a function word at +4.
inline unsigned char* slot_adjusted(unsigned char* object, unsigned char* slot) {
    return object + *reinterpret_cast<short*>(slot);
}
typedef unsigned char* (*ModelFn)(unsigned char*);
typedef int (*IntFn)(unsigned char*, int, int, unsigned char*);
typedef void (*TouchFn)(unsigned char*, int);
unsigned char* target_model(unsigned char* target) {
    unsigned char* slot = word_pointer(target, 72) + 128;
    return reinterpret_cast<ModelFn>(function_word(slot))(slot_adjusted(target, slot));
}
}  // namespace

extern "C" void attack__14SpellHarmTouchP10GameObjectPiT2(unsigned char* self, unsigned char* target, int* damage, int* total) {
    float one = constant(0x3f800000u);
    float cap = *reinterpret_cast<float*>(self + 24);
    float limited = one < cap ? one : cap;
    unsigned char* record = word_pointer(self, 12);
    *reinterpret_cast<short*>(record + 146) = 113;
    int level = static_cast<int>(limited * 128.0f);
    *reinterpret_cast<short*>(record + 142) = static_cast<short>(level);
    *reinterpret_cast<short*>(record + 150) = static_cast<short>(level);
    if (!(*reinterpret_cast<unsigned*>(target + 12) & 0x20)) return;
    float speed = constant(0x4255bd8fu);
    float shrink = constant(0x3e0c49bau);
    float position[3];
    float out[3];
    float scratch[4];
    float direction[3];
    int round = 101;
    do {
        --round;
        int r = eRand__5Spell(self);
        int index = r % modelGetNmShadowPoints__FP12_modelHeader(target_model(target));
        unsigned char* model = target_model(target);
        const float* at = reinterpret_cast<float*>(target + 16);
        position[0] = at[0];
        position[1] = at[1];
        position[2] = at[2];
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, target + 288, position, *reinterpret_cast<short*>(target + 56), index, out, scratch, nullptr, one);
        unsigned char* from = word_pointer(self, 12);
        direction[0] = out[0] - *reinterpret_cast<float*>(from + 16);
        direction[2] = 0.0f;
        direction[1] = out[1] - *reinterpret_cast<float*>(from + 20);
        float inverse = one / __builtin_sqrtf(direction[0] * direction[0] + direction[1] * direction[1]);
        direction[0] = direction[0] * inverse;
        direction[1] = direction[1] * inverse;
        direction[2] = direction[2] * inverse;
        direction[0] = direction[0] * speed;
        direction[1] = direction[1] * speed;
        eRand__5Spell(self);
        direction[2] = 0.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(harm1, out, direction, 0.0f);
        direction[0] = direction[0] * shrink;
        direction[1] = direction[1] * shrink;
        direction[2] = direction[2] * shrink;
        P_AddParticle__FP11ParticleDefP6Point3T1f(harm2, out, direction, 0.0f);
    } while (round >= 0);
    const float* base = reinterpret_cast<float*>(target + 16);
    void* block = blockAlloc__Fi(188);
    if (block) {
        float flash[3] = {base[0] + 0.0f, base[1], base[2] + constant(0x42920000u)};
        __11LightEffectG6Point3ffffffffffffP10GameObjecti(
            block, flash, constant(0x42a60000u), constant(0x435a0000u), constant(0x42920000u), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            constant(0x43580000u), constant(0x42700000u), constant(0x3f000000u), constant(0x3ebb63b2u), nullptr, 0);
    }
    unsigned char* caster = word_pointer(self, 8);
    unsigned char* slot = word_pointer(caster, 96) + 104;
    int dealt = reinterpret_cast<IntFn>(function_word(slot))(slot_adjusted(caster, slot), 0, *reinterpret_cast<signed char*>(self), word_pointer(self, 12));
    float scaled = static_cast<float>(dealt) * constant(0x3e4ccccc);
    *damage = dealt;
    float roll = static_cast<float>(eRand__5Spell(self)) * constant(0x37800000u);
    unsigned char* notify = word_pointer(self, 20) + 8;
    void* function = function_word(notify);
    *total = *damage + static_cast<int>(scaled * roll);
    reinterpret_cast<TouchFn>(function)(slot_adjusted(self, notify), 3);
}
