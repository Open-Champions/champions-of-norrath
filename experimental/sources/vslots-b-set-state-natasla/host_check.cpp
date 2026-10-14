#define NORRATH_VSLOTS_B_HOST 1
#include "set_state_natasla.cpp"
#include <cstring>
static int calls = 0;
static void* seen_state = nullptr;
static void* seen_header = nullptr;
static unsigned seen_rate = 0;
static int seen_mode = -1;
static unsigned during = 0;
extern "C" void animAddTransitionTo__FP14AnimationStateP15AnimationHeaderfi(void* state, void* header, float rate, int mode) {
    ++calls;
    seen_state = state;
    seen_header = header;
    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&rate);
    seen_rate = unsigned(bytes[0]) | (unsigned(bytes[1]) << 8) | (unsigned(bytes[2]) << 16) | (unsigned(bytes[3]) << 24);
    seen_mode = mode;
    vslots_b::write_u16(vslots_b::address(static_cast<unsigned char*>(state) - 288u) + 572u, 44u);
    during = 44u;
}
static unsigned half(unsigned char* object) {
    return vslots_b::read_u16(vslots_b::address(object) + 572u);
}
int main() {
    unsigned char object[600];
    unsigned char group[40];
    unsigned char header[4];
    std::memset(object, 0x11, sizeof object);
    std::memset(group, 0, sizeof group);
    vslots_b::host_note(group);
    vslots_b::host_note(header);
    vslots_b::write_word(vslots_b::address(object) + 36u, vslots_b::address_bits(group));
    vslots_b::write_word(vslots_b::address(group) + 28u, vslots_b::address_bits(header));
    vslots_b::write_u16(vslots_b::address(object) + 572u, 99u);
    setState__7Nataslai(object, 0);
    if (calls != 1 || seen_state != object + 288 || seen_header != header || seen_rate != 0x3e999999u || seen_mode != 3) return 1;
    if (half(object) != 0u || during != 44u) return 1;
    if (object[571] != 0x11 || object[574] != 0x11) return 1;

    calls = 0;
    vslots_b::write_u16(vslots_b::address(object) + 572u, 99u);
    const int kept[] = {1, 3, 5, 6, 8, 9, 10};
    for (int state : kept) {
        setState__7Nataslai(object, state);
        if (half(object) != 99u || calls != 0) return 2;
    }
    setState__7Nataslai(object, 2);
    if (half(object) != 2u || calls != 0) return 3;
    setState__7Nataslai(object, 4);
    if (half(object) != 4u) return 4;
    setState__7Nataslai(object, 7);
    if (half(object) != 7u) return 5;
    setState__7Nataslai(object, 11);
    if (half(object) != 11u || calls != 0) return 6;
    setState__7Nataslai(object, -1);
    if (half(object) != 0xffffu) return 7;
    setState__7Nataslai(object, 0x10002);
    if (half(object) != 2u) return 8;
    return 0;
}
