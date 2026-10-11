#include "master_volume.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_master_volume_view == 4, "volume word");
    static_assert(sizeof norrath_sound_master_scale_view == 4, "scale word");
    for (unsigned char byte : norrath_sound_master_volume_view)
        if (byte != 0) return 1;
    const unsigned scale = norrath_sound_master_scale_view[0]
        | (norrath_sound_master_scale_view[1] << 8)
        | (norrath_sound_master_scale_view[2] << 16)
        | (norrath_sound_master_scale_view[3] << 24);
    if (scale != 0x467ffc00u) return 2;
    std::puts("master volume passed");
    return 0;
}
