// Observed dropTreasure__8CreatureG6Point3iffffffPc at 0x0010c338, 3864 bytes.
// Arguments: the creature, a Point3 passed by address, an int n, six singles f0..f5 and a text
// pointer. The Point3 is copied to a local first. If bit 32 of the 64-bit word at o+160 is set
// nothing else happens. Otherwise the dynamic type of the creature is compared, in this order,
// with the type-info nodes returned by __tf8SuperOrc, __tf11SpiderQueen, __tf8AntQueen,
// __tf12UndeadKnight, __tf11VampireLord, __tf10Arenabeast, __tf10SeaMonster, __tf10CloudGiant,
// __tf6Minion, __tf9MummyKing and __tf8Innoruuk. Each comparison reads the node through the
// function pointer at table byte 4 (the type-info slot) of the table at o+72, called without
// adjusting the object, and calls __throw_bad_typeid when the object pointer is null; the
// comparison is __eq__C9type_infoRC9type_info(dynamic, wanted).
// SeaMonster only counts as matched when bit 0 of the 64-bit word at +160 is set (read again) and
// CloudGiant only when the word at +976 is nonzero; otherwise the chain of comparisons simply goes on
// with the next class.
// For the first ten classes the item::dropTreasure(local Point3, n, f0, f1, f2', f3', f4', f5',
// text, 0) overload is called four times in a row (the callee is dropTreasure__4itemG6Point3i
// ffffffPci, called as item::dropTreasure below):
//   1. (f2, f3, f4, f5, text, 0);
//   2. (4.0, 1.0, 6.0, 1.0, null, 0);
//   3. (5.0, 1.0, 7.0, 1.0, null, 0);
//   4. repeated once for every player slot while the counter is below nmPlayers (at least one pass
//      is made only when nmPlayers is positive): (0.0, 0.0, 8.0, 1.0, null, 1).
//   f0 and f1 are passed unchanged every time.
// For Innoruuk the second, third and per-player calls are (15.0, 1.0, 7.0, 1.0, null, 0),
// (20.0, 1.0, 8.0, 1.0, null, 0) and (0.0, 0.0, 9.0, 1.0, null, 1).
// For any other type: f2 is multiplied by 5.0 and f4 by 7.0 when bit 0 of the word at o+160 is set;
// then, when the signed byte at o+156 is nonzero, f4 and f2 are multiplied by 3.0; one call of the
// item overload follows with (f0, f1, f2, f3, f4, f5, text, 0).
// Inferred: this is the treasure drop of a dying creature, with one scripted showering of items for
// each boss class. Unknown: the item overload's parameters, the meaning of bit 32 and bit 0 of the
// word at +160 and of byte +156. Nothing here ran on the EE.
#include "../w10-actor-d-common/words.hpp"

struct Point3 {
    float x, y, z;
};

extern "C" {
void item_dropTreasure(const Point3* point, int n, float f0, float f1, float f2, float f3, float f4, float f5, char* text, int flag)
    __asm__("dropTreasure__4itemG6Point3iffffffPci");
int __eq__C9type_infoRC9type_info(const void* left, const void* right);
void __throw_bad_typeid();
const void* __tf8SuperOrc();
const void* __tf11SpiderQueen();
const void* __tf8AntQueen();
const void* __tf12UndeadKnight();
const void* __tf11VampireLord();
const void* __tf10Arenabeast();
const void* __tf10SeaMonster();
const void* __tf10CloudGiant();
const void* __tf6Minion();
const void* __tf9MummyKing();
const void* __tf8Innoruuk();
extern int nmPlayers;
}

#if defined(W10_ACTOR_D_HOST)
extern const void* w10_actor_d_type_slot(unsigned function);
#endif

namespace {
typedef const void* (*TypeFunction)();
typedef const void* (*TypeSlot)();

inline const void* dynamic_type(unsigned char* object) {
    if (object == nullptr) __throw_bad_typeid();
    const unsigned char* table = w10_actor_d::word_at(object, 72);
    const unsigned function = *reinterpret_cast<const unsigned*>(table + 4);
#if defined(W10_ACTOR_D_HOST)
    return w10_actor_d_type_slot(function);
#else
    return reinterpret_cast<TypeSlot>(static_cast<__UINTPTR_TYPE__>(function))();
#endif
}

inline bool is_type(unsigned char* object, TypeFunction wanted) {
    const void* dynamic = dynamic_type(object);
    const void* candidate = wanted();
    return __eq__C9type_infoRC9type_info(dynamic, candidate) != 0;
}
}  // namespace

extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const Point3* point, int n, float f0, float f1, float f2, float f3, float f4, float f5, char* text)
    __asm__("dropTreasure__8CreatureG6Point3iffffffPc");
extern "C" void dropTreasure__8CreatureG6Point3iffffffPc(void* self, const Point3* point, int n, float f0, float f1, float f2, float f3, float f4, float f5, char* text) {
    unsigned char* o = static_cast<unsigned char*>(self);
    const unsigned long long flags = *reinterpret_cast<unsigned long long*>(o + 160);
    const Point3 local = *point;
    if ((flags & 0x100000000ull) != 0) return;
    static TypeFunction const boss[10] = {__tf8SuperOrc, __tf11SpiderQueen, __tf8AntQueen, __tf12UndeadKnight, __tf11VampireLord,
                                          __tf10Arenabeast, __tf10SeaMonster, __tf10CloudGiant, __tf6Minion, __tf9MummyKing};
    for (int i = 0; i < 10; ++i) {
        if (!is_type(o, boss[i])) continue;
        if (i == 6 && static_cast<int>(*reinterpret_cast<unsigned long long*>(o + 160) & 1u) == 0) continue;
        if (i == 7 && *reinterpret_cast<int*>(o + 976) == 0) continue;
        item_dropTreasure(&local, n, f0, f1, f2, f3, f4, f5, text, 0);
        item_dropTreasure(&local, n, f0, f1, 4.0f, 1.0f, 6.0f, 1.0f, nullptr, 0);
        item_dropTreasure(&local, n, f0, f1, 5.0f, 1.0f, 7.0f, 1.0f, nullptr, 0);
        if (nmPlayers > 0) {
            int player = 0;
            do {
                item_dropTreasure(&local, n, f0, f1, 0.0f, 0.0f, 8.0f, 1.0f, nullptr, 1);
                ++player;
            } while (player < nmPlayers);
        }
        return;
    }
    if (is_type(o, __tf8Innoruuk)) {
        item_dropTreasure(&local, n, f0, f1, f2, f3, f4, f5, text, 0);
        item_dropTreasure(&local, n, f0, f1, 15.0f, 1.0f, 7.0f, 1.0f, nullptr, 0);
        item_dropTreasure(&local, n, f0, f1, 20.0f, 1.0f, 8.0f, 1.0f, nullptr, 0);
        if (nmPlayers > 0) {
            int player = 0;
            do {
                item_dropTreasure(&local, n, f0, f1, 0.0f, 0.0f, 9.0f, 1.0f, nullptr, 1);
                ++player;
            } while (player < nmPlayers);
        }
        return;
    }
    if (static_cast<int>(flags & 1u) != 0) {
        f2 = f2 * 5.0f;
        f4 = f4 * 7.0f;
    }
    if (static_cast<signed char>(o[156]) != 0) {
        f4 = f4 * 3.0f;
        f2 = f2 * 3.0f;
    }
    item_dropTreasure(&local, n, f0, f1, f2, f3, f4, f5, text, 0);
}
