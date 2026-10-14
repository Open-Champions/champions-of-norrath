// Observed __18DarknessProjectileG6Point3T1P5SkilliP10GameObject at 0x00297790, 544 bytes. Arguments: the object, a position P and a
// velocity V (both Point3 by value), a skill K, an integer N and an owner object W (may be null). Returns nothing. In order:
//  1. The GameObject constructor runs on the object with P, a null resource map and a null fourth argument. _vt$18DarknessProjectile
//     goes to +72 and P is copied to the three words at +136.
//  2. A light record at +88 is filled in place: bytes +8 and +9 are 0, halfwords +10, +12, +14, +18, +20 and +22 are 0xff82 (-126),
//     halfwords +16 and +24 are 0, the singles at +28 and +36 are 168.0, the singles at +32 and +40 are the single 0x38149b92, the
//     word +44 is 0. lightAdd is called with the address object+88.
//  3. Word +164 and the ListHead word at +168 are 0, the flag word at +12 gets 0x4001 set, V times the single 0x3c888888 goes to the
//     three singles at +76, the words +148 = 108, +172 = K, +152 = N, +156 = 1 and the byte +160 = 0 are stored.
//  4. The resource "spel_de_engulf.anm" is looked up in "fx.lmp" and played with soundPlaySequence(result, W, 1.0).
//  5. With W non-null listOnewayLink(object+168, W, W) runs. lightGetAmbient fills three halfwords; the halfwords at +98, +100 and
//     +102 become 1 minus the corresponding unsigned halfword. objectAddToRunList runs on the object.
// Inferred: the 0x3c888888 factor is 1/60 (per-frame step) from its value. Unknown: the meaning of the fields set in 3, the role of
// the light record and what the ambient-derived halfwords at +98..+102 are used for. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern unsigned char vt_DarknessProjectile[] __asm__("_vt$18DarknessProjectile");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void lightAdd__FP5Light(void* light);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item);
void lightGetAmbient__FPs(short* out);
void objectAddToRunList__FP10GameObject(void* object);
void __18DarknessProjectileG6Point3T1P5SkilliP10GameObject(void* self, Point3 position, Point3 velocity, void* skill, int n, void* owner);
}

static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_de_engulf.anm";

extern "C" void __18DarknessProjectileG6Point3T1P5SkilliP10GameObject(void* self, Point3 position, Point3 velocity, void* skill, int n, void* owner) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    game_object_construct(self, position, nullptr, nullptr);
    const Point3 here = position;
    unsigned char* const light = o + 88;
    word(o, 72) = to_word(vt_DarknessProjectile);
    light[9] = 0;
    *reinterpret_cast<Point3*>(o + 136) = here;
    real(light, 36) = bits(0x43280000u);
    real(light, 40) = bits(0x38149b92u);
    half(light, 22) = -126;
    real(light, 28) = bits(0x43280000u);
    real(light, 32) = bits(0x38149b92u);
    half(light, 10) = -126;
    half(light, 12) = -126;
    half(light, 14) = -126;
    half(light, 18) = -126;
    half(light, 20) = -126;
    word(light, 44) = 0;
    half(light, 16) = 0;
    half(light, 24) = 0;
    light[8] = 0;
    lightAdd__FP5Light(light);
    word(o, 164) = 0;
    const float step = bits(0x3c888888u);
    Point3 scaled;
    scaled.x = velocity.x * step;
    scaled.y = velocity.y * step;
    scaled.z = velocity.z * step;
    word(o, 168) = 0;
    word(o, 12) |= 0x4001u;
    *reinterpret_cast<Point3*>(o + 76) = scaled;
    word(o, 148) = 108;
    word(o, 172) = to_word(skill);
    word(o, 152) = static_cast<unsigned>(n);
    o[160] = 0;
    word(o, 156) = 1;
    void* const header = lumpFindResource__FPcT0(lump_name, sound_name);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, owner, 1.0f);
    if (owner != nullptr) listOnewayLink__FP8ListHeadT0Pv(o + 168, owner, owner);
    short ambient[4];
    lightGetAmbient__FPs(ambient);
    half(o, 98) = static_cast<short>(1 - static_cast<unsigned short>(ambient[0]));
    half(o, 100) = static_cast<short>(1 - static_cast<unsigned short>(ambient[1]));
    half(o, 102) = static_cast<short>(1 - static_cast<unsigned short>(ambient[2]));
    objectAddToRunList__FP10GameObject(self);
}
