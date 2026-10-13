// Host check for IsEventModifier. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
// The table is defined here with the observed values; every halfword from 0 to 0x1ff is tried, with
// the high byte varying so that a byte-wide compare would be caught.
#include <cstdio>
extern "C" {
unsigned char ModifierCodes[8] = {0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7};
}
#include "is_event_modifier.cpp"
int main() {
    int bad = 0;
    for (unsigned v = 0; v < 0x20000; v += (v < 0x200 ? 1 : 0x101)) {
        unsigned char event[16] = {9, 9, 9, 9, static_cast<unsigned char>(v & 255), static_cast<unsigned char>((v >> 8) & 255), 9, 9};
        const int want = v >= 0xe0 && v <= 0xe7;
        if (v > 0xffff) break;
        if (IsEventModifier(event) != want) { std::printf("value %x\n", v); ++bad; }
    }
    ModifierCodes[7] = 0x10;   // the table, not a constant range, decides
    unsigned char event[16] = {0, 0, 0, 0, 0x10, 0, 0, 0};
    if (IsEventModifier(event) != 1) ++bad;
    event[4] = 0xe7;
    if (IsEventModifier(event) != 0) ++bad;
    return bad;
}
