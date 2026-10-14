// Observed terminate__19MultiFireProjectilei at 0x002a1a10, 288 bytes. O is the projectile and h the integer argument; the word at
// O+96 is a done flag and nothing happens when it is non-zero. Otherwise, in order:
//  1. P_DeleteTrail__Fi(the word at O+116) runs.
//  2. When h is non-zero: a roll r = eRand__10GameObject(O) gives the digit (r & 3) + 1. With levelFlags bit 0x40000 set the name
//     "impct_mtl_ice%d.anm" is built with sprintf and addEffect__FiG6Point3(8, copy of O+16) runs; otherwise
//     "impct_mtl_stone%d.anm" is built and effect 2 runs. Then soundPlaySequence__FP15AnimationHeaderP10GameObjectf(
//     lumpFindResource__FPcT0("fx.lmp", name), O, 1.0) runs.
//  3. objectAddToDeleteList__FP10GameObject(O) runs and the word at O+96 becomes 1.
// Inferred: the end of a multi-fire shot, with an icy or stony impact effect by level kind. Unknown: the meaning of h beyond zero
// versus non-zero and of the word at +116 beyond its use as a trail handle.
// Nothing here ran on the EE.
#include "../w13-spell-msg-b-common/access.hpp"

extern "C" {
extern int levelFlags;
int libc_sprintf(char* buffer, const char* format, ...) __asm__("sprintf");
int eRand__10GameObject(void* self);
void P_DeleteTrail__Fi(int handle);
void addEffect__FiG6Point3(int effect, Point3 position);
void* lumpFindResource__FPcT0(const char* lump, const char* resource);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void objectAddToDeleteList__FP10GameObject(void* object);

void terminate__19MultiFireProjectilei(void* self, int hit) {
    using namespace w13smb;
    unsigned char* const o = static_cast<unsigned char*>(self);
    if (word(o, 96) != 0u) return;
    P_DeleteTrail__Fi(static_cast<int>(word(o, 116)));
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
    }
    objectAddToDeleteList__FP10GameObject(o);
    word(o, 96) = 1;
}
}
