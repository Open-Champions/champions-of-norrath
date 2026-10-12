#define NORRATH_COMBAT_REACTIONS_HOST 1
#include "set_state_natasla.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int calls = 0;
static void* seen_state = nullptr;
static void* seen_header = nullptr;
static unsigned seen_rate_bits = 0;
static int seen_flags = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void norrath_animation_add_transition(void* state, void* header, float rate, int flags) {
    ++calls;
    seen_state = state;
    seen_header = header;
    std::memcpy(&seen_rate_bits, &rate, 4);
    seen_flags = flags;
}
static unsigned state_of(unsigned char* object) { return combat_reactions::read_u16(combat_reactions::address(object) + 572u); }
int main() {
    unsigned char object[600];
    unsigned char table[64];
    unsigned char header[4];
    std::memset(object, 0, sizeof object);
    std::memset(table, 0, sizeof table);
    combat_reactions::host_note(table);
    combat_reactions::host_note(header);
    combat_reactions::write_word(combat_reactions::address(object) + 36u, combat_reactions::address_bits(table));
    combat_reactions::write_word(combat_reactions::address(table) + 28u, combat_reactions::address_bits(header));

    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 9u);
    setState__7Nataslai(object, 0);
    expect(calls == 1 && seen_state == object + 288 && seen_header == header, "state 0 adds a transition");
    expect(seen_rate_bits == 0x3e999999u && seen_flags == 3, "rate and flags");
    expect(state_of(object) == 0u, "state 0 is stored");

    const int plain[] = {2, 4, 7, 11, 12, 300, 0x10003};
    for (int value : plain) {
        combat_reactions::write_u16(combat_reactions::address(object) + 572u, 99u);
        setState__7Nataslai(object, value);
        expect(state_of(object) == (static_cast<unsigned>(value) & 0xffffu), "plain states are stored");
    }
    expect(calls == 1, "plain states add no transition");
    const int kept[] = {1, 3, 5, 6, 8, 9, 10};
    for (int value : kept) {
        combat_reactions::write_u16(combat_reactions::address(object) + 572u, 99u);
        setState__7Nataslai(object, value);
        expect(state_of(object) == 99u, "listed states leave the state alone");
    }
    combat_reactions::write_u16(combat_reactions::address(object) + 572u, 99u);
    setState__7Nataslai(object, -1);
    expect(state_of(object) == 0xffffu, "negative argument takes the plain path");
    expect(calls == 1, "still one transition");
    return failures ? 1 : 0;
}
