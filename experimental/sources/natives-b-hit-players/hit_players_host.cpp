#include <cstdio>
#include <cstring>
extern "C" int amxHitPlayers__FP5__amxPi(void*, int*);
extern "C" int nmPlayersAlive = 0;
extern "C" unsigned char norrath_route_nodes_bytes[5344 + 16] = {};
extern "C" unsigned char norrath_player_progression_records_view[23984 * 2] = {};
static unsigned char object[28];
static unsigned char near_creature[760];
static unsigned char far_creature[760];
static unsigned char table[32];
static int finds = 0;
static int hits = 0;
static void* self_seen = nullptr;
static void* position_seen = nullptr;
static float damage_seen = 0.f;
extern "C" int amx_GetAddr(void*, int, void** physical) {
    static char packed[4];
    *physical = packed;
    return 0;
}
extern "C" int amx_GetString(char* destination, void*) {
    std::strcpy(destination, "bolt");
    return 0;
}
extern "C" void* objectFindObject__FPc(char* name) {
    finds += 1;
    if (std::strcmp(name, "bolt") != 0) return nullptr;
    return object;
}
extern "C" int iatan2__Fff(float y, float x) {
    if (y != 0.f || x != 3.f) return -1;
    return 0x1234;
}
static void on_hit(void* self, int zero_a, int zero_b, void* position, float damage) {
    hits += 1;
    if (zero_a != 0 || zero_b != 0) hits = -100;
    self_seen = self;
    position_seen = position;
    damage_seen = damage;
}
static void put_float(unsigned char* bytes, float value) {
    union Bits { unsigned word; float value; } bits;
    bits.value = value;
    std::memcpy(bytes, &bits.word, 4);
}

int main() {
    std::memset(object, 0, sizeof object);
    std::memset(near_creature, 0, sizeof near_creature);
    std::memset(far_creature, 0, sizeof far_creature);
    put_float(near_creature + 16, 3.f);
    put_float(far_creature + 16, 10.f);
    *reinterpret_cast<void**>(near_creature + 72) = table;
    const short adjust = 4;
    std::memcpy(table + 16, &adjust, sizeof adjust);
    void (*fn)(void*, int, int, void*, float) = on_hit;
    std::memcpy(table + 20, &fn, sizeof fn);
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 21872) = near_creature;
    *reinterpret_cast<void**>(norrath_player_progression_records_view + 23984 + 21872) = far_creature;
    int cells[] = {16, 1, 4, 7, 9};
    if (amxHitPlayers__FP5__amxPi(nullptr, cells) != 0) return 1;
    if (finds != 1 || hits != 0) return 2;
    nmPlayersAlive = 2;
    norrath_route_nodes_bytes[5344] = 0;
    norrath_route_nodes_bytes[5348] = 1;
    far_creature[756] = 0xab;
    if (amxHitPlayers__FP5__amxPi(nullptr, cells) != 0) return 3;
    if (hits != 1 || self_seen != near_creature + 4 || position_seen != near_creature + 16) return 4;
    if (damage_seen != 7.f || far_creature[756] != 0xab) return 5;
    if (near_creature[748] != 0x34 || near_creature[749] != 0x12) return 6;
    unsigned word = 0;
    std::memcpy(&word, near_creature + 756, 4);
    if (word != 0x41100000u) return 7;
    nmPlayersAlive = 1;
    norrath_route_nodes_bytes[5344] = 1;
    put_float(far_creature + 16, 4.f);
    if (amxHitPlayers__FP5__amxPi(nullptr, cells) != 0) return 8;
    if (hits != 1) return 9;
    std::puts("amxHitPlayers passed");
    return 0;
}
