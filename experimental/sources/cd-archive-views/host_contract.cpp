#include "views.cpp"
#include <cstdio>
#include <cstring>
int main() {
    if (sizeof(norrath_archive_directory_a_view) != 24580u) return 1;
    if (sizeof(norrath_archive_directory_b_view) != 24580u) return 2;
    if (sizeof(norrath_archive_command_view) != 16u) return 3;
    if (sizeof(norrath_archive_completion_pointer_view) != 4u) return 4;
    if (sizeof(norrath_archive_read_mode_view) != 4u) return 5;
    if (sizeof(norrath_archive_cd_command_count_view) != 4u) return 21;
    if (norrath_archive_cd_command_count_view[0] != 0 || norrath_archive_cd_command_count_view[3] != 0) return 22;
    if (norrath_archive_directory_pointer_view != norrath_archive_directory_a_view) return 6;
    if (norrath_archive_directory_switch_view[0] != 0 || norrath_archive_layer_bias_view[0] != 0) return 7;
    if (norrath_archive_read_active_view[0] != 0 || norrath_archive_permanent_halt_flag_view[0] != 0) return 8;
    if (norrath_archive_stream_active_view[0] != 0 || norrath_archive_stream_restart_view[0] != 0) return 9;
    if (std::memcmp(norrath_archive_animation_extension_view, "anm", 4) != 0) return 10;
    if (std::memcmp(norrath_archive_animation_extension_view + 32, "world", 6) != 0) return 11;
    if (std::memcmp(norrath_archive_animation_extension_view + 56, "etex", 5) != 0) return 12;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_archive_check_file_view), "lump.cc") != 0) return 13;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_archive_prefix_five_view), "ntsc_") != 0) return 14;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_archive_prefix_four_view), "pal_") != 0) return 15;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_archive_lookup_format_view), "%s;1") != 0) return 16;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_archive_directory_name_view), "\\BG\\DATA") != 0) return 17;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_archive_load_failure_format_view), "'%s' not found\n") != 0) return 18;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_archive_zero_record_format_view),
                    "!!!!!!!!!! %s is fucked\n") != 0) return 19;
    unsigned count = 0xffffffffu;
    std::memcpy(&count, norrath_archive_directory_a_view + 24576, 4);
    if (count != 0u) return 20;
    std::puts("archive views contract passed");
    return 0;
}
