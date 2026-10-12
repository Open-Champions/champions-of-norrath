#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "set_state_gnome.cpp"
#include "../combat-reactions-bytes/host_vcall.hpp"
#include <cstdio>
#include <cstring>
#include <initializer_list>
static int failures = 0;
static char log_text[32];
static unsigned log_length = 0;
static int processing_answer = 0;
static int processing_rewrite = -1;
static unsigned random_value = 0;
static void* transition_state = nullptr;
static void* transition_header = nullptr;
static unsigned transition_bits = 0;
static int transition_flags = 0;
static void* synced = nullptr;
static void* slot_self = nullptr;
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
extern "C" unsigned norrath_actor_random(void*) { note('r'); return random_value; }
extern "C" void norrath_animation_add_transition(void* state, void* header, float rate, int flags) {
    note('t'); transition_state = state; transition_header = header; transition_bits = bits_of(rate); transition_flags = flags;
}
extern "C" void animGlobalSync__FP14AnimationState(void* state) { note('g'); synced = state; }
extern "C" void resetRouting__8Creature(void*) { note('z'); }
extern "C" void getSmartRandomDestination__8Creaturei(void* result, void*, int range) {
    note('d'); destination_range = range;
    for (unsigned i = 0; i < 12; ++i) static_cast<unsigned char*>(result)[i] = static_cast<unsigned char>(0x30 + i);
}
extern "C" void death_stub(void* self) { note('k'); slot_self = self; }
static unsigned word(unsigned char* base, unsigned offset) { return combat_reactions::read_word(combat_reactions::address(base) + offset); }
static void put(unsigned char* base, unsigned offset, unsigned value) { combat_reactions::write_word(combat_reactions::address(base) + offset, value); }
static unsigned half(unsigned char* base) { return combat_reactions::read_u16(combat_reactions::address(base) + 572u); }
static void run(unsigned char* object, int state) {
    log_length = 0; log_text[0] = 0;
    setState__14GnomeNavigatori(object, state);
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
    put(table, 28u, combat_reactions::address_bits(headers + 1));
    put(table, 52u, combat_reactions::address_bits(headers + 2));
    put(table, 84u, combat_reactions::address_bits(headers + 3));
    combat_reactions::host_install_slot(object, vtable, 192u, -16, reinterpret_cast<void*>(death_stub));
    put(object, 808u, 4000u);
    random_value = 555;

    processing_answer = 1;
    put(object, 572u, 0x4321u);
    run(object, 3);
    expect(std::strcmp(log_text, "p") == 0 && half(object) == 0x4321u, "nonzero processing result returns without a store");
    processing_answer = 0;

    run(object, 0);
    expect(std::strcmp(log_text, "ptr") == 0, "0: processing, transition, random");
    expect(transition_state == object + 288 && transition_header == headers + 1 && transition_bits == 0x3dccccccu && transition_flags == 3, "0: transition");
    expect(word(object, 256u) == 4000u + 555u % 120u + 60u && half(object) == 0u, "0: wait and state");

    for (unsigned flag_bits : {0u, 0x400000u, 0x800000u, 0xc00000u, 0x3fffffu}) {
        put(object, 160u, flag_bits);
        put(object, 164u, 0xffffffffu);
        run(object, 1);
        expect(std::strcmp(log_text, "pzdtrg") == 0 && destination_range == 200, "1: call order");
        expect(object[272] == 0x30 && object[283] == 0x3b && object[284] == 0, "1: point bytes at +272");
        const bool wide = (flag_bits & 0xc00000u) != 0u;
        expect(transition_header == (wide ? headers + 2 : headers + 3) && transition_flags == 2051, "1: header by bits at +160");
        expect(word(object, 256u) == 4000u + 555u % 120u + 300u && synced == object + 288 && half(object) == 1u, "1: wait, sync, state");
    }
    run(object, 3);
    expect(std::strcmp(log_text, "pztg") == 0 && transition_header == headers + 2 && transition_flags == 2051 && half(object) == 3u, "3");
    run(object, 5);
    expect(std::strcmp(log_text, "pt") == 0 && transition_header == headers + 1 && transition_flags == 6 && half(object) == 5u, "5");
    run(object, 9);
    expect(std::strcmp(log_text, "pt") == 0 && transition_header == headers + 1 && transition_flags == 3 && half(object) == 9u, "9");
    run(object, 10);
    expect(std::strcmp(log_text, "ptk") == 0 && transition_flags == 6 && slot_self == object - 16 && half(object) == 10u, "10");
    expect(transition_bits == 0x3dccccccu, "rate is shared");

    for (int s : {6, 8}) {
        put(object, 572u, 0x0000ff00u);
        run(object, s);
        expect(std::strcmp(log_text, "p") == 0 && half(object) == 0xff00u, "6/8 keep the state");
    }
    processing_rewrite = 8;
    put(object, 572u, 0x00001357u);
    run(object, 5);
    expect(std::strcmp(log_text, "p") == 0 && half(object) == 0x1357u, "processing can rewrite the word");
    processing_rewrite = -1;
    for (int s : {2, 4, 7, 11, 300, 0x30007, -2}) {
        put(object, 572u, 0x66660000u);
        run(object, s);
        expect(std::strcmp(log_text, "p") == 0 && half(object) == (static_cast<unsigned>(s) & 0xffffu) && (word(object, 572u) >> 16) == 0x6666u, "plain states");
    }
    return failures ? 1 : 0;
}
