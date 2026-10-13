// Observed msg_poke__5Chesti at 0x00218eb8, 1916 bytes.
// Let o be the object, m the pointer stored at o+36 and n the int argument.
// If the word at o+132 is zero (the chest is closed):
//  - lumpFindResource("fx.lmp", "door_wdchst_open.anm") is played with soundPlaySequence(header, o,
//    1.0); animAdd(AnimationState at o+92, word m+28, 516); word +132 = 1; objectAddToRunList(o);
//    word +80 = frameNm; if the signed halfword +76 is not positive, bit 0x100000 of word +12 is
//    cleared;
//  - then three loops spawn dropped items, each item being an ItemDescription D (initialised the
//    way the image does: halfwords at +36, +44, +52 and +60 are -1, bytes +32 and +33 are 0xff,
//    and halfwords +16..+30 are 0), a 584-byte PhProp built at (x, y, z + 12.0) of the object, and
//    a launch: with r = eRand, the angle is halfword +56 + (r & 0x3fff) - 8192, the PhProp floats
//    at +252 and +256 are icos(angle) * 24 and isin(angle) * 24, and initStuff(prop, 1089) runs.
//    The PhProp arguments are the text "equipmnt.lmp", word 0 of the globalItem record (16 bytes
//    each) at getProxyMeshId(D) and word 1 of the record at a second getProxyMeshId(D) call, 1,
//    1089, D and 1.0. A failed allocation skips the launch.
//    1. For j = 0, 1, ... while findTagString(o, "item", j) is non-null: item::create(a second
//       lookup of the same name, D, -1, 0).
//    2. For j = 0 .. findTagInt(o, "potions", 0) - 1 (the count is re-read before each pass):
//       makeRandomPotion(D).
//    3. For j = 0, 1, ... while findTagString(o, "random", j) is non-null: v = findTagFloat(o,
//       "random", j) * gameItemTreasureRamp() * (((r2 * 2^-16) * 0.1 + 0.95) * 600.0) with r2 = a
//       new eRand, doubled if bit 0x10000 of levelFlags is set and doubled again if bit 0x8000 is
//       set; then createRandom(D, (int)v, k, -1, 1) with k = the table {0, 1, 2} indexed by a
//       further eRand % 3. A zero result skips the item.
// If the word at o+132 is nonzero: word +84 = 1; "game_pu_coin.anm" from "fx.lmp" is played the same
// way; hudGivePlayerGold(n, copy of the Point3 at +16, signed halfword +76); halfword +76 = 0.
// Inferred: this is the poke message that opens the chest, spawns its contents, and on a second poke
// pays out gold. Unknown: names of the tags' meaning, the full size of ItemDescription (at most 80
// bytes; only the initialised bytes are reproduced), what the globalItem record fields are, and
// the meaning of 1089. The table {0, 1, 2} is the file-local static at 0x42c8c0. Nothing here
// ran on the EE.
struct Point3 {
    float x, y, z;
};

extern "C" {
void* lumpFindResource__FPcT0(char* lump, char* name);
void soundPlaySequence__FP15AnimationHeaderP10GameObjectf(void* header, void* object, float volume);
void animAdd__FP14AnimationStateP15AnimationHeaderi(void* state, void* header, int flags);
void objectAddToRunList__FP10GameObject(void* object);
char* findTagString__10GameObjectPci(void* self, const char* name, int index);
int findTagInt__10GameObjectPci(void* self, const char* name, int index);
float findTagFloat__10GameObjectPci(void* self, const char* name, int index);
int create__4itemPcPQ24item11descriptionii(char* name, void* description, int a, int b);
void makeRandomPotion__4itemPQ24item11description(void* description);
void* createRandom__4itemPQ24item11descriptioniiii(void* description, int a, int b, int c, int d);
int getProxyMeshId__4itemPQ24item11description(void* description);
float gameItemTreasureRamp__Fv();
int eRand__10GameObject(void* self);
void* blockAlloc__Fi(int size);
void phProp_construct(void* self, const Point3* position, char* a, char* b, char* c, int d, int e, void* description, float f)
    __asm__("__6PhPropG6Point3PcN22liPQ24item11descriptionf");
void initStuff__6PhPropi(void* prop, int value);
float icos__Fi(int angle);
float isin__Fi(int angle);
void hudGivePlayerGold__FiG6Point3i(int player, const Point3* position, int amount);
extern unsigned char globalItem[] __asm__("_4item$globalItem");
extern int frameNm;
extern int levelFlags;
}

#include "../w10-actor-d-common/words.hpp"

