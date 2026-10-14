// Host check for the a.1122 storage unit. Not a build-plan input.
//   c++ -std=c++17 -o check host_check.cpp && ./check
// The object is four zero ints (16 bytes, spacing of the image's labels) and is addressable per player.
#include <cstdio>
#include "quick_spell_alpha.cpp"
int main() {
    int bad = 0;
    if (sizeof norrath_quick_spell_alpha != 16) { std::puts("size"); ++bad; }
    for (int v : norrath_quick_spell_alpha) if (v != 0) { std::puts("not zero"); ++bad; }
    norrath_quick_spell_alpha[3] = 128;
    if (norrath_quick_spell_alpha[3] != 128 || norrath_quick_spell_alpha[2] != 0) { std::puts("index"); ++bad; }
    return bad;
}
