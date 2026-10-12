#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "update_enemy.cpp"
#include "../combat-reactions-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
using namespace combat_reactions;
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" unsigned frameNm = 0u;

struct Actor { alignas(8) unsigned char bytes[900]; };
struct Thing { alignas(8) unsigned char bytes[64]; };
static Actor creature;
static unsigned char table[512];
static Thing things[8];
static unsigned char nodes[2][32];
static int enemy_answer[8];
static Thing* found_list[8];
static unsigned found_count = 0;

static int destroy_calls = 0, link_calls = 0, find_calls = 0, enemy_calls = 0;
static void* link_head = nullptr;
static void* link_object = nullptr;
static void* link_value = nullptr;
static unsigned char seen_low[12], seen_high[12];
static int seen_capacity = 0;

extern "C" void norrath_list_destroy(void* head) { ++destroy_calls; (void)head; }
extern "C" void* norrath_list_oneway_link(void* head, void* object_head, void* value) {
    ++link_calls; link_head = head; link_object = object_head; link_value = value; return value;
}
extern "C" void objectFindInBox__FG6Point3T0PP10GameObjecti(const void* low, const void* high, void** output, int capacity) {
    ++find_calls;
    std::memcpy(seen_low, low, 12);
    std::memcpy(seen_high, high, 12);
    seen_capacity = capacity;
    for (unsigned i = 0; i < found_count; ++i) output[i] = found_list[i];
    output[found_count] = nullptr;
}
extern "C" int is_enemy_stub(void* self, void* other) {
    ++enemy_calls;
    (void)self;
    for (int i = 0; i < 8; ++i) if (other == things[i].bytes) return enemy_answer[i];
    return 0;
}

static unsigned char* C() { return creature.bytes; }
static void put_word(unsigned char* b, unsigned o, unsigned v) { write_word(address(b) + o, v); }
static unsigned get_word(unsigned char* b, unsigned o) { return read_word(address(b) + o); }
static void put_float(unsigned char* b, unsigned o, float v) { write_single(address(b) + o, v); }
static void place(int i, float x, float y, float z, float size = 0.0f) {
    put_float(things[i].bytes, 16, x); put_float(things[i].bytes, 20, y); put_float(things[i].bytes, 24, z);
    put_float(things[i].bytes, 28, size);
}
static void reset() {
    std::memset(C(), 0, sizeof creature.bytes);
    std::memset(nodes, 0, sizeof nodes);
    for (int i = 0; i < 8; ++i) { std::memset(things[i].bytes, 0, sizeof things[i].bytes); enemy_answer[i] = 0; place(i, 1000, 1000, 1000); }
    found_count = 0;
    destroy_calls = link_calls = find_calls = enemy_calls = 0;
    link_head = link_object = link_value = nullptr;
    put_float(C(), 16, 0); put_float(C(), 20, 0); put_float(C(), 24, 0);
    put_float(C(), 776, 0); put_float(C(), 780, 0); put_float(C(), 784, 0);
    put_float(C(), 788, 10.0f);
    host_install_slot(C(), table, 160u, 0, reinterpret_cast<void*>(is_enemy_stub));
    frameNm = 1000u;
    put_word(C(), 812, 0);
    C()[569] = 1;  // frameNm ^ 1 has low bits 1: no periodic search
}
static void set_head(unsigned slot, unsigned head_offset, int thing) {
    host_note(nodes[slot]);
    host_note(things[thing].bytes);
    write_word(address(nodes[slot]) + 12u, address_bits(things[thing].bytes));
    put_word(C(), head_offset, address_bits(nodes[slot]));
}
static void run() { updateEnemy__8Creature(C()); }

