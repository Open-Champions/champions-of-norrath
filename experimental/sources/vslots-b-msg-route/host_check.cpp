#define NORRATH_VSLOTS_B_HOST 1
#include "msg_route.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
static unsigned word(const unsigned char* object, unsigned offset) {
    return vslots_b::read_word(vslots_b::address(object) + offset);
}
int main() {
    unsigned char object[800];
    std::memset(object, 0x5a, sizeof object);
    vslots_b::write_word(vslots_b::address(object) + 708u, 0x11111111u);
    vslots_b::write_word(vslots_b::address(object) + 716u, 0x22222222u);
    vslots_b::write_word(vslots_b::address(object) + 580u, 0x33333333u);
    vslots_b::write_word(vslots_b::address(object) + 584u, 0x44444444u);
    msg_route__8Creaturei(object, 4);
    expect(word(object, 708u) == 0u, "nonzero route clears 708");
    expect(word(object, 716u) == 0u, "nonzero route clears 716");
    expect(word(object, 580u) == 0x33333333u, "nonzero route leaves 580");
    expect(word(object, 584u) == 0x44444444u, "nonzero route leaves 584");
    expect(object[707] == 0x5a && object[712] == 0x5a && object[720] == 0x5a, "neighbors stay");

    std::memset(object, 0xa5, sizeof object);
    vslots_b::write_word(vslots_b::address(object) + 716u, 0xffffffffu);
    msg_route__8Creaturei(object, 0);
    expect(word(object, 708u) == 0u, "zero route clears 708");
    expect(word(object, 580u) == 0xffffffffu, "zero route stores -1 at 580");
    expect(word(object, 584u) == 0xffffffffu, "zero route stores -1 at 584");
    expect(word(object, 716u) == 0u, "zero route wraps the counter");
    expect(object[576] == 0xa5 && object[588] == 0xa5, "zero-route neighbors stay");
    return failures ? 1 : 0;
}
