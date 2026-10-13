// Host contract for femaleDarkElfResourceMap. Not an EE plan input.
#include <cstring>
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" ResourceSlot femaleDarkElfResourceMap[113];
int main() {
    if (sizeof(femaleDarkElfResourceMap) != 113u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 113u; ++i)
        if (femaleDarkElfResourceMap[i].unused != 0u) return 2;
    if (femaleDarkElfResourceMap[112].name != nullptr) return 3;
    for (unsigned i = 0; i < 112u; ++i)
        if (femaleDarkElfResourceMap[i].name == nullptr || femaleDarkElfResourceMap[i].name[0] == 0) return 4;
    if (std::strcmp(femaleDarkElfResourceMap[0].name, "elf_fdk2.lmp") != 0) return 5;
    if (std::strcmp(femaleDarkElfResourceMap[1].name, "nw_death.anm") != 0) return 5;
    if (std::strcmp(femaleDarkElfResourceMap[56].name, "l_blockhit.anm") != 0) return 5;
    if (std::strcmp(femaleDarkElfResourceMap[111].name, "dw_evade.anm") != 0) return 5;
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 112u; ++i) {
        for (const char* p = femaleDarkElfResourceMap[i].name; *p; ++p) hash = (hash ^ static_cast<unsigned char>(*p)) * 0x01000193u;
        hash = (hash ^ 10u) * 0x01000193u;
    }
    if (hash != 841136935u) return 6;
    if (femaleDarkElfResourceMap[4].name != femaleDarkElfResourceMap[5].name) return 7;
    return 0;
}
