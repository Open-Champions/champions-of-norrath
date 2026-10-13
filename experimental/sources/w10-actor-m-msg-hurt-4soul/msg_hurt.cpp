#include "../w10-actor-m-common/access.hpp"
// Observed msg_hurt__4SoulfP10GameObjecti at 0x1f4430, 660 bytes. Arguments: object, float amount, source
// object, int. Nothing is returned.
//  1. A null source calls __throw_bad_typeid (it does not return).
//  2. The function word at byte 4 of the source's table (word at source+72, slot 0) is called with the first
//     argument left as the receiving object, and its result is compared with __tf5Demon() through
//     __eq__C9type_infoRC9type_info. When the comparison returns zero the function ends here.
//  3. Otherwise msg_hurt__8CreaturefP10GameObjecti runs with the same four arguments, and then, when
//     modelGetNmShadowPoints__FP12_modelHeader of the model (table byte 128 of the object, getModel's slot)
//     returns a nonzero count n, six times: the model is fetched again, a roll from eRand__10GameObject(object)
//     modulo n (signed) picks a shadow point, modelGetShadowPos is called (animation state at +288, the 12 bytes
//     at +16 as the position, (short)(halfword at +56 - 16384), that index, an output Point3 and an output
//     float, no matrix, the float at +224), a velocity is drawn as in the Soul state code (three rolls mapped
//     to 2*r*2^-16 - 1, redrawn while the squared length exceeds 1.0, z forced to zero), each component is
//     multiplied by 15.0, and P_AddParticle__FP11ParticleDefP6Point3T1f(soulParticle, output point, velocity,
//     0.0f) runs.
// Inferred: the Soul only reacts to a source whose type is exactly Demon (by comparing type info), and the
// particles are the hit sparks drawn from the shadow points. Unknown: the int argument and what the point
// index and the float output mean.
extern "C" {
extern unsigned char soulParticle[];
__attribute__((noreturn)) void __throw_bad_typeid();
void* __tf5Demon();
int __eq__C9type_infoRC9type_info(const void* a, const void* b);
void msg_hurt__8CreaturefP10GameObjecti(void* self, float amount, void* source, int flag);
int modelGetNmShadowPoints__FP12_modelHeader(void* model);
void modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
    void* model, void* animation, Point3 position, short angle, int index, Point3* out_point, float* out_value, void* matrix, float scale);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, Point3* position, Point3* velocity, float value);
int eRand__10GameObject(void* object);
}

namespace {
inline void* model_of(unsigned char* o) { return w10m::from_word(w10m::call_entry(o, 128)); }
}

extern "C" void msg_hurt__4SoulfP10GameObjecti(void* self, float amount, void* source, int flag) {
    unsigned char* o = static_cast<unsigned char*>(self);
    unsigned char* s = static_cast<unsigned char*>(source);
    if (s == nullptr) __throw_bad_typeid();
    const unsigned type_word = w10m::call_entry_plain(s, 0, self);
    const void* source_type = reinterpret_cast<const void*>(static_cast<__UINTPTR_TYPE__>(type_word));
    if (__eq__C9type_infoRC9type_info(source_type, __tf5Demon()) == 0) return;
    msg_hurt__8CreaturefP10GameObjecti(self, amount, source, flag);
    const int count = modelGetNmShadowPoints__FP12_modelHeader(model_of(o));
    if (count == 0) return;
    int shots = 0;
    do {
        void* model = model_of(o);
        const int roll = eRand__10GameObject(self);
        Point3 point;
        float value;
        modelGetShadowPos__FP12_modelHeaderP14AnimationStateG6Point3siP6Point3PfP8Matrix33f(
            model, o + 288, *reinterpret_cast<Point3*>(o + 16),
            static_cast<short>(static_cast<unsigned>(static_cast<unsigned short>(w10m::half(o, 56))) - 16384u), roll % count, &point, &value, nullptr,
            w10m::real(o, 224));
        Point3 sample;
        w10m::unit_ball(o, sample, [](unsigned char* object) { return eRand__10GameObject(object); });
        Point3 velocity;
        velocity.x = sample.x;
        velocity.y = sample.y;
        velocity.z = 0.0f;
        velocity.x = velocity.x * 15.0f;
        velocity.y = velocity.y * 15.0f;
        velocity.z = velocity.z * 15.0f;
        P_AddParticle__FP11ParticleDefP6Point3T1f(soulParticle, &point, &velocity, 0.0f);
        ++shots;
    } while (shots < 6);
}
