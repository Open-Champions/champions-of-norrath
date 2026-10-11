#include "floor_storage.cpp"

#include <cstdio>
#include <cstring>

static int zeros(const unsigned char* bytes, unsigned count) {
    for (unsigned i = 0; i < count; ++i)
        if (bytes[i] != 0) return 1;
    return 0;
}

int main() {
    if (sizeof norrath_floor_blood_level_word_view != 4) return 1;
    if (sizeof norrath_floor_sprite_count_word_view != 4) return 2;
    if (sizeof norrath_floor_fade_packet_word_view != 4) return 3;
    if (sizeof norrath_floor_free_list_word_view != 4) return 4;
    if (sizeof norrath_floor_light_interface_mode_word != 4) return 5;
    if (sizeof norrath_floor_light_scene_view != 56) return 6;
    if (sizeof norrath_floor_expand_list_view != 320) return 7;
    if (sizeof norrath_floor_sprite_grid_view != 3600) return 8;
    if (sizeof norrath_floor_sprite_packet_view != 98304) return 9;
    if (sizeof norrath_floor_light_grid_view != 32400) return 10;
    if (sizeof norrath_floor_sprite_texture_map != 16) return 11;
    if (sizeof norrath_floor_sprite_texture_ready_word != 4) return 12;
    if (zeros(norrath_floor_blood_level_word_view, 4)) return 13;
    if (zeros(norrath_floor_sprite_count_word_view, 4)) return 14;
    if (zeros(norrath_floor_fade_packet_word_view, 4)) return 15;
    if (zeros(norrath_floor_free_list_word_view, 4)) return 16;
    if (zeros(norrath_floor_light_interface_mode_word, 4)) return 17;
    if (zeros(norrath_floor_light_scene_view, 56)) return 18;
    if (zeros(norrath_floor_expand_list_view, 320)) return 19;
    if (zeros(norrath_floor_sprite_grid_view, 3600)) return 20;
    if (zeros(norrath_floor_sprite_packet_view, 98304)) return 21;
    if (zeros(norrath_floor_light_grid_view, 32400)) return 22;
    if (zeros(norrath_floor_sprite_texture_map, 16)) return 23;
    if (zeros(norrath_floor_sprite_texture_ready_word, 4)) return 24;
    if (floorDrawEnable != 1) return 25;
    if (std::strcmp(norrath_floor_sprite_texture_group, "engine.lmp") != 0) return 26;
    if (std::strcmp(norrath_floor_sprite_texture_name, "floorsprite.tex") != 0) return 27;
    std::puts("floor storage layout passed");
    return 0;
}
