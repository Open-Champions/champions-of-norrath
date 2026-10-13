// Host comparison only. Not an EE build input.
// Build: c++ -std=c++17 -Wall -Wextra -Werror host_check.cpp (includes the sources).
#include "norrath_front_a_view_4848fc.cpp"
#include "norrath_front_a_view_484900.cpp"
#include "norrath_front_a_view_483f34.cpp"
#include "norrath_front_a_view_484b08.cpp"
#include "norrath_front_a_view_4858e0.cpp"
#include "norrath_front_a_view_6ca800.cpp"
#include "norrath_front_a_view_8d5b58.cpp"
#include "norrath_front_a_view_8d5fd0.cpp"

#include <cstdio>
#include <cstring>

int main() {
    int bad = 0;
    auto check = [&](bool ok, const char* what) { if (!ok) { std::printf("FAIL %s\n", what); ++bad; } };
    check(norrath_front_a_view_4848fc == 0 && sizeof norrath_front_a_view_4848fc == 4, "arrowLeft");
    check(norrath_front_a_view_484900 == 0 && sizeof norrath_front_a_view_484900 == 4, "arrowRight");
    check(norrath_front_a_view_483f34 == 0 && sizeof norrath_front_a_view_483f34 == 4, "charAnimTest");
    unsigned bits;
    std::memcpy(&bits, &norrath_front_a_view_484b08, 4);
    check(bits == 0 && sizeof norrath_front_a_view_484b08 == 4, "bustAlpha");
    check(std::strcmp(norrath_front_a_view_4858e0.text, "oneshot=1") == 0 && norrath_front_a_view_4858e0.unused == 0, "batParams");
    check(sizeof norrath_front_a_view_6ca800 == 1568 && norrath_front_a_view_6ca800[1552] == 0, "emergencyResult");
    check(sizeof norrath_front_a_view_8d5b58 == 16, "beardSetting");
    check(sizeof norrath_front_a_view_8d5fd0 == 16, "endPos");
    if (bad == 0) std::puts("front-a views host check passed");
    return bad != 0;
}
