// Host contract for the initialised data objects. Not an EE plan input.
#include <cstdio>
#include <cstring>
#include "data_tables.cpp"
int main() {
    const int want[10] = {128, 9920, 8000, 18496, 28640, 44512, 63168, 98368, 98368, 15360};
    for (int i = 0; i != 10; ++i) if (effectModeSize[i] != want[i]) return 1;
    unsigned sum = 0;
    for (unsigned i = 0; i != 16; ++i) sum += defaultHalos[i];
    if (sum != 0x0a + 0x0a + 0x44 + 0x40 + 0x80 + 0x72 + 0x44 + 0x28 + 0x28 + 0x44 + 0x48 + 0xf4 + 0x43) return 2;
    const char* names[13] = {"fx.lmp", "blood.vif", "blood.tex", "blood_yellow.vif", "blood_yellow.tex", "bloodpool.anm",
                             "bloodsplat1.vif", "bloodsplat1.tex", "bloodsplat2.vif", "bloodsplat2.tex",
                             "gutchunks.vif", "gutchunks.tex", "gutchunks_yellow.tex"};
    for (int i = 0; i != 13; ++i) {
        if (fxResourceMap[2 * i] == nullptr || std::strcmp(fxResourceMap[2 * i], names[i]) != 0) return 3;
        if (fxResourceMap[2 * i + 1] != nullptr) return 4;
    }
    if (fxResourceMap[26] != nullptr || fxResourceMap[27] != nullptr) return 5;
    std::puts("data-tables contract passed");
    return 0;
}
