// Host contract for femaleHighElfResourceMap. Not an EE plan input.
#include <cstring>
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" ResourceSlot femaleHighElfResourceMap[113];
int main() {
    if (sizeof(femaleHighElfResourceMap) != 113u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 113u; ++i)
        if (femaleHighElfResourceMap[i].unused != 0u) return 2;
    if (femaleHighElfResourceMap[112].name != nullptr) return 3;
    for (unsigned i = 0; i < 112u; ++i)
        if (femaleHighElfResourceMap[i].name == nullptr || femaleHighElfResourceMap[i].name[0] == 0) return 4;
    if (std::strcmp(femaleHighElfResourceMap[0].name, "elf_fhi2.lmp") != 0) return 5;
    if (std::strcmp(femaleHighElfResourceMap[1].name, "nw_death.anm") != 0) return 5;
    if (std::strcmp(femaleHighElfResourceMap[56].name, "ohw_idlea.anm") != 0) return 5;
    if (std::strcmp(femaleHighElfResourceMap[111].name, "ohw_idlea.anm") != 0) return 5;
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 112u; ++i) {
        for (const char* p = femaleHighElfResourceMap[i].name; *p; ++p) hash = (hash ^ static_cast<unsigned char>(*p)) * 0x01000193u;
        hash = (hash ^ 10u) * 0x01000193u;
    }
    if (hash != 4211494250u) return 6;
    if (femaleHighElfResourceMap[4].name != femaleHighElfResourceMap[5].name) return 7;
    return 0;
}
