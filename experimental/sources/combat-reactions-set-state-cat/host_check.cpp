#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "set_state_cat.cpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int failures = 0;
static char log_text[32];
static unsigned log_length = 0;
static unsigned random_values[4];
static unsigned random_index = 0;
static void* transition_state = nullptr;
static void* transition_header = nullptr;
static unsigned transition_bits = 0;
static int transition_flags = 0;
static int destination_range = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static void note(char c) { log_text[log_length++] = c; log_text[log_length] = 0; }
static unsigned bits_of(float f) { unsigned u; std::memcpy(&u, &f, 4); return u; }
extern "C" unsigned norrath_actor_random(void*) { note('r'); return random_values[random_index++ & 3u]; }
extern "C" void norrath_animation_add_transition(void* state, void* header, float rate, int flags) {
    note('t'); transition_state = state; transition_header = header; transition_bits = bits_of(rate); transition_flags = flags;
}
extern "C" void resetRouting__8Creature(void*) { note('z'); }
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void*, int range) {
    note('d'); destination_range = range;
    std::memset(result, 0x77, 12);
}
static unsigned word(unsigned char* base, unsigned offset) { return combat_reactions::read_word(combat_reactions::address(base) + offset); }
static void put(unsigned char* base, unsigned offset, unsigned value) { combat_reactions::write_word(combat_reactions::address(base) + offset, value); }
static unsigned half(unsigned char* base) { return combat_reactions::read_u16(combat_reactions::address(base) + 572u); }
static void run(unsigned char* object, int state) {
    log_length = 0; log_text[0] = 0; random_index = 0;
    setState__3Cati(object, state);
}
int main() {
    unsigned char object[1000];
    unsigned char table[160];
    unsigned char headers[4];
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    combat_reactions::host_note(table);
    combat_reactions::host_note(headers);
    combat_reactions::host_note(headers + 1);
    combat_reactions::host_note(headers + 2);
    put(object, 36u, combat_reactions::address_bits(table));
    put(table, 116u, combat_reactions::address_bits(headers));
    put(table, 132u, combat_reactions::address_bits(headers + 1));
    put(object, 808u, 10000u);
    random_values[0] = 1000; random_values[1] = 777;

    run(object, 0);
    expect(std::strcmp(log_text, "rrt") == 0, "0: two randoms then a transition");
    expect(word(object, 968u) == 10000u + 1000u % 120u + 60u, "0: first wait");
    expect(word(object, 972u) == 10000u + 777u % 60u + 120u, "0: second wait");
    expect(transition_state == object + 288 && transition_header == headers && transition_bits == 0x3f000000u && transition_flags == 3, "0: transition");
    expect(half(object) == 0u, "0: state");

    run(object, 13);
    expect(std::strcmp(log_text, "rrt") == 0, "13: calls");
    expect(word(object, 968u) == 10000u + 1000u % 1800u + 1200u && word(object, 972u) == 10000u + 777u % 480u + 240u, "13: waits");
    expect(transition_header == headers + 1 && transition_bits == 0x3e999999u && transition_flags == 3 && half(object) == 13u, "13: transition");

    for (int s : {1, 3}) {
        float negative = -2.5f;
        std::memcpy(object + 336, &negative, 4);
        put(object, 340u, 0xdeadbeefu);
        run(object, s);
        expect(std::strcmp(log_text, "zdr") == 0 && destination_range == 300, "1/3: calls");
        expect(word(object, 176u) == (s == 1 ? 0x41a00000u : 0x42200000u), "1/3: single at 176");
        expect(word(object, 968u) == 10000u + 1000u % 300u + 180u, "1/3: wait");
        expect(word(object, 336u) == 0u, "1/3: negative single becomes zero");
        expect(word(object, 340u) == 0x3eaaaaabu, "1/3: single at 340");
        expect(half(object) == static_cast<unsigned>(s), "1/3: state");
    }
    float positive = 4.0f;
    std::memcpy(object + 336, &positive, 4);
    run(object, 1);
    expect(word(object, 336u) == bits_of(4.0f), "non-negative single stays");

    put(object, 572u, 0x00007777u);
    run(object, 6);
    expect(log_length == 0 && half(object) == 3u, "6 stores 3");
    put(object, 572u, 0x77770055u);
    run(object, 8);
    expect(log_length == 0 && half(object) == 0x55u && (word(object, 572u) >> 16) == 0x7777u, "8 keeps the state");
    for (int s : {2, 4, 5, 7, 9, 10, 11, 12, 14, 99, 0x20005, -1}) {
        run(object, s);
        expect(log_length == 0 && half(object) == (static_cast<unsigned>(s) & 0xffffu), "plain states");
    }
    return failures ? 1 : 0;
}
