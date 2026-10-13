// Host contract for dropTreasure__8CreatureG6Point3iffffffPc; not an EE input. Pointer words are
// offsets from a host base (W10_ACTOR_D_HOST). The type-info slot returns a scripted node.
#define W10_ACTOR_D_HOST 1
#include <cstdio>
#include <cstring>
#include <string>
unsigned char* w10_actor_d_host_base;
typedef void (*HostFn)(void*, int);
HostFn w10_actor_d_host_functions[1];
static const void* currentType;
const void* w10_actor_d_type_slot(unsigned) { return currentType; }
extern "C" int nmPlayers;
int nmPlayers;
#include "drop_treasure.cpp"
static std::string calls;
static char nodes[11];
static const char* nodeNames[11] = {"orc", "spider", "ant", "knight", "vampire", "arena", "sea", "cloud", "minion", "mummy", "innoruuk"};
extern "C" {
const void* __tf8SuperOrc() { return &nodes[0]; }
const void* __tf11SpiderQueen() { return &nodes[1]; }
const void* __tf8AntQueen() { return &nodes[2]; }
const void* __tf12UndeadKnight() { return &nodes[3]; }
const void* __tf11VampireLord() { return &nodes[4]; }
const void* __tf10Arenabeast() { return &nodes[5]; }
const void* __tf10SeaMonster() { return &nodes[6]; }
const void* __tf10CloudGiant() { return &nodes[7]; }
const void* __tf6Minion() { return &nodes[8]; }
const void* __tf9MummyKing() { return &nodes[9]; }
const void* __tf8Innoruuk() { return &nodes[10]; }
int __eq__C9type_infoRC9type_info(const void* a, const void* b) { return a == b; }
void __throw_bad_typeid() { calls += "throw;"; }
void item_dropTreasure(const Point3* p, int n, float f0, float f1, float f2, float f3, float f4, float f5, char* text, int flag) {
    char b[160];
    std::snprintf(b, sizeof b, "item(%g,%d,%g,%g,%g,%g,%g,%g,%s,%d);", p->x, n, f0, f1, f2, f3, f4, f5, text ? text : "null", flag);
    calls += b;
}
}
static int failures;
static void expect(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n  calls: %s\n", what, calls.c_str()); ++failures; } }
int main() {
    alignas(16) static unsigned char arena[8192];
    w10_actor_d_host_base = arena;
    unsigned char* object = arena + 2048;
    char name[] = "txt";
    const Point3 point = {9.0f, 8.0f, 7.0f};
    auto run = [&](int typeIndex, unsigned long long flags, signed char b156, int w976, int players) {
        std::memset(arena, 0, sizeof arena); unsigned table = 4096; std::memcpy(object + 72, &table, 4);
        std::memcpy(object + 160, &flags, 8); object[156] = static_cast<unsigned char>(b156); std::memcpy(object + 976, &w976, 4);
        currentType = typeIndex < 0 ? nullptr : &nodes[typeIndex]; nmPlayers = players; calls.clear();
        dropTreasure__8CreatureG6Point3iffffffPc(object, &point, 3, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, 13.0f, name);
    };
    const std::string first = "item(9,3,1,2,10,11,12,13,txt,0);";
    for (int t : {0, 1, 2, 3, 4, 5, 8, 9}) {
        run(t, 0, 0, 0, 2);
        expect(calls == first + "item(9,3,1,2,4,1,6,1,null,0);item(9,3,1,2,5,1,7,1,null,0);item(9,3,1,2,0,0,8,1,null,1);item(9,3,1,2,0,0,8,1,null,1);", nodeNames[t]);
    }
    run(0, 0, 0, 0, 0); expect(calls == first + "item(9,3,1,2,4,1,6,1,null,0);item(9,3,1,2,5,1,7,1,null,0);", "no players, no per-player calls");
    run(0, 0, 0, 0, -3); expect(calls.find("null,1);") == std::string::npos, "negative player count");
    run(0, 0, 0, 0, 1); expect(calls.size() > 0 && calls.rfind("null,1);") != std::string::npos, "one player");
    run(10, 0, 0, 0, 1); expect(calls == first + "item(9,3,1,2,15,1,7,1,null,0);item(9,3,1,2,20,1,8,1,null,0);item(9,3,1,2,0,0,9,1,null,1);", "Innoruuk");
    run(6, 1, 0, 0, 1); expect(calls.find("item(9,3,1,2,4,1,6,1,null,0);") != std::string::npos, "SeaMonster with bit 0");
    run(6, 0, 0, 0, 1); expect(calls == "item(9,3,1,2,10,11,12,13,txt,0);", "SeaMonster without bit 0 falls to the default");
    run(7, 0, 0, 1, 1); expect(calls.find("item(9,3,1,2,4,1,6,1,null,0);") != std::string::npos, "CloudGiant with +976");
    run(7, 0, 0, 0, 1); expect(calls == "item(9,3,1,2,10,11,12,13,txt,0);", "CloudGiant without +976");
    run(7, 1, 0, 0, 1); expect(calls == "item(9,3,1,2,50,11,84,13,txt,0);", "CloudGiant without +976 and bit 0: default multipliers 5 and 7");
    run(-2 + 3, 0, 0, 0, 1); (void)0;
    currentType = &nodes[0];
    run(0, 0x100000000ull, 0, 0, 1); expect(calls.empty(), "bit 32 stops everything");
    run(0, 0x100000001ull, 0, 0, 1); expect(calls.empty(), "bit 32 with bit 0");
    // other type: default block
    std::memset(arena, 0, sizeof arena);
    auto other = [&](unsigned long long flags, signed char b156) {
        std::memset(arena, 0, sizeof arena); unsigned table = 4096; std::memcpy(object + 72, &table, 4);
        std::memcpy(object + 160, &flags, 8); object[156] = static_cast<unsigned char>(b156); static char foreign; currentType = &foreign; nmPlayers = 3; calls.clear();
        dropTreasure__8CreatureG6Point3iffffffPc(object, &point, 3, 1.0f, 2.0f, 10.0f, 11.0f, 12.0f, 13.0f, name);
    };
    other(0, 0); expect(calls == "item(9,3,1,2,10,11,12,13,txt,0);", "default, no flags");
    other(1, 0); expect(calls == "item(9,3,1,2,50,11,84,13,txt,0);", "default, bit 0");
    other(0, 2); expect(calls == "item(9,3,1,2,30,11,36,13,txt,0);", "default, byte 156");
    other(1, -1); expect(calls == "item(9,3,1,2,150,11,252,13,txt,0);", "default, both");
    other(0x2, 0); expect(calls == "item(9,3,1,2,10,11,12,13,txt,0);", "default, bit 1 only");
    return failures ? 1 : 0;
}
