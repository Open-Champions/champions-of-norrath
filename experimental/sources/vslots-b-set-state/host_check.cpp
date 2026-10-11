#define NORRATH_VSLOTS_B_HOST 1
#include "set_state.cpp"
#include <cstdio>
#include <cstring>
static int failures = 0;
static void expect(bool ok, const char* message) {
    if (!ok) { std::fprintf(stderr, "fail: %s\n", message); ++failures; }
}
int main() {
    unsigned char object[580];
    std::memset(object, 0x7e, sizeof object);
    setState__8Creaturei(object, 0x1234abcd);
    expect(object[572] == 0xcd && object[573] == 0xab, "low half is stored");
    expect(object[571] == 0x7e && object[574] == 0x7e, "neighbors stay");
    setState__8Creaturei(object, -1);
    expect(object[572] == 0xff && object[573] == 0xff, "negative state stores 0xffff");
    return failures ? 1 : 0;
}
