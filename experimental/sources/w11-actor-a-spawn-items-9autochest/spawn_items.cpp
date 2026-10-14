// Observed spawnItems__9AutoChest at 0x0021f898, 2528 bytes. Argument: the object o (a chest). Nothing is
// returned. Tags are read with the object's findTagString / findTagInt / findTagFloat using the strings "item",
// "potions", "random" and "arrows" (which the image keeps in its data; here they are literals). Every drop
// below starts from an item description D, a stack record cleared the same way each time: the halfwords at
// +36, +44, +52 and +60 are -1, the bytes at +32 and +33 are -1, and the halfwords at +16..+22 and +24..+30 are
// zero. "Spawn(D)" is the shared step, in order:
//   a 584-byte block is allocated and, when it succeeds, the PhProp constructor (three-name form) runs on it
//   with the point (x + 0.0, y, z + 12.0) of o's position at +16, the lump name "equipmnt.lmp" (the image's
//   string at 0x00470de8), the word at item::globalItem[getProxyMeshId(D)] + 0 and the word at
//   globalItem[getProxyMeshId(D)] + 4 (globalItem entries are 16 bytes; getProxyMeshId is called twice, one
//   call per word), the long 1, the int 1089, D and 1.0. Then one eRand draw r gives the angle
//   heading(o, halfword at +56) + (r & 0x3fff) - 8192; the block's floats at +252 and +256 become icos(angle)
//   * 24.0 and isin(angle) * 24.0, and initStuff(block, 1089) runs.
//  1. For i = 0, 1, ... while findTagString(o, "item", i) is non-null: clear D, create(that string, D, -1, 0),
//     Spawn(D).
//  2. For j = 0 while j < findTagInt(o, "potions", 0) (read again each time): clear D, makeRandomPotion(D),
//     Spawn(D). "potions found" is set once the body runs.
//  3. For k = 0, 1, ... while findTagString(o, "random", k) is non-null ("random found" is set): clear D;
//     f = findTagFloat(o, "random", k) * gameItemTreasureRamp(); then one eRand draw gives
//     g = (((float)draw * 2^-16) * 0.1 + 0.95) * 300.0 and f *= g; f is doubled when levelFlags has bit
//     0x10000 and doubled again when it has 0x8000; a second eRand draw mod 3 selects the category
//     {0, 1, 2}[draw mod 3]; when createRandom(D, (int)f, category, -1, 1) returns nonzero, Spawn(D).
//  4. When "random found" is not set and the word at +236 is zero, dropTreasure(pos, heading, 36.0, 2.0, 0.0,
//     0.0, a, b, null, 0) is called three times with (a, b) = (2.5, 0.75), (3.5, 0.02) and (4.5, 0.005).
//  5. When "potions found" is not set and the word at +236 is zero: clear D and draw eRand (signed). Below 5000:
//     create("arrows", D, -1, 0) and Spawn(D); below 20000: makeRandomPotion(D) and Spawn(D); otherwise nothing.
// The constants 0.1, 0.95, 0.02 and 0.005 are read from unnamed words through the global pointer (0x004816f4,
// 0x004816f8, 0x004816fc, 0x00481700). Inferred: the tags are chest content lists (items, potions, random
// loot) from the strings; Unknown: what the word at +236, the tag scale and the category numbers mean.
// Nothing here ran on the EE.
#include "../w11-actor-a-common/access.hpp"

extern "C" {
extern unsigned char globalItem[] __asm__("_4item$globalItem");
extern int levelFlags;
char* findTagString__10GameObjectPci(void* self, const char* tag, int index);
int findTagInt__10GameObjectPci(void* self, const char* tag, int index);
float findTagFloat__10GameObjectPci(void* self, const char* tag, int index);
unsigned eRand__10GameObject(void* self);
void* blockAlloc__Fi(int size);
float icos__Fi(int angle);
float isin__Fi(int angle);
int getProxyMeshId__4itemPQ24item11description(void* description);
void __6PhPropG6Point3PcN22liPQ24item11descriptionf(void* self, Point3 position, char* a, char* b, char* c, long n, int flags,
                                                    void* description, float f);
void initStuff__6PhPropi(void* prop, int flags);
void create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
void makeRandomPotion__4itemPQ24item11description(void* description);
int createRandom__4itemPQ24item11descriptioniiii(void* description, int amount, int category, int a, int b);
float gameItemTreasureRamp__Fv();
void dropTreasure__4itemG6Point3iffffffPci(Point3 position, int heading, float a, float b, float c, float d, float e, float f, char* name, int n);
}

