// Observed terminate__19FireArrowProjectilei at 0x0029e5e0, 196 bytes. O is the projectile and h the integer argument; the
// word at O+96 is a done flag. Nothing happens when that word is non-zero. Otherwise, in order:
//  1. When h is 1, a StaticFire (1192 bytes from blockAlloc__Fi) is constructed, when the allocation succeeded, with a copy of the
//     three floats at O+16, 300.0, the int 1020 and 0.5.
//  2. When h is non-zero, soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp",
//     "spel_we_firearrow_hit.anm"), O, 1.0) runs.
//  3. objectAddToDeleteList__FP10GameObject(O) runs and the word at O+96 becomes 1.
// Inferred: the end of a fire arrow's flight, where only a hit of kind 1 leaves a fire behind. Unknown: the meaning of other values
// of h (the collision handler passes 2) and of the word at +96 beyond its use as a done flag.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
void* blockAlloc__Fi(int size);
void StaticFire_construct(void* self, Point3 where, float a, int b, float c) __asm__("__10StaticFireG6Point3fif");
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectAddToDeleteList__FP10GameObject(void* object);

void terminate__19FireArrowProjectilei(void* self, int hit) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    if (hit == 1) {
        void* block = blockAlloc__Fi(1192);
        if (block != nullptr) StaticFire_construct(block, *reinterpret_cast<Point3*>(o + 16), 300.0f, 1020, 0.5f);
    }
    if (hit != 0)
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0("fx.lmp", "spel_we_firearrow_hit.anm"), o, 1.0f);
    objectAddToDeleteList__FP10GameObject(o);
    word(o, 96) = 1;
}
}
