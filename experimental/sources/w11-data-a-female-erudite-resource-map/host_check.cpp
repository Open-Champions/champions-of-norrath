// Host contract for femaleEruditeResourceMap. Not an EE plan input.
#include <cstring>
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" ResourceSlot femaleEruditeResourceMap[113];
int main() {
    if (sizeof(femaleEruditeResourceMap) != 113u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 113u; ++i)
        if (femaleEruditeResourceMap[i].unused != 0u) return 2;
    if (femaleEruditeResourceMap[112].name != nullptr) return 3;
    for (unsigned i = 0; i < 112u; ++i)
        if (femaleEruditeResourceMap[i].name == nullptr || femaleEruditeResourceMap[i].name[0] == 0) return 4;
    if (std::strcmp(femaleEruditeResourceMap[0].name, "eru_fem.lmp") != 0) return 5;
    if (std::strcmp(femaleEruditeResourceMap[1].name, "nw_death.anm") != 0) return 5;
    if (std::strcmp(femaleEruditeResourceMap[56].name, "l_blockhit.anm") != 0) return 5;
    if (std::strcmp(femaleEruditeResourceMap[111].name, "ohw_idlea.anm") != 0) return 5;
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 112u; ++i) {
        for (const char* p = femaleEruditeResourceMap[i].name; *p; ++p) hash = (hash ^ static_cast<unsigned char>(*p)) * 0x01000193u;
        hash = (hash ^ 10u) * 0x01000193u;
    }
    if (hash != 2018398000u) return 6;
    if (femaleEruditeResourceMap[2].name != femaleEruditeResourceMap[3].name) return 7;
    return 0;
}
