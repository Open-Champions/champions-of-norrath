#include "find_channel.cpp"
#include <cstdio>
extern "C" int nextChannel = 0;
extern "C" unsigned char norrath_sound_channel_records_view[43 * 20 + 4] = {};
static void flags(int i, unsigned v) {
    norrath_sound_channel_records_view[i * 20 + 4] = v & 255;
    norrath_sound_channel_records_view[i * 20 + 5] = v >> 8;
}
int main() {
    if (findChannel__Fv() != 1 || nextChannel != 1) return 1;
    flags(2, 1); flags(3, 4); flags(4, 0x100);   // 0x100 is not a tested bit
    if (findChannel__Fv() != 4 || nextChannel != 4) return 2;
    flags(5, 2);                                  // bit 1 is not tested either
    if (findChannel__Fv() != 5) return 3;
    nextChannel = 41;
    if (findChannel__Fv() != 42 || nextChannel != 42) return 4;
    if (findChannel__Fv() != 0 || nextChannel != 0) return 5;   // wraps past 42
    for (int i = 0; i < 43; ++i) flags(i, 4);
    nextChannel = 10;
    if (findChannel__Fv() != -1) return 6;
    if (nextChannel != (10 + 44) % 43) return 7;
    flags(11, 0x8000);
    nextChannel = 10;
    if (findChannel__Fv() != 11) return 8;
    std::puts("find_channel passed");
    return 0;
}
