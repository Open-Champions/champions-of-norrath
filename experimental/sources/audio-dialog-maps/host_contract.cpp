#include "dialog_maps.cpp"

#include <cstdio>
#include <cstring>

int main() {
    static_assert(sizeof norrath_sound_global_dialog_map_view == 12, "three map words");
    static_assert(sizeof norrath_sound_global_dialog_name_view == 10, "mouth.lmp");
    static_assert(sizeof norrath_sound_global_dialog_extension_view == 11, "dialog.bin");
    for (unsigned char byte : norrath_sound_global_dialog_map_view)
        if (byte != 0) return 1;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_sound_global_dialog_name_view), "mouth.lmp") != 0)
        return 2;
    if (std::strcmp(reinterpret_cast<const char*>(norrath_sound_global_dialog_extension_view), "dialog.bin") != 0)
        return 3;
    std::puts("dialog maps passed");
    return 0;
}
