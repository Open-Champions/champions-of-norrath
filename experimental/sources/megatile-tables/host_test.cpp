#include "tables.hpp"

#include <cstdio>
#include <cstring>
#include <string>

namespace {
int failures = 0;

void expect(const char* name, bool condition, const std::string& detail) {
    if (!condition) {
        std::fprintf(stderr, "FAIL %s: %s\n", name, detail.c_str());
        failures += 1;
    }
}

bool all_zero(const unsigned char* bytes, unsigned size) {
    for (unsigned index = 0; index < size; index += 1) {
        if (bytes[index] != 0) return false;
    }
    return true;
}

void expect_text(const char* name, const unsigned char* bytes, const char* text) {
    expect(name, std::strcmp(reinterpret_cast<const char*>(bytes), text) == 0, text);
}

void expect_bytes(const char* name, const unsigned char* bytes, const unsigned char* expected, unsigned size) {
    expect(name, std::memcmp(bytes, expected, size) == 0, "bytes");
}
}

int main() {
    expect("regions", sizeof norrath_megatile_regions_view == 0x140 &&
                          all_zero(norrath_megatile_regions_view, 0x140), "320 zero bytes");
    expect("x", sizeof norrath_megatile_x_view == 0x400 && all_zero(norrath_megatile_x_view, 0x400), "1024 zero bytes");
    expect("y", sizeof norrath_megatile_y_view == 0x400 && all_zero(norrath_megatile_y_view, 0x400), "1024 zero bytes");
    expect("grid", sizeof norrath_megatile_grid_origin_view == 0x18 &&
                       all_zero(norrath_megatile_grid_origin_view, 0x18), "24 zero bytes");
    expect("pm pointers", sizeof norrath_megatile_pm_pointers_view == 0x400 &&
                              all_zero(norrath_megatile_pm_pointers_view, 0x400), "1024 zero bytes");
    expect("pm characters", sizeof norrath_megatile_pm_characters_view == 0x1000 &&
                                all_zero(norrath_megatile_pm_characters_view, 0x1000), "4096 zero bytes");
    expect("dynlevels", sizeof norrath_megatile_dynlevels_view == 0x1c0 &&
                            all_zero(norrath_megatile_dynlevels_view, 0x1c0), "448 zero bytes");
    expect("level name", sizeof norrath_megatile_level_name_view == 0x142 &&
                             all_zero(norrath_megatile_level_name_view, 0x142), "322 zero bytes");
    expect("counters", sizeof norrath_megatile_counters_view == 20 &&
                           all_zero(norrath_megatile_counters_view, 20), "five zero words");
    expect("world", sizeof norrath_megatile_world_word == 4 && all_zero(norrath_megatile_world_word, 4),
           "zero word");

    const unsigned char scale[] = {0x60, 0x0b, 0xb6, 0x3a};
    const unsigned char closest[] = {0xeb, 0x78, 0xad, 0x60};
    expect_bytes("scale", norrath_megatile_scale_view, scale, 4);
    expect_bytes("closest", norrath_megatile_closest_limit_view, closest, 4);

    expect_text("name a", norrath_megatile_name_a_view, "zzTileType");
    expect_text("name b", norrath_megatile_name_b_view, "EdgeMarker");
    expect_text("name c", norrath_megatile_name_c_view, "Directional_Light");
    expect_text("name d", norrath_megatile_name_d_view, "Ambient_Light");
    expect_text("name e", norrath_megatile_name_e_view, "clearColor");
    expect_text("name f", norrath_megatile_name_f_view, "customVars");
    expect_text("pm name", norrath_megatile_pm_name_view, "Line");
    expect_text("pm a", norrath_megatile_pm_a_view, "name=barrier");
    expect_text("pm b", norrath_megatile_pm_b_view, "continous=1");
    expect_text("pm c", norrath_megatile_pm_c_view, "norotate=1");
    expect_text("count format", norrath_megatile_count_format_view, "pcount=-%d");
    expect_text("point format", norrath_megatile_point_format_view, "p=%f,%f,0");
    expect_text("trigger", norrath_megatile_trigger_text, "trigger");
    expect_text("width key", norrath_megatile_width_key, "w=");
    expect_text("height key", norrath_megatile_height_key, "h=");
    expect_text("bang", norrath_megatile_bang_text, "!!!");
    expect_text("new world", norrath_megatile_dynamic_new_world_text, "!!!dynamicnewworld");
    expect_text("scan pair", norrath_megatile_scan_pair_format, "%[^,],%d");
    expect_text("suffix", norrath_megatile_suffix_number_format, "%s%d");
    expect_text("level assignment", norrath_megatile_level_assignment_format, "level=%s");
    expect_text("shift", norrath_megatile_dynamic_shift_text, "!!!dynamicshift");
    expect_text("pair join", norrath_megatile_pair_join_format, "%s_%s");
    expect_text("decimal", norrath_megatile_decimal_format, "%d");
    expect_text("orientation text", norrath_megatile_dynamic_orientation_text, "!!!dynamicorientation");
    expect_text("orientation format", norrath_megatile_orientation_format, "orientation=%d");
    expect_text("closest text", norrath_megatile_closest_level_text, "!!!closestdynamiclevel");
    expect_text("from", norrath_megatile_from_format, "from=%s");
    expect_text("width format", norrath_megatile_width_format, "w=%d");
    expect_text("height format", norrath_megatile_height_format, "h=%d");
    expect_text("mspawn", norrath_megatile_mspawn_text, "MSpawn");
    expect_text("push", norrath_megatile_push_trigger_text, "PushTrigger");
    expect_text("kitten", norrath_megatile_kitten_text, "kitten");
    expect_text("kitten format", norrath_megatile_kitten_name_format, "name=kitten%d");

    if (failures != 0) {
        std::fprintf(stderr, "%d failure(s)\n", failures);
        return 1;
    }
    std::printf("megatile table host checks passed\n");
    return 0;
}
