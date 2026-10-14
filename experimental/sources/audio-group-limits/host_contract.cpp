#include "group_limits.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_group_limits_view == 300, "group limit span");
    for (int index = 0; index < 300; ++index) {
        const unsigned char expected = index == 14 ? 9 : (index == 50 || index == 51) ? 8 : 2;
        if (norrath_sound_group_limits_view[index] != expected) return 1;
    }
    std::puts("group limits passed");
    return 0;
}
