#include "voice_map.cpp"

#include <cstdio>

int main() {
    static_assert(sizeof norrath_sound_channel_voice_map_view == 86, "43 pairs");
    for (int index = 0; index < 43; ++index) {
        const unsigned bank = index < 24 ? 0u : 1u;
        const unsigned voice = index < 24 ? static_cast<unsigned>(index) : static_cast<unsigned>(index - 24);
        if (norrath_sound_channel_voice_map_view[index * 2] != bank) return 1;
        if (norrath_sound_channel_voice_map_view[index * 2 + 1] != voice) return 2;
    }
    std::puts("channel voice map passed");
    return 0;
}
