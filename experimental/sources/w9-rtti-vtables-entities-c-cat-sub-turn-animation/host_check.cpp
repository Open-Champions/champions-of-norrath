// Host check only. Not an EE build input.
#define NORRATH_RTTI_C_HOST 1
#include "sub_turn.cpp"
#include <cstdio>
#include <cstring>
int main() {
    int bad = 0;
    unsigned char object[512];
    unsigned char before[512];
    for (unsigned i = 0; i < sizeof object; ++i) before[i] = object[i] = static_cast<unsigned char>(i * 7 + 2);
    subTurnAnimation__3Cat(object);
    for (unsigned i = 0; i < sizeof object; ++i) {
        const bool in_store = (i >= 404 && i < 408) || (i >= 372 && i < 376);
        if (in_store) continue;
        if (object[i] != before[i]) { std::printf("changed byte %u\n", i); ++bad; }
    }
    if (rtti_c::read_word(rtti_c::address(object) + 404u) != 0xbd088888u) { std::printf("404\n"); ++bad; }
    if (rtti_c::read_word(rtti_c::address(object) + 372u) != 0xbd088888u) { std::printf("372\n"); ++bad; }
    std::printf(bad ? "failed\n" : "subTurnAnimation__3Cat passed\n");
    return bad != 0;
}