namespace {
template <typename T>
inline T& field(unsigned char* object, unsigned offset) { return *reinterpret_cast<T*>(object + offset); }

struct ItemDescription {
    unsigned char bytes[80];
};

const int categoryTable[3] = {0, 1, 2};

inline __attribute__((always_inline)) void init_description(ItemDescription& d) {
    for (unsigned i = 36; i <= 60; i += 8) *reinterpret_cast<unsigned short*>(d.bytes + i) = 0xffffu;
    d.bytes[32] = 0xff;
    d.bytes[33] = 0xff;
    for (unsigned i = 16; i < 32; i += 2) *reinterpret_cast<unsigned short*>(d.bytes + i) = 0;
}

inline __attribute__((always_inline)) void spawn_prop(void* self, ItemDescription& d) {
    unsigned char* o = static_cast<unsigned char*>(self);
    void* prop = blockAlloc__Fi(584);
    if (prop == nullptr) return;
    const Point3 offset = {0.0f, 0.0f, 12.0f};
    const Point3* position = reinterpret_cast<const Point3*>(o + 16);
    Point3 where = {position->x + offset.x, position->y, position->z + offset.z};
    const int first = getProxyMeshId__4itemPQ24item11description(&d);
    const int second = getProxyMeshId__4itemPQ24item11description(&d);
    phProp_construct(prop, &where, const_cast<char*>("equipmnt.lmp"), reinterpret_cast<char*>(*reinterpret_cast<unsigned*>(globalItem + 16 * first)),
                     reinterpret_cast<char*>(*reinterpret_cast<unsigned*>(globalItem + 16 * second + 4)), 1, 1089, &d, 1.0f);
    const int roll = eRand__10GameObject(self);
    const int angle = field<short>(o, 56) + ((roll & 0x3fff) - 8192);
    const float c = icos__Fi(angle);
    const float s = isin__Fi(field<short>(o, 56) + ((roll & 0x3fff) - 8192));
    float velocity[3] = {c, s, 0.0f};
    field<float>(static_cast<unsigned char*>(prop), 252) = velocity[0] * 24.0f;
    field<float>(static_cast<unsigned char*>(prop), 256) = velocity[1] * 24.0f;
    initStuff__6PhPropi(prop, 1089);
}
}  // namespace

extern "C" void msg_poke__5Chesti(void* self, int player) {
    unsigned char* o = static_cast<unsigned char*>(self);
    if (field<int>(o, 132) != 0) {
        field<int>(o, 84) = 1;
        void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("game_pu_coin.anm"));
        soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, self, 1.0f);
        const Point3 where = field<Point3>(o, 16);
        hudGivePlayerGold__FiG6Point3i(player, &where, field<short>(o, 76));
        field<short>(o, 76) = 0;
        return;
    }
    void* header = lumpFindResource__FPcT0(const_cast<char*>("fx.lmp"), const_cast<char*>("door_wdchst_open.anm"));
    soundPlaySequence__FP15AnimationHeaderP10GameObjectf(header, self, 1.0f);
    animAdd__FP14AnimationStateP15AnimationHeaderi(o + 92, w10_actor_d::word_at(w10_actor_d::word_at(o, 36), 28), 516);
    field<int>(o, 132) = 1;
    objectAddToRunList__FP10GameObject(self);
    field<int>(o, 80) = frameNm;
    if (field<short>(o, 76) <= 0) field<unsigned>(o, 12) &= 0xffefffffu;
    for (int j = 0; findTagString__10GameObjectPci(self, "item", j) != nullptr; ++j) {
        ItemDescription d;
        init_description(d);
        create__4itemPcPQ24item11descriptionii(findTagString__10GameObjectPci(self, "item", j), &d, -1, 0);
        spawn_prop(self, d);
    }
    for (int j = 0; j < findTagInt__10GameObjectPci(self, "potions", 0); ++j) {
        ItemDescription d;
        init_description(d);
        makeRandomPotion__4itemPQ24item11description(&d);
        spawn_prop(self, d);
    }
    for (int j = 0; findTagString__10GameObjectPci(self, "random", j) != nullptr; ++j) {
        ItemDescription d;
        init_description(d);
        float amount = findTagFloat__10GameObjectPci(self, "random", j);
        amount = amount * gameItemTreasureRamp__Fv();
        const float spread = (static_cast<float>(eRand__10GameObject(self)) * 1.52587890625e-05f * 0.1f + 0.95f) * 600.0f;
        const int flags = levelFlags;
        amount = amount * spread;
        if ((flags & 0x10000) != 0) amount = amount + amount;
        if ((flags & 0x8000) != 0) amount = amount + amount;
        const int category = categoryTable[static_cast<unsigned>(eRand__10GameObject(self)) % 3u];
        if (createRandom__4itemPQ24item11descriptioniiii(&d, static_cast<int>(amount), category, -1, 1) == nullptr) continue;
        spawn_prop(self, d);
    }
}
