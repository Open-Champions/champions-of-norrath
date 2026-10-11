#include "channel_records.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_channel_records_view == 43u * 20u, "43 records");
    for (unsigned char byte : norrath_sound_channel_records_view)
        if (byte != 0) return 1;
    std::puts("channel records passed");
    return 0;
}
