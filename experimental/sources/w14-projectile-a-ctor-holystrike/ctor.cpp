// Observed __10HolyStrikesiG6Point3P10GameObjectP5Skill at 0x002b1460, 504 bytes. The class name is HolyStrike (ten letters; the
// table is _vt$10HolyStrike) and the arguments are: the object, a short A, an integer B, a position P (by value), an owner object W
// (may be null) and a skill K. Returns nothing. In order:
//  1. The Projectile constructor runs with (object, P, W, null). The word +84 and the ListHead word +88 are cleared, the halfword +56
//     becomes A and _vt$10HolyStrike goes to +72. When W is non-null, listOnewayLink(object+84, W, W) runs.
//  2. With F = frameNm: +100 = F + 15, +96 = F + 180, the halfword +92 = B and +104 = K. icos__Fi(A) * 450.0 goes to the single +108
//     and isin__Fi(A) * 450.0 to the single +112 (A is re-read from the halfword at +56 for each call); the words +116 and +120 are 0.
//  3. P_StartTrail(1, 250, the single 0x3e4ccccc, 0, 250, 200, 100, 128) returns an id stored in +124 and P_ExtendTrail(id, the
//     position at +16) runs.
//  4. objectGetSortedCreatureList(the position at +16, A, 360.0, 75.0, the ListHead at object+88, C) runs where C is W when W is
//     non-null and bit 0x20 of the flag word at W+12 is set, else null. objectAddToRunList runs on the object.
//  5. The resource "spel_he_hstrike.anm" is looked up in "fx.lmp" and played with soundPlaySequence(result, object, 1.0).
// Inferred: icos/isin of the angle give a heading vector scaled by 450.0; the list at +88 collects creatures in a 360-degree cone of
// reach 75.0. Unknown: what A, B and the words at +96 and +100 mean beyond the stored form, and the meaning of the trail numbers.
// Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern int frameNm;
extern unsigned char vt_HolyStrike[] __asm__("_vt$10HolyStrike");
void projectile_construct(void* self, Point3 position, void* owner, void* map) __asm__("__10ProjectileG6Point3P10GameObjectPQ210GameObject11resourceMap");
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item);
float icos__Fi(int angle);
float isin__Fi(int angle);
int P_StartTrail__Fiifiiiii(int a, int b, float c, int d, int e, int f, int g, int h);
void P_ExtendTrail__FiG6Point3(int trail, Point3 position);
void objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(Point3 position, short angle, float a, float b, void* list, void* creature);
void objectAddToRunList__FP10GameObject(void* object);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void holy_strike_construct(void* self, int a, int b, Point3 position, void* owner, void* skill) __asm__("__10HolyStrikesiG6Point3P10GameObjectP5Skill");
}

static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_he_hstrike.anm";

// a is declared as an int so that the halfword sign extension the image performs on entry stays explicit.
extern "C" void holy_strike_construct(void* self, int a, int b, Point3 position, void* owner, void* skill) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const short angle = static_cast<short>(a);
    projectile_construct(self, position, owner, nullptr);
    word(o, 84) = 0;
    half(o, 56) = angle;
    word(o, 72) = to_word(vt_HolyStrike);
    word(o, 88) = 0;
    if (owner != nullptr) listOnewayLink__FP8ListHeadT0Pv(o + 84, owner, owner);
    word(o, 100) = static_cast<unsigned>(frameNm) + 15u;
    word(o, 96) = static_cast<unsigned>(frameNm) + 180u;
    half(o, 92) = static_cast<short>(b);
    const float reach = bits(0x43e10000u);
    word(o, 104) = to_word(skill);
    real(o, 108) = icos__Fi(half(o, 56)) * reach;
    real(o, 112) = isin__Fi(half(o, 56)) * reach;
    word(o, 116) = 0;
    word(o, 120) = 0;
    const int trail = P_StartTrail__Fiifiiiii(1, 250, bits(0x3e4ccccc), 0, 250, 200, 100, 128);
    word(o, 124) = static_cast<unsigned>(trail);
    P_ExtendTrail__FiG6Point3(trail, *reinterpret_cast<Point3*>(o + 16));
    void* creature = owner;
    if (owner == nullptr || (word(static_cast<unsigned char*>(owner), 12) & 0x20u) == 0) creature = nullptr;
    objectGetSortedCreatureList__FG6Point3sffP8ListHeadP8Creature(*reinterpret_cast<Point3*>(o + 16), half(o, 56), 360.0f, 75.0f, o + 88, creature);
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, sound_name), self, 1.0f);
}
