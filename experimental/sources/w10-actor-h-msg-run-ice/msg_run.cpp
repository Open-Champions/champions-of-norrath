// Observed msg_run__3Ice at 0x001df800, 884 bytes.
// o is the object. Runs once, then deletes the object.
//  - scriptGetGameState__FPc("iceSheetBroken") is called; a zero result returns at once (the object
//    is not deleted).
//  - lumpFindResource__FPcT0(word at o+76, word at o+88) gives a resource r. For k = 0, 2, 4, ...
//    while k < the signed byte at r+18:
//      * blockAlloc__Fi(592) gives a prop p. When it is non-null, the PhProp constructor
//        (__6PhPropG6Point3PcN22liPQ24item11descriptionf) runs on it with a copy of the 12 bytes at
//        o+16, the words at o+76, o+88 and o+92, the value 1 << k, 1556, a null description and the
//        float 1.0; then p+72 gets the PhIce table pointer (_vt$5PhIce) and the words at p+588 and
//        p+584 are cleared.
//      * When p is non-null: p+564 becomes 20; three eRand__10GameObject draws (each scaled by 2^-16)
//        give p+252 = (d1 - 0.5) * 5.0, p+256 = -100.0 - d2 * 200.0 and p+48 = d3 * 200.0 + 100.0;
//        then initStuff__6PhPropi(p, 1556) runs.
//      * One particle: a direction is drawn by rejection (each component (float)eRand * 2^-16,
//        doubled, minus 1.0; redraw while 1.0 < x*x + y*y + z*z), normalised with rsqrt.s, and
//        scaled by (eRand * 2^-16) * 10.0 * 35.0 (y first, then x) and x also by the float constant
//        0x3ba3d70a; z becomes 0.0 and y is forced to be not positive (negated when above 0.0).
//        P_AddParticle__FP11ParticleDefP6Point3T1f(vaporParticle, p + 16, direction, 0.0) runs. This
//        happens even when p is null, with the pointer value 16.
//  - Finally the object is deleted through its table: the halfword at table+8 is added to the
//    object, the function word at table+12 is called with that pointer and 3.
// Inferred: this breaks an ice sheet into physics props with particle puffs (names only).
// Unknown: the meaning of the object's words at +76, +88, +92 and of the PhProp fields written.
// Nothing here ran on the EE.
#include "../w10-actor-h-common/words.hpp"

extern "C" {
int scriptGetGameState__FPc(char* name);
void* lumpFindResource__FPcT0(char* lump, char* name);
void* blockAlloc__Fi(int size);
void phprop_construct(void* prop, const void* position, char* a, char* b, char* c, long flags, int d, void* description,
                      float scale) __asm__("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
void initStuff__6PhPropi(void* prop, int value);
int eRand__10GameObject(void* object);
void P_AddParticle__FP11ParticleDefP6Point3T1f(void* definition, void* position, void* velocity, float extra);
extern unsigned char vt_PhIce[] __asm__("_vt$5PhIce");
extern unsigned char vaporParticle[128];
}

namespace {
float reciprocal_sqrt(float length_squared) {
#if defined(__mips__)
    float result;
    float one = 1.0f;
    __asm__ volatile("rsqrt.s %0, %1, %2" : "=f"(result) : "f"(one), "f"(length_squared));
    return result;
#else
    return 1.0f / __builtin_sqrtf(length_squared);
#endif
}

float random_component(void* object) {
    float value = static_cast<float>(eRand__10GameObject(object)) * 0x1.0p-16f;
    value = value + value;
    return value - 1.0f;
}

unsigned low32(const void* pointer) { return static_cast<unsigned>(reinterpret_cast<__UINTPTR_TYPE__>(pointer)); }
}  // namespace

extern "C" void msg_run__3Ice(void* self) {
    unsigned char* object = static_cast<unsigned char*>(self);
    if (scriptGetGameState__FPc(const_cast<char*>("iceSheetBroken")) == 0) return;
    unsigned char* resource = static_cast<unsigned char*>(lumpFindResource__FPcT0(
        reinterpret_cast<char*>(w10h::from_word(w10h::word_at(self, 76))), reinterpret_cast<char*>(w10h::from_word(w10h::word_at(self, 88)))));
    for (int k = 0; k < static_cast<signed char>(resource[18]); k += 2) {
        unsigned char* prop = static_cast<unsigned char*>(blockAlloc__Fi(592));
        if (prop != nullptr) {
            unsigned char point[12];
            w10h::copy_bytes(point, object + 16, 12);
            phprop_construct(prop, point, reinterpret_cast<char*>(w10h::from_word(w10h::word_at(self, 76))),
                             reinterpret_cast<char*>(w10h::from_word(w10h::word_at(self, 88))),
                             reinterpret_cast<char*>(w10h::from_word(w10h::word_at(self, 92))), 1L << k, 1556, nullptr, 1.0f);
            w10h::word_at(prop, 72) = low32(vt_PhIce);
            w10h::word_at(prop, 588) = 0;
            w10h::word_at(prop, 584) = 0;
        }
        if (prop != nullptr) {
            w10h::word_at(prop, 564) = 20;
            float d1 = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
            *reinterpret_cast<float*>(prop + 252) = (d1 - 0.5f) * 5.0f;
            float d2 = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
            *reinterpret_cast<float*>(prop + 256) = -100.0f - d2 * 200.0f;
            float d3 = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
            *reinterpret_cast<float*>(prop + 48) = d3 * 200.0f + 100.0f;
            initStuff__6PhPropi(prop, 1556);
        }
        float v[3];
        float sum;
        do {
            v[0] = random_component(self);
            v[1] = random_component(self);
            v[2] = random_component(self);
            sum = v[0] * v[0] + v[1] * v[1];
            sum = sum + v[2] * v[2];
        } while (1.0f < sum);
        const float length_squared = (v[0] * v[0] + v[1] * v[1]) + v[2] * v[2];
        const float scale = reciprocal_sqrt(length_squared);
        v[2] = v[2] * scale;
        v[0] = v[0] * scale;
        v[1] = v[1] * scale;
        float speed = static_cast<float>(eRand__10GameObject(self)) * 0x1.0p-16f;
        speed = speed * 10.0f;
        speed = speed * 35.0f;
        float y = v[1] * speed;
        float x = v[0] * speed;
        x = x * 0x1.47ae14p-8f;
        v[2] = 0.0f;
        v[0] = x;
        v[1] = y;
        if (0.0f < y) v[1] = -y;
        void* position = reinterpret_cast<void*>(reinterpret_cast<__UINTPTR_TYPE__>(prop) + 16);
        P_AddParticle__FP11ParticleDefP6Point3T1f(vaporParticle, position, v, 0.0f);
    }
    if (self != nullptr) {
        typedef void (*Deleter)(void*, int);
        w10h::fn_from_word<Deleter>(w10h::entry_function(self, 8))(w10h::adjusted(self, 8), 3);
    }
}
