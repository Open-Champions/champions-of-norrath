// Host contract for udkResourceMap. Not an EE plan input.
#include <cstring>
struct ResourceSlot {
    char* name;
    unsigned unused;
};
extern "C" ResourceSlot udkResourceMap[27];
int main() {
    if (sizeof(udkResourceMap) != 27u * sizeof(ResourceSlot)) return 1;
    for (unsigned i = 0; i < 27u; ++i)
        if (udkResourceMap[i].unused != 0u) return 2;
    if (udkResourceMap[26].name != nullptr) return 3;
    if (std::strcmp(udkResourceMap[0].name, "udk.lmp") != 0) return 5;
    if (std::strcmp(udkResourceMap[1].name, "udk.vif") != 0) return 5;
    if (std::strcmp(udkResourceMap[2].name, "udk.tex") != 0) return 5;
    if (std::strcmp(udkResourceMap[3].name, "attak1.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[4].name, "attak2.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[5].name, "attak3.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[6].name, "attak4.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[7].name, "block.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[8].name, "conjure.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[9].name, "hitreaction.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[10].name, "idlea.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[11].name, "idleb.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[12].name, "taunt.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[13].name, "walk.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[14].name, "walkatak.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[15].name, "death.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[16].name, "hh_atak1.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[17].name, "hh_atak2.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[18].name, "hh_atak3.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[19].name, "hh_rearup.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[20].name, "hh_run.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[21].name, "hh_trot.anm") != 0) return 5;
    if (std::strcmp(udkResourceMap[22].name, "sword.vif") != 0) return 5;
    if (std::strcmp(udkResourceMap[23].name, "sword.tex") != 0) return 5;
    if (std::strcmp(udkResourceMap[24].name, "shield.vif") != 0) return 5;
    if (std::strcmp(udkResourceMap[25].name, "shield.tex") != 0) return 5;
    return 0;
}
