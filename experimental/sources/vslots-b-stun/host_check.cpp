#define NORRATH_VSLOTS_B_HOST 1
#include "stun.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static int calls = 0;
static void* seen_self = nullptr;
static int seen_state = -1;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
extern "C" void set_state_stub(void* self, int state) {
    ++calls;
    seen_self = self;
    seen_state = state;
}
int main() {
    unsigned char object[900];
    unsigned char slot[8];
    unsigned char shifted[8];
    std::memset(object, 0x3c, sizeof object);
    std::memset(slot, 0, sizeof slot);
    vslots_b::write_u16(vslots_b::address(slot), 8u);
    vslots_b::write_word(vslots_b::address(slot) + 4u, vslots_b::address_bits(reinterpret_cast<void*>(set_state_stub)));
    vslots_b::host_note(reinterpret_cast<void*>(set_state_stub));
    vslots_b::host_note(slot);
    vslots_b::write_word(vslots_b::address(object) + 72u, vslots_b::address_bits(slot) - 184u);
    vslots_b::host_note(slot - 184);
    vslots_b::write_u16(vslots_b::address(object) + 572u, 10u);
    vslots_b::write_word(vslots_b::address(object) + 804u, 0x11111111u);
    vslots_b::write_word(vslots_b::address(object) + 808u, 0x20u);
    stun__8Creaturei(object, 5);
    expect(calls == 0, "state 10 does not call");
    expect(vslots_b::read_word(vslots_b::address(object) + 804u) == 0x11111111u, "state 10 leaves 804");

    vslots_b::write_u16(vslots_b::address(object) + 572u, 3u);
    stun__8Creaturei(object, -2);
    expect(calls == 1, "other states call once");
    expect(seen_self == object + 8, "adjustment is added to this");
    expect(seen_state == 9, "called state is 9");
    expect(vslots_b::read_word(vslots_b::address(object) + 804u) == 0x1eu, "804 is 808 plus the argument");
    expect(vslots_b::read_word(vslots_b::address(object) + 808u) == 0x20u, "808 stays");
    (void)shifted;
    return failures ? 1 : 0;
}
