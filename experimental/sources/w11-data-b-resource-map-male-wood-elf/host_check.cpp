// Host contract for maleWoodElfResourceMap. Includes the source; not an EE plan input.
#include <cstdio>
#include <cstring>
#include "resource_map.cpp"

int main() {
    const ResourceEntry* map = maleWoodElfResourceMap;
    if (sizeof maleWoodElfResourceMap / sizeof maleWoodElfResourceMap[0] != 113) return 1;
    if (std::strcmp(map[0].name, "elf_mwo2.lmp") != 0) return 2;
    if (std::strcmp(map[1].name, "nw_death.anm") != 0) return 3;
    if (std::strcmp(map[111].name, "dw_evade.anm") != 0) return 4;
    if (map[112].name != nullptr || map[112].value != 0) return 5;
    unsigned hash = 2166136261u;
    for (int i = 0; i != 112; ++i) {
        if (map[i].name == nullptr || map[i].value != 0) return 6;
        for (const char* p = map[i].name;; ++p) {
            hash = (hash ^ static_cast<unsigned char>(*p)) * 16777619u;
            if (*p == 0) break;
        }
    }
    if (hash != 0x93dbc61fu) return 7;
    std::printf("maleWoodElfResourceMap contract passed\n");
    return 0;
}
