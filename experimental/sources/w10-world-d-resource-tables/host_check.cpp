// Host contract for skeletonResMap and typeToResource. Not an EE plan input.
#include <cstdio>
#include <cstring>
struct ResourceEntry { const char* name; int value; };
extern "C" {
unsigned char maleBarbResourceMap[1], femBarbResourceMap[1], maleWoodElfResourceMap[1], femaleWoodElfResourceMap[1],
    maleHighElfResourceMap[1], femaleHighElfResourceMap[1], maleEruditeResourceMap[1], femaleEruditeResourceMap[1],
    maleDarkElfResourceMap[1], femaleDarkElfResourceMap[1];
extern ResourceEntry skeletonResMap[100];
extern void* typeToResource[10];
}
int main() {
    if (std::strcmp(skeletonResMap[0].name, "skelcom.lmp") != 0) return 1;
    if (std::strcmp(skeletonResMap[1].name, "1_ataka.anm") != 0) return 2;
    if (std::strcmp(skeletonResMap[77].name, "1_axe.vif") != 0) return 3;
    if (std::strcmp(skeletonResMap[98].name, "spear2.tex") != 0) return 4;
    if (skeletonResMap[99].name != nullptr || skeletonResMap[99].value != 0) return 5;
    for (int i = 0; i != 99; ++i) if (skeletonResMap[i].name == nullptr || skeletonResMap[i].value != 0) return 6;
    if (typeToResource[0] != maleBarbResourceMap || typeToResource[1] != femBarbResourceMap) return 7;
    if (typeToResource[8] != maleDarkElfResourceMap || typeToResource[9] != femaleDarkElfResourceMap) return 8;
    std::printf("resource tables contract passed\n");
    return 0;
}
