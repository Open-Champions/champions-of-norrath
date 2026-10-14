// Host contract for innoruukResourceMap. Not an EE plan input.
#include <cstring>
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" ResourceSlot innoruukResourceMap[29];
int main() {
    if (sizeof(innoruukResourceMap) != 29u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 29u; ++i)
        if (innoruukResourceMap[i].unused != 0u) return 2;
    if (innoruukResourceMap[28].name != nullptr) return 3;
    for (unsigned i = 0; i < 28u; ++i)
        if (innoruukResourceMap[i].name == nullptr || innoruukResourceMap[i].name[0] == 0) return 4;
    if (std::strcmp(innoruukResourceMap[0].name, "innoruuk.lmp") != 0) return 5;
    if (std::strcmp(innoruukResourceMap[1].name, "innoruuk.vif") != 0) return 5;
    if (std::strcmp(innoruukResourceMap[14].name, "idle2.anm") != 0) return 5;
    if (std::strcmp(innoruukResourceMap[27].name, "spel_innoruuk_summon.anm") != 0) return 5;
    unsigned hash = 0x811c9dc5u;
    for (unsigned i = 0; i < 28u; ++i) {
        for (const char* p = innoruukResourceMap[i].name; *p; ++p) hash = (hash ^ static_cast<unsigned char>(*p)) * 0x01000193u;
        hash = (hash ^ 10u) * 0x01000193u;
    }
    if (hash != 3836660659u) return 6;
    return 0;
}
