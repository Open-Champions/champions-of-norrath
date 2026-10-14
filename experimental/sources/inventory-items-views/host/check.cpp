#include <cstring>
extern "C" const unsigned char norrath_inventory_ability_sound_lump_view[8];
extern "C" const unsigned char norrath_inventory_ability_sound_resource_view[19];
extern "C" const unsigned char norrath_inventory_reset_resource_names_view[96];
int main() {
    if (std::memcmp(norrath_inventory_ability_sound_lump_view, "fx.lmp\0", 8) != 0) return 1;
    if (std::memcmp(norrath_inventory_ability_sound_resource_view, "menu_char_incr.anm", 19) != 0) return 2;
    static const char* const names[6] = {"dhs_idled.anm", "dha_idled.anm", "l_idled.anm", "b_idled.anm", "dw_idled.anm", "ohw_idled.anm"};
    for (int slot = 0; slot < 6; ++slot) {
        const unsigned char* p = norrath_inventory_reset_resource_names_view + slot * 16;
        if (std::strcmp(reinterpret_cast<const char*>(p), names[slot]) != 0) return 10 + slot;
        for (int i = int(std::strlen(names[slot])); i < 16; ++i) if (p[i] != 0) return 20 + slot;
    }
    return 0;
}
