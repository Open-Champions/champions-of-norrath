// Host contract for femBarbResourceMap. Not an EE plan input.
#include <cstring>
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" ResourceSlot femBarbResourceMap[113];
int main() {
    if (sizeof(femBarbResourceMap) != 113u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 113u; ++i)
        if (femBarbResourceMap[i].unused != 0u) return 2;
    if (femBarbResourceMap[112].name != nullptr) return 3;
    for (unsigned i = 0; i < 112u; ++i)
        if (femBarbResourceMap[i].name == nullptr || femBarbResourceMap[i].name[0] == 0) return 4;
    if (std::strcmp(femBarbResourceMap[0].name, "sonya2.lmp") != 0) return 5;
    if (std::strcmp(femBarbResourceMap[1].name, "nw_death.anm") != 0) return 5;
    if (std::strcmp(femBarbResourceMap[56].name, "l_blockhit.anm") != 0) return 5;
    if (std::strcmp(femBarbResourceMap[111].name, "dw_evade.anm") != 0) return 5;
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 112u; ++i) {
        for (const char* p = femBarbResourceMap[i].name; *p; ++p) hash = (hash ^ static_cast<unsigned char>(*p)) * 0x01000193u;
        hash = (hash ^ 10u) * 0x01000193u;
    }
    if (hash != 1298637001u) return 6;
    if (femBarbResourceMap[4].name != femBarbResourceMap[5].name) return 7;
    return 0;
}
