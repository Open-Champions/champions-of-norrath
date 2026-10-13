// Host check for the armor-cache local tables. Not an EE plan input.
#include "armor_cache_locals.cpp"

#include <cstring>

int main() {
    static const int expected[18] = {7, 8, 6, 9, 0, -1, -1, -1, -1, 2, 1, 3, 4, 5, -1, -1, -1, -1};
    for (int i = 0; i < 18; ++i)
        if (norrath_equipment_slot_to_cache[i] != expected[i]) return 1;
    static const char* const names[10] = {"armor(body)", "armor(boots)", "armor(gloves)", "armor(helmet)", "armor(leggings)",
                                          "armor(quiver)", "armor(lefthand)", "armor(righthand)", "armor(righthand2)", "armor(shield)"};
    for (int i = 0; i < 10; ++i)
        if (std::strcmp(norrath_armor_name[i], names[i]) != 0) return 2;
    if (sizeof norrath_equipment_slot_to_cache != 72 || sizeof norrath_armor_name != 10 * sizeof(char*)) return 3;
    return 0;
}
