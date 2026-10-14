#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "set_state_mermaid.cpp"
#include <cstdio>
#include <initializer_list>
#include <cstring>
static int failures = 0;
static char log_text[32];
static unsigned log_length = 0;
static unsigned random_answer = 0;
static void* random_object = nullptr;
static void* transition_state = nullptr;
static void* transition_header = nullptr;
static unsigned transition_bits = 0;
static int transition_flags = 0;
static void* shot_state = nullptr;
static void* shot_header = nullptr;
static unsigned shot_first = 0, shot_second = 0;
static int shot_flags = 0;
static void* routed = nullptr;
static void* destination_self = nullptr;
static int destination_range = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void note(char c) { log_text[log_length++] = c; log_text[log_length] = 0; }
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" unsigned norrath_actor_random(void* object) { note('r'); random_object = object; return random_answer; }
extern "C" void norrath_animation_add_transition(void* state, void* header, float rate, int flags) {
    note('t'); transition_state = state; transition_header = header; transition_bits = bits_of(rate); transition_flags = flags;
}
extern "C" void resetRouting__8Creature(void* self) { note('z'); routed = self; }
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void* self, int range) {
    note('d'); destination_self = self; destination_range = range;
    for (unsigned i = 0; i < 12; ++i) static_cast<unsigned char*>(result)[i] = static_cast<unsigned char>(0x40 + i);
}
extern "C" void animAddOneShot__FP14AnimationStateP15AnimationHeaderffi(void* state, void* header, float a, float b, int flags) {
    note('o'); shot_state = state; shot_header = header; shot_first = bits_of(a); shot_second = bits_of(b); shot_flags = flags;
}
static unsigned word(unsigned char* base, unsigned offset) { return combat_reactions::read_word(combat_reactions::address(base) + offset); }
static void put(unsigned char* base, unsigned offset, unsigned value) { combat_reactions::write_word(combat_reactions::address(base) + offset, value); }
static void run(unsigned char* object, int state) {
    log_length = 0; log_text[0] = 0;
    setState__7Mermaidi(object, state);
}
int main() {
    unsigned char object[900];
    unsigned char table[128];
    unsigned char header84[4], header92[4];
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    combat_reactions::host_note(table);
    combat_reactions::host_note(header84);
    combat_reactions::host_note(header92);
    put(object, 36u, combat_reactions::address_bits(table));
    put(table, 84u, combat_reactions::address_bits(header84));
    put(table, 92u, combat_reactions::address_bits(header92));
    put(object, 808u, 1000u);
    random_answer = 250;

    run(object, 0);
    expect(std::strcmp(log_text, "rt") == 0, "0: random then transition");
    expect(word(object, 256u) == 1000u + 250u % 120u, "0: wait is 808 plus random mod 120");
    expect(transition_state == object + 288 && transition_header == header92, "0: animation and header at +92");
    expect(transition_bits == 0x3e999999u && transition_flags == 3, "0: rate and flags");
    expect(random_object == object && (word(object, 572u) & 0xffffu) == 0u, "0: random object and state");

    run(object, 1);
    expect(std::strcmp(log_text, "zdrt") == 0, "1: reset, destination, random, transition");
    expect(routed == object && destination_self == object && destination_range == 200, "1: object and range");
    expect(object[272] == 0x40 && object[283] == 0x4b && object[284] == 0, "1: twelve point bytes at +272");
    expect(word(object, 256u) == 1000u + 300u + 250u % 120u, "1: wait adds 300");
    expect(transition_header == header84 && transition_bits == 0x3e999999u && transition_flags == 2051, "1: header at +84, flags 2051");
    expect((word(object, 572u) & 0xffffu) == 1u, "1: state");

    for (int s : {5, 8}) {
        run(object, s);
        expect(std::strcmp(log_text, "o") == 0, "one-shot only");
        expect(shot_state == object + 288 && shot_header == header92, "one-shot target");
        expect(shot_first == 0x3dcccccc && shot_second == 0x3dcccccc && shot_flags == 16, "one-shot values");
        expect((word(object, 572u) & 0xffffu) == static_cast<unsigned>(s), "one-shot state");
    }
    run(object, 10);
    expect(std::strcmp(log_text, "t") == 0 && transition_header == header92 && transition_flags == 6 && transition_bits == 0x3e999999u, "10: transition flags 6");
    for (int s : {2, 3, 4, 6, 7, 9, 11, 500, 0x10000 + 7}) {
        put(object, 572u, 0x11110000u);
        run(object, s);
        expect(log_length == 0, "plain states make no calls");
        expect((word(object, 572u) & 0xffffu) == (static_cast<unsigned>(s) & 0xffffu), "plain states store");
        expect((word(object, 572u) >> 16) == 0x1111u, "store is a halfword");
    }
    run(object, -1);
    expect(log_length == 0 && (word(object, 572u) & 0xffffu) == 0xffffu, "negative argument is plain");
    return failures ? 1 : 0;
}
