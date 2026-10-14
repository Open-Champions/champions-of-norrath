#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "collision_gold.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static char log_text[64];
static unsigned log_length = 0;
static char seen_lump[16];
static char seen_resource[32];
static void* seen_header = nullptr;
static void* seen_sound_object = nullptr;
static float seen_gain = 0.0f;
static int seen_player = 0;
static int seen_amount = 0;
static unsigned char seen_point[12];
static void* seen_deleted = nullptr;
static int header_marker = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void note(char c) { log_text[log_length++] = c; log_text[log_length] = 0; }
extern "C" void* norrath_lump_find_resource(char* lump_name, char* resource_name) {
    note('l');
    std::strcpy(seen_lump, lump_name);
    std::strcpy(seen_resource, resource_name);
    return &header_marker;
}
extern "C" int norrath_sound_play_sequence(void* header, void* object, float gain) {
    note('p');
    seen_header = header; seen_sound_object = object; seen_gain = gain;
    return 0;
}
extern "C" void hudGivePlayerGold__FiG6Point3i(int player, const void* point, int amount) {
    note('g');
    seen_player = player; seen_amount = amount;
    std::memcpy(seen_point, point, 12);
}
extern "C" unsigned norrath_object_add_to_delete_list(void* object) { note('x'); seen_deleted = object; return 1u; }
static void put(unsigned char* base, unsigned offset, unsigned value) {
    combat_reactions::write_word(combat_reactions::address(base) + offset, value);
}
static void run(unsigned char* object, unsigned char* other) {
    log_length = 0; log_text[0] = 0;
    msg_collision__4GoldP10GameObject(object, other);
}
int main() {
    unsigned char object[200];
    unsigned char other[1000];
    std::memset(object, 0, sizeof object);
    std::memset(other, 0, sizeof other);
    for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(50 + i);
    object[76] = 1;
    put(object, 128u, 25u);
    put(other, 12u, 0x40u);
    put(other, 960u, 2u);

    put(other, 12u, 0x20040u);
    run(object, other);
    expect(log_length == 0, "bit 0x20000 on the other object stops everything");
    put(other, 12u, 0x40u);
    object[76] = 0;
    run(object, other);
    expect(log_length == 0, "zero byte at 76 stops everything");
    object[76] = 0x80;
    put(other, 12u, 0x3fu);
    run(object, other);
    expect(log_length == 0, "missing bit 0x40 stops everything");
    put(other, 12u, 0x40u);
    put(object, 128u, 0u);
    run(object, other);
    expect(log_length == 0, "zero amount stops everything");

    put(object, 128u, 25u);
    run(object, other);
    expect(std::strcmp(log_text, "lpgx") == 0, "lookup, sound, gold, delete in order");
    expect(std::strcmp(seen_lump, "fx.lmp") == 0 && std::strcmp(seen_resource, "inv_equip_coin.anm") == 0, "lookup names");
    expect(seen_header == &header_marker && seen_sound_object == object && seen_gain == 1.0f, "sound arguments");
    expect(seen_player == 2 && seen_amount == 25, "player and amount");
    expect(std::memcmp(seen_point, object + 16, 12) == 0, "point copy");
    expect(combat_reactions::read_word(combat_reactions::address(object) + 128u) == 0u, "amount cleared");
    expect(seen_deleted == object, "object deleted");
    return failures ? 1 : 0;
}