namespace {
void clear_description(unsigned char* d) {
    using namespace w11a;
    for (int k = 0; k < 4; ++k) half(d, 60 - 8 * k) = -1;
    d[33] = 0xff;
    d[32] = 0xff;
    for (int k = 0; k < 4; ++k) {
        half(d, 16 + 2 * k) = 0;
        half(d, 24 + 2 * k) = 0;
    }
}

void spawn(unsigned char* o, unsigned char* description) {
    using namespace w11a;
    void* const block = blockAlloc__Fi(584);
    if (block == nullptr) return;
    Point3 offset = {0.0f, 0.0f, 12.0f};
    Point3 point;
    point.x = real(o, 16) + offset.x;
    point.y = real(o, 20);
    point.z = real(o, 24) + offset.z;
    char* const first = reinterpret_cast<char*>(from_word(word(globalItem + 16u * static_cast<unsigned>(getProxyMeshId__4itemPQ24item11description(description)), 0)));
    char* const second = reinterpret_cast<char*>(from_word(word(globalItem + 16u * static_cast<unsigned>(getProxyMeshId__4itemPQ24item11description(description)), 4)));
    __6PhPropG6Point3PcN22liPQ24item11descriptionf(block, point, const_cast<char*>("equipmnt.lmp"), first, second, 1, 1089, description, 1.0f);
    const unsigned draw = eRand__10GameObject(o);
    const int angle = half(o, 56) + static_cast<int>(draw & 0x3fffu) - 8192;
    Point3 direction;
    direction.x = icos__Fi(angle);
    direction.y = isin__Fi(angle);
    direction.z = 0.0f;
    unsigned char* const b = static_cast<unsigned char*>(block);
    real(b, 252) = direction.x * bits(0x41c00000u);
    real(b, 256) = direction.y * bits(0x41c00000u);
    initStuff__6PhPropi(block, 1089);
}
}  // namespace

extern "C" void spawnItems__9AutoChest(void* self) {
    using namespace w11a;
    unsigned char* const o = static_cast<unsigned char*>(self);
    unsigned char description[80];
    for (int i = 0; findTagString__10GameObjectPci(self, "item", i) != nullptr; ++i) {
        clear_description(description);
        create__4itemPcPQ24item11descriptionii(findTagString__10GameObjectPci(self, "item", i), description, -1, 0);
        spawn(o, description);
    }
    bool potionsFound = false;
    for (int j = 0; j < findTagInt__10GameObjectPci(self, "potions", 0); ++j) {
        potionsFound = true;
        clear_description(description);
        makeRandomPotion__4itemPQ24item11description(description);
        spawn(o, description);
    }
    bool randomFound = false;
    for (int k = 0; findTagString__10GameObjectPci(self, "random", k) != nullptr; ++k) {
        randomFound = true;
        clear_description(description);
        float amount = findTagFloat__10GameObjectPci(self, "random", k);
        amount = amount * gameItemTreasureRamp__Fv();
        float scale = static_cast<float>(static_cast<int>(eRand__10GameObject(self))) * bits(0x37800000u);
        scale = scale * bits(0x3dcccccc);
        scale = scale + bits(0x3f733333u);
        scale = scale * bits(0x43960000u);
        amount = amount * scale;
        if (levelFlags & 0x10000) amount = amount + amount;
        if (levelFlags & 0x8000) amount = amount + amount;
        static const int categories[3] = {0, 1, 2};
        const int category = categories[eRand__10GameObject(self) % 3u];
        if (createRandom__4itemPQ24item11descriptioniiii(description, static_cast<int>(amount), category, -1, 1) != 0) spawn(o, description);
    }
    if (!randomFound && word(o, 236) == 0) {
        dropTreasure__4itemG6Point3iffffffPci(*reinterpret_cast<Point3*>(o + 16), half(o, 56), bits(0x42100000u), 2.0f, 0.0f, 0.0f, bits(0x40200000u), bits(0x3f400000u), nullptr, 0);
        dropTreasure__4itemG6Point3iffffffPci(*reinterpret_cast<Point3*>(o + 16), half(o, 56), bits(0x42100000u), 2.0f, 0.0f, 0.0f, bits(0x40600000u), bits(0x3ca3d70au), nullptr, 0);
        dropTreasure__4itemG6Point3iffffffPci(*reinterpret_cast<Point3*>(o + 16), half(o, 56), bits(0x42100000u), 2.0f, 0.0f, 0.0f, bits(0x40900000u), bits(0x3ba3d70au), nullptr, 0);
    }
    if (!potionsFound && word(o, 236) == 0) {
        clear_description(description);
        const int draw = static_cast<int>(eRand__10GameObject(self));
        bool made = false;
        if (draw < 5000) {
            create__4itemPcPQ24item11descriptionii(const_cast<char*>("arrows"), description, -1, 0);
            made = true;
        } else if (draw < 20000) {
            makeRandomPotion__4itemPQ24item11description(description);
            made = true;
        }
        if (made) spawn(o, description);
    }
}
