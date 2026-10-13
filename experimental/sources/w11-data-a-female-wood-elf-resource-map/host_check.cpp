// Host contract for femaleWoodElfResourceMap. Not an EE plan input.
#include <cstring>
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" ResourceSlot femaleWoodElfResourceMap[113];
int main() {
    if (sizeof(femaleWoodElfResourceMap) != 113u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 113u; ++i)
        if (femaleWoodElfResourceMap[i].unused != 0u) return 2;
    if (femaleWoodElfResourceMap[112].name != nullptr) return 3;
    for (unsigned i = 0; i < 112u; ++i)
        if (femaleWoodElfResourceMap[i].name == nullptr || femaleWoodElfResourceMap[i].name[0] == 0) return 4;
    if (std::strcmp(femaleWoodElfResourceMap[0].name, "elf_fwo2.lmp") != 0) return 5;
    if (std::strcmp(femaleWoodElfResourceMap[1].name, "nw_death.anm") != 0) return 5;
    if (std::strcmp(femaleWoodElfResourceMap[56].name, "ohw_idlea.anm") != 0) return 5;
    if (std::strcmp(femaleWoodElfResourceMap[111].name, "dw_evade.anm") != 0) return 5;
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 112u; ++i) {
        for (const char* p = femaleWoodElfResourceMap[i].name; *p; ++p) hash = (hash ^ static_cast<unsigned char>(*p)) * 0x01000193u;
        hash = (hash ^ 10u) * 0x01000193u;
    }
    if (hash != 3104583018u) return 6;
    if (femaleWoodElfResourceMap[2].name != femaleWoodElfResourceMap[3].name) return 7;
    return 0;
}
