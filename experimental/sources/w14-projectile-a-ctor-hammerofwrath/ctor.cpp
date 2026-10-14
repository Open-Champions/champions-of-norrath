// Observed __13HammerOfWrathG6Point3sP10GameObjectiP5Skill at 0x002b3700, 612 bytes. Arguments: the object, a position P (by value), a
// short A, a creature W (may be null), an integer N and a skill K. Returns nothing. In order:
//  1. The GameObject constructor runs with (object, P, null, null). The words +76 and +80 are cleared and _vt$13HammerOfWrath goes to
//     +72. An 80-byte item description at +88 is initialised: the first halfword of each of the four 8-byte entries at +124, +132,
//     +140 and +148 is -1, the bytes at +120 and +121 are 255 and the eight halfwords at +104..+110 and +112..+118 are 0.
//  2. The halfword +56 is A. When W is non-null, listOnewayLink(object+76, W, W) runs. The word +164 becomes N.
//  3. item::create is called with a name selected by N (0 "warhammer+frostburst10,10-20", 1 "...12,12-24", 2 "...14,14-26", 3
//     "...16,16-28", any other value "...18,18-30" with the same prefix "warhammer+frostburst"), the description at +88, -1 and 0.
//     The word +164 (read again) is then increased by 1 and bit 1 of the flag word at +12 is set.
//  4. objectAddToRunList runs. The resource "spel_he_hammer.anm" is looked up in "fx.lmp" and played with
//     soundPlaySequence(result, object, 1.0).
//  5. The single +180 is 16384.0, the position at +16 is copied to the three words at +216, the word +196 is K. The table at the word
//     K+96 is used: its entry at byte 88 is called on K (adjusted by the entry's signed halfword) with (1, the word +164) and the
//     result times 60 goes to the word +168. The byte wrathHammerCount increases by 1.
// Inferred: the name strings are item descriptions (the item::create callee) for a weapon with a frost effect; N picks a level.
// Unknown: the meaning of N and A beyond the stored form, of +180/+196/+216/+168, of the call at table byte 88 and of
// wrathHammerCount. Nothing here ran on the EE.
#include "../w14-projectile-a-common/access.hpp"

extern "C" {
extern unsigned char wrathHammerCount;
extern unsigned char vt_HammerOfWrath[] __asm__("_vt$13HammerOfWrath");
void game_object_construct(void* self, Point3 position, void* map, char** names) __asm__("__10GameObjectG6Point3PQ210GameObject11resourceMapPPc");
void listOnewayLink__FP8ListHeadT0Pv(void* head, void* other, void* item);
void create__4itemPcPQ24item11descriptionii(const char* name, void* description, int a, int b);
void objectAddToRunList__FP10GameObject(void* object);
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void hammer_of_wrath_construct(void* self, Point3 position, int a, void* creature, int n, void* skill) __asm__("__13HammerOfWrathG6Point3sP10GameObjectiP5Skill");
}

static char lump_name[] = "fx.lmp";
static char sound_name[] = "spel_he_hammer.anm";

// a is declared as an int so that the halfword sign extension the image performs on entry stays explicit.
extern "C" void hammer_of_wrath_construct(void* self, Point3 position, int a, void* creature, int n, void* skill) {
    using namespace w14pa;
    unsigned char* const o = static_cast<unsigned char*>(self);
    const short angle = static_cast<short>(a);
    game_object_construct(self, position, nullptr, nullptr);
    word(o, 76) = 0;
    word(o, 80) = 0;
    word(o, 72) = to_word(vt_HammerOfWrath);
    unsigned char* const description = o + 88;
    for (int entry = 3; entry >= 0; --entry) half(description, 36 + 8 * entry) = -1;
    description[33] = 0xff;
    description[32] = 0xff;
    for (int i = 0; i < 4; ++i) {
        half(description, 16 + 2 * i) = 0;
        half(description, 24 + 2 * i) = 0;
    }
    half(o, 56) = angle;
    if (creature != nullptr) listOnewayLink__FP8ListHeadT0Pv(o + 76, creature, creature);
    word(o, 164) = static_cast<unsigned>(n);
    const char* name;
    switch (n) {
    case 0: name = "warhammer+frostburst10,10-20"; break;
    case 1: name = "warhammer+frostburst12,12-24"; break;
    case 2: name = "warhammer+frostburst14,14-26"; break;
    case 3: name = "warhammer+frostburst16,16-28"; break;
    default: name = "warhammer+frostburst18,18-30"; break;
    }
    create__4itemPcPQ24item11descriptionii(name, description, -1, 0);
    word(o, 164) = word(o, 164) + 1u;
    word(o, 12) |= 1u;
    objectAddToRunList__FP10GameObject(self);
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(lumpFindResource__FPcT0(lump_name, sound_name), self, 1.0f);
    real(o, 180) = bits(0x46800000u);
    *reinterpret_cast<Point3*>(o + 216) = *reinterpret_cast<Point3*>(o + 16);
    word(o, 196) = to_word(skill);
    const unsigned result = call_entry_from(static_cast<unsigned char*>(skill), 96, 88, 1, word(o, 164));
    word(o, 168) = result * 60u;
    wrathHammerCount = static_cast<unsigned char>(wrathHammerCount + 1);
}