int main() {
    host_note(C());
    host_note(table);
    for (int i = 0; i < 8; ++i) host_note(things[i].bytes);

    // Early outs.
    reset();
    C()[570] = 1; put_word(C(), 576, 0x1234u); set_head(0, 576, 0);
    unsigned head_before = get_word(C(), 576);
    run();
    expect(destroy_calls == 0 && link_calls == 0 && find_calls == 0 && enemy_calls == 0 && get_word(C(), 576) == head_before, "nonzero byte at 570 does nothing");
    reset();
    write_u16(address(C()) + 796u, 0xffffu); set_head(0, 576, 0);
    run();
    expect(destroy_calls == 1 && get_word(C(), 576) == 0u && enemy_calls == 0 && find_calls == 0, "halfword at 796 destroys and clears");
    reset();
    put_word(C(), 160, 0x4000000u); set_head(0, 576, 0);
    run();
    expect(destroy_calls == 1 && get_word(C(), 576) == 0u && link_calls == 0, "bit 0x4000000 destroys and clears");
    reset();
    put_word(C(), 164, 0x4000000u);
    run();
    expect(find_calls == 1, "bit in the high half of the 64-bit value does not count: the search still runs");

    // Held target inside the leash is kept and not relinked, with no search tick.
    reset();
    place(0, 5, 0, 0); enemy_answer[0] = 1; set_head(0, 576, 0);
    run();
    expect(destroy_calls == 0 && link_calls == 0 && find_calls == 0 && get_word(C(), 576) == address_bits(nodes[0]), "held target near home is kept");

    // Held target far from home, creature at home, out of reach: dropped, then search finds nothing.
    reset();
    place(0, 30, 0, 0); enemy_answer[0] = 1; set_head(0, 576, 0);
    run();
    expect(find_calls == 1 && destroy_calls == 1 && get_word(C(), 576) == 0u && link_calls == 0, "far held target is dropped and the list cleared");
    expect(seen_capacity == 128, "search asks for 128 entries");
    const float reach = 10.0f * single_from_bits(0x3f34fdf3u);
    expect(read_single(address(seen_low)) == -reach && read_single(address(seen_low) + 4u) == -reach && read_single(address(seen_low) + 8u) == -reach, "box minimum");
    expect(read_single(address(seen_high)) == reach && read_single(address(seen_high) + 8u) == reach, "box maximum");

    // Hurt recently keeps the far target.
    reset();
    place(0, 30, 0, 0); enemy_answer[0] = 1; set_head(0, 576, 0);
    put_word(C(), 812, 600u);
    run();
    expect(destroy_calls == 0 && find_calls == 0 && link_calls == 0, "recently hurt keeps the held target");
    put_word(C(), 812, 520u);  // 1000 - 480 = 520 is not below 520
    run();
    expect(find_calls == 1, "520 is not below 520 so the target is dropped");

    // Far held target but the creature is the one far from home: kept.
    reset();
    place(0, 30, 0, 0); enemy_answer[0] = 1; set_head(0, 576, 0);
    put_float(C(), 16, 100.0f);
    run();
    expect(find_calls == 0 && destroy_calls == 0, "creature farther from home than the target keeps it");

    // Reach test: size terms plus 50 are not below the squared distance.
    reset();
    place(0, 30, 0, 0, 900.0f); enemy_answer[0] = 1; set_head(0, 576, 0);
    run();
    expect(find_calls == 0 && destroy_calls == 0, "size plus 50 above the squared distance keeps the target");

    // Held entry that is not an enemy: attacker list is tried.
    reset();
    place(0, 5, 0, 0); enemy_answer[0] = 0; set_head(0, 576, 0);
    place(1, 5, 0, 0); enemy_answer[1] = 1; set_head(1, 752, 1);
    run();
    expect(enemy_calls == 2, "both heads asked");
    expect(destroy_calls == 1 && link_calls == 0 && get_word(C(), 576) == 0u, "attacker inside K*leash^2 gives no target: nothing near home");

    reset();
    place(0, 5, 0, 0); enemy_answer[0] = 0; set_head(0, 576, 0);
    place(1, 9, 9, 0); enemy_answer[1] = 1; set_head(1, 752, 1);
    run();
    expect(find_calls == 1 && destroy_calls == 1 && link_calls == 0, "far attacker with the creature at home is dropped, then nothing is found");

    reset();
    place(0, 5, 0, 0); enemy_answer[0] = 0; set_head(0, 576, 0);
    place(1, 9, 9, 0); enemy_answer[1] = 1; set_head(1, 752, 1);
    put_word(C(), 812, 900u);
    run();
    expect(find_calls == 0, "attacker far from home, recently hurt: taken, no search");
    expect(destroy_calls == 1 && link_calls == 1 && link_object == things[1].bytes && link_value == things[1].bytes, "attacker replaces the held head and is linked");
    expect(link_head == C() + 576, "link head is the list at 576");

    reset();
    place(1, 6, 6, 0); enemy_answer[1] = 1; set_head(1, 752, 1);   // 72: above 70.7, below 100
    run();
    expect(find_calls == 0 && link_calls == 1 && destroy_calls == 0, "between K*leash^2 and leash^2 the attacker is taken and kept");

    reset();
    place(1, 5, 5, 0); enemy_answer[1] = 1; set_head(1, 752, 1);   // 50: below 70.7
    run();
    expect(find_calls == 1 && link_calls == 0 && destroy_calls == 1, "attacker inside K*leash^2 is not taken");

    reset();
    place(1, 9, 9, 0); enemy_answer[1] = 1; set_head(1, 752, 1);
    put_word(C(), 812, 900u);
    run();
    expect(destroy_calls == 0 && link_calls == 1 && get_word(C(), 576) == 0u, "empty head is linked without a destroy");

    // Search every 32nd frame when a target is held.
    reset();
    place(0, 5, 0, 0); enemy_answer[0] = 1; set_head(0, 576, 0);
    C()[569] = static_cast<unsigned char>(1000u & 0x1fu);
    run();
    expect(find_calls == 1, "matching low five bits run the search");
    C()[569] = static_cast<unsigned char>((1000u & 0x1fu) | 0x20u);
    find_calls = 0;
    run();
    expect(find_calls == 1, "only the low five bits are compared");
    C()[569] = static_cast<unsigned char>((1000u & 0x1fu) ^ 4u);
    find_calls = 0;
    run();
    expect(find_calls == 0, "other low bits skip the search");

    // Search scoring.
    reset();
    place(0, 2, 0, 0); enemy_answer[0] = 1;            // near, plain
    place(1, 6, 0, 0); enemy_answer[1] = 1;            // farther, plain
    place(2, 3, 0, 0); enemy_answer[2] = 0;            // not an enemy
    found_list[0] = &things[1]; found_list[1] = &things[2]; found_list[2] = &things[0]; found_count = 3;
    run();
    expect(find_calls == 1 && link_calls == 1 && link_object == things[0].bytes, "nearer plain candidate wins ties on score");
    expect(destroy_calls == 0, "empty head needs no destroy");

    reset();
    place(0, 2, 0, 0); enemy_answer[0] = 1;
    place(1, 6, 0, 0); enemy_answer[1] = 1;
    found_list[0] = &things[0]; found_list[1] = &things[1]; found_count = 2;
    set_head(0, 576, 1);                               // the held head names the farther one
    C()[569] = static_cast<unsigned char>(1000u & 0x1fu);
    run();
    expect(destroy_calls == 1 && link_calls == 1 && link_object == things[0].bytes, "listed first, the nearer one lowers the best distance and the held one loses the tie");

    reset();
    place(0, 2, 0, 0); enemy_answer[0] = 1;
    place(1, 6, 0, 0); enemy_answer[1] = 1;
    found_list[0] = &things[1]; found_list[1] = &things[0]; found_count = 2;
    set_head(0, 576, 1);
    C()[569] = static_cast<unsigned char>(1000u & 0x1fu);
    run();
    expect(destroy_calls == 0 && link_calls == 0 && get_word(C(), 576) == address_bits(nodes[0]), "the held object listed first scores 3 and stays");

    reset();
    place(0, 2, 0, 0); enemy_answer[0] = 1;
    place(1, 6, 0, 0); enemy_answer[1] = 1;
    found_list[0] = &things[1]; found_list[1] = &things[0]; found_count = 2;
    set_head(1, 752, 1);                               // the attacker head names the farther one
    run();
    expect(link_calls == 1 && link_object == things[1].bytes, "the attacker object listed first also scores 3");

    reset();
    place(0, 2, 0, 0); enemy_answer[0] = 3;            // isEnemy answers 3: higher base score
    place(1, 1, 0, 0); enemy_answer[1] = 1;
    found_list[0] = &things[1]; found_list[1] = &things[0]; found_count = 2;
    run();
    expect(link_calls == 1 && link_object == things[0].bytes, "higher isEnemy answer outranks a nearer candidate");

    reset();
    place(0, 20, 0, 0); enemy_answer[0] = 1;           // outside the leash radius: no distance bonus
    place(1, 25, 0, 0); enemy_answer[1] = 1;
    found_list[0] = &things[1]; found_list[1] = &things[0]; found_count = 2;
    run();
    expect(link_calls == 1 && link_object == things[0].bytes, "equal scores outside the leash: nearer wins only if inside best distance");
    return failures ? 1 : 0;
}
