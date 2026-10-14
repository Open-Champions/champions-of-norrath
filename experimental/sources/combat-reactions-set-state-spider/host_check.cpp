#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "set_state_spider.cpp"
#include "../combat-reactions-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int failures = 0;
static char log_text[32];
static unsigned log_length = 0;
static unsigned random_values[4];
static unsigned random_index = 0;
static int processing_answer = 0;
static int processing_rewrite = -1;
static void* transition_state = nullptr;
static void* transition_header = nullptr;
static unsigned transition_bits = 0;
static int transition_flags = 0;
static void* ended = nullptr;
static void* slot_self = nullptr;
static int floor_angle = 0;
static unsigned floor_extent_bits = 0;
static int floor_kind = 0, floor_extra = 0;
static unsigned char floor_point[12];
static int destination_range = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void note(char c) { log_text[log_length++] = c; log_text[log_length] = 0; }
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" int setStateProcessing__8CreaturePi(void*, int* state) {
    note('p');
    if (processing_rewrite != -1) *state = processing_rewrite;
    return processing_answer;
}
extern "C" unsigned norrath_actor_random(void*) { note('r'); return random_values[random_index++ & 3u]; }
extern "C" void norrath_animation_add_transition(void* state, void* header, float rate, int flags) {
    note('t'); transition_state = state; transition_header = header; transition_bits = bits_of(rate); transition_flags = flags;
}
extern "C" void norrath_animation_set_to_end(void* state) { note('e'); ended = state; }
extern "C" void resetRouting__8Creature(void*) { note('z'); }
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void*, int range) {
    note('d'); destination_range = range;
    for (unsigned i = 0; i < 12; ++i) static_cast<unsigned char*>(result)[i] = static_cast<unsigned char>(0x60 + i);
}
extern "C" void floorAdd__FG6Point3sfii(const unsigned char* point, int angle, float extent, int kind, int extra) {
    note('f'); floor_angle = angle; floor_extent_bits = bits_of(extent); floor_kind = kind; floor_extra = extra;
    std::memcpy(floor_point, point, 12);
}
extern "C" void death_stub(void* self) { note('k'); slot_self = self; }
static unsigned word(unsigned char* base, unsigned offset) { return combat_reactions::read_word(combat_reactions::address(base) + offset); }
static void put(unsigned char* base, unsigned offset, unsigned value) { combat_reactions::write_word(combat_reactions::address(base) + offset, value); }
static unsigned half(unsigned char* base) { return combat_reactions::read_u16(combat_reactions::address(base) + 572u); }
static void run(unsigned char* object, int state) {
    log_length = 0; log_text[0] = 0; random_index = 0;
    setState__11SmallSpideri(object, state);
}
int main() {
    unsigned char object[900];
    unsigned char vtable[256];
    unsigned char table[128];
    unsigned char headers[8];
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    std::memset(vtable, 0, sizeof vtable);
    combat_reactions::host_note(table);
    for (unsigned i = 0; i < 8; ++i) combat_reactions::host_note(headers + i);
    put(object, 36u, combat_reactions::address_bits(table));
    put(table, 60u, combat_reactions::address_bits(headers + 1));
    put(table, 76u, combat_reactions::address_bits(headers + 2));
    for (unsigned i = 0; i < 4; ++i) put(table, 28u + 8u * i, combat_reactions::address_bits(headers + 4 + i));
    combat_reactions::host_install_slot(object, vtable, 192u, 24, reinterpret_cast<void*>(death_stub));
    put(object, 808u, 5000u);
    put(object, 572u, 0x0000abcdu);

    processing_answer = 1;
    run(object, 2);
    expect(std::strcmp(log_text, "p") == 0 && half(object) == 0xabcdu, "nonzero processing result returns without a store");
    processing_answer = 0;

    random_values[0] = 700;
    run(object, 0);
    expect(std::strcmp(log_text, "prt") == 0, "0: processing, random, transition");
    expect(word(object, 256u) == 5000u + 700u % 300u + 120u, "0: wait");
    expect(transition_state == object + 288 && transition_header == headers + 1 && transition_bits == 0x3e999999u && transition_flags == 3, "0: transition");
    expect(half(object) == 0u, "0: state");

    run(object, 1);
    expect(std::strcmp(log_text, "pzdt") == 0 && destination_range == 200, "1: calls");
    expect(object[272] == 0x60 && object[283] == 0x6b && object[284] == 0, "1: point bytes at +272");
    expect(transition_header == headers + 2 && transition_flags == 3 && half(object) == 1u, "1: header at +76");

    for (int s : {5, 8, 9}) {
        put(object, 572u, 0x0000fffeu);
        run(object, s);
        expect(std::strcmp(log_text, "p") == 0 && half(object) == 0xfffeu, "5/8/9 keep the current state");
    }
    processing_rewrite = 9;
    put(object, 572u, 0x00001234u);
    run(object, 4);
    expect(half(object) == 0x1234u, "processing can rewrite the word and 9 then keeps the state");
    processing_rewrite = 3;
    run(object, 12);
    expect(half(object) == 3u, "rewritten word is what gets stored");
    processing_rewrite = -1;

    for (int s : {2, 3, 4, 6, 7, 11, 70000}) {
        put(object, 572u, 0x55550000u);
        run(object, s);
        expect(std::strcmp(log_text, "p") == 0 && half(object) == (static_cast<unsigned>(s) & 0xffffu) && (word(object, 572u) >> 16) == 0x5555u, "plain states");
    }

    for (unsigned i = 0; i < 12; ++i) object[16 + i] = static_cast<unsigned char>(i + 1);
    put(object, 12u, 0x100u);
    random_values[0] = 0x18005; random_values[1] = 27; random_values[2] = 0x1ffa;
    run(object, 10);
    expect(std::strcmp(log_text, "prrfrtk") == 0, "10: processing, two randoms, floor, random, transition, slot");
    expect(word(object, 12u) == 0x109u, "10: flags ORed with 9");
    expect(floor_angle == -32763 && floor_extent_bits == bits_of(17.0f) && floor_kind == 6 && floor_extra == 1, "10: floor arguments");
    expect(std::memcmp(floor_point, object + 16, 12) == 0, "10: point copy");
    expect(transition_header == headers + 4 + 2 && transition_flags == 6 && transition_bits == 0x3e999999u, "10: header chosen by random and 3");
    expect(slot_self == object + 24, "10: death slot gets the adjusted object");
    expect(half(object) == 10u, "10: state");
    put(object, 12u, 0x102u);
    run(object, 10);
    expect(std::strcmp(log_text, "prrfrtek") == 0 && ended == object + 288, "10: bit 2 ends the animation before the slot");
    return failures ? 1 : 0;
}
