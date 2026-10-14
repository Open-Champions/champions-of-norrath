#include "restore_allocation.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_restore_allocation_condition_view == 7, "size");
    static const unsigned char expected[] = {'b', 'u', 'f', 'f', 'e', 'r', 0};
    static_assert(sizeof expected == 7, "expected size");
    if (std::memcmp(norrath_sound_restore_allocation_condition_view, expected, 7) != 0) return 1;
    std::puts("restore_allocation passed");
    return 0;
}
