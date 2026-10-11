#include "sequence_spinner.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_sequence_id_spinner_view == 2, "size");
    static const unsigned char expected[] = {0, 0};
    static_assert(sizeof expected == 2, "expected size");
    if (std::memcmp(norrath_sound_sequence_id_spinner_view, expected, 2) != 0) return 1;
    std::puts("sequence_spinner passed");
    return 0;
}
