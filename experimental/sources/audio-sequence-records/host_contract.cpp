#include "sequence_records.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_sequence_records_view == 32u * 76u, "32 records");
    for (unsigned char byte : norrath_sound_sequence_records_view)
        if (byte != 0) return 1;
    std::puts("sequence records passed");
    return 0;
}
