#include "restore_slot.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_restore_slot_condition_view == 33, "size");
    static const unsigned char expected[] = {'!', 's', 't', 'r', 'e', 'a', 'm', 'A', 'l', 'l', 'o', 'c', 'a', 't', 'i', 'o', 'n', '[', 'n', 'm', 'A', 'l', 'l', 'o', 'c', 'a', 't', 'i', 'o', 'n', 's', ']', 0};
    static_assert(sizeof expected == 33, "expected size");
    if (std::memcmp(norrath_sound_restore_slot_condition_view, expected, 33) != 0) return 1;
    std::puts("restore_slot passed");
    return 0;
}
