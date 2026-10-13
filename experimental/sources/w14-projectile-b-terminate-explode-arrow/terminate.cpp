// Observed terminate__22ExplodeArrowProjectilei at 0x002a2388, 572 bytes. O is the projectile, h the integer argument and n the
// signed word at O+104; the word at O+96 is a done flag. Nothing happens when that word is non-zero. Otherwise, in order:
//  1. When h is non-zero: a roll r = eRand__10GameObject(O) gives the digit (r & 3) + 1. With levelFlags bit 0x40000 set the name
//     "impct_mtl_ice%d.anm" is built with sprintf and addEffect__FiG6Point3(8, copy of O+16) runs; otherwise
//     "impct_mtl_stone%d.anm" is built and effect 2 runs. soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
//     lumpFindResource__FPcT0("fx.lmp", name), O, 1.0) runs, then the same with "spel_we_explarrow_hit.anm".
//     makeExplosion__FG6Point3f(copy of O+16, max(1.0, float(n) * 0.1)) runs (0.1 is the single in the small-data area at
//     0x00482a0c, bits 0x3dcccccc; the max is the EE max.s). A LightEffect (188 bytes from blockAlloc__Fi, when the allocation
//     succeeded) is built from a copy of O+16, then the twelve singles 255, 10, 10, 0, 0, 0, 0, 0, 120, 60, 0.5 and 0.25, a null
//     object and the int 0. Last, gameSetCameraShake__Ffb(float(n / 2), 0) runs (the division rounds toward zero).
//  2. objectAddToDeleteList__FP10GameObject(O) runs and the word at O+96 becomes 1.
// Inferred: the blast at the end of an explosive arrow, with an impact sound by level kind, a flash and a camera shake scaled by the
// level number. Unknown: what the twelve light singles mean and what n is (it also scales the blast).
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern int levelFlags;
int libc_sprintf(char* buffer, const char* format, ...) __asm__("sprintf");
int eRand__10GameObject(void* self);
void addEffect__FiG6Point3(int effect, Point3 position);
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void makeExplosion__FG6Point3f(Point3 position, float scale);
void* blockAlloc__Fi(int size);
void LightEffect_construct(void* self, Point3 position, float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l,
                           void* object, int n) __asm__("__11LightEffectG6Point3ffffffffffffP10GameObjecti");
void gameSetCameraShake__Ffb(float amount, int skip);
void objectAddToDeleteList__FP10GameObject(void* object);

namespace {
static inline float fp_max(float a, float b) {
#if defined(__mips__)
    float out;
    __asm__("max.s %0, %1, %2" : "=f"(out) : "f"(a), "f"(b));
    return out;
#else
    return a > b ? a : b;
#endif
}
}  // namespace

void terminate__22ExplodeArrowProjectilei(void* self, int hit) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    if (hit != 0) {
        char name[80];
        if (levelFlags & 0x40000) {
            libc_sprintf(name, "impct_mtl_ice%d.anm", (eRand__10GameObject(o) & 3) + 1);
            addEffect__FiG6Point3(8, *reinterpret_cast<Point3*>(o + 16));
        } else {
            libc_sprintf(name, "impct_mtl_stone%d.anm", (eRand__10GameObject(o) & 3) + 1);
            addEffect__FiG6Point3(2, *reinterpret_cast<Point3*>(o + 16));
        }
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", name), o, 1.0f);
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_we_explarrow_hit.anm"), o, 1.0f);
        makeExplosion__FG6Point3f(*reinterpret_cast<Point3*>(o + 16), fp_max(1.0f, static_cast<float>(sword(o, 104)) * bits(0x3dccccccu)));
        void* const block = blockAlloc__Fi(188);
        if (block != nullptr)
            LightEffect_construct(block, *reinterpret_cast<Point3*>(o + 16), 255.0f, 10.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 120.0f, 60.0f, 0.5f, 0.25f,
                                  nullptr, 0);
        gameSetCameraShake__Ffb(static_cast<float>(sword(o, 104) / 2), 0);
    }
    objectAddToDeleteList__FP10GameObject(o);
    word(o, 96) = 1;
}
}
