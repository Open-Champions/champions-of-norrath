#include <cstdio>
#include <cstring>
#include <cstdint>
#include "data_views.cpp"
int main() {
    if (padFilter != 0 || reglistLoopCount != 0) return 1;
    if (sizeof rdata != 128 || sizeof sceDmaCurrentEnv != 80) return 2;
    if ((reinterpret_cast<std::uintptr_t>(rdata) & 63) != 0 || (reinterpret_cast<std::uintptr_t>(sceDmaCurrentEnv) & 15) != 0) return 3;
    for (unsigned char b : rdata) if (b) return 4;
    for (unsigned char b : sceDmaCurrentEnv) if (b) return 5;
    if (sizeof norrath_sce_d_race_strings_view_4293e8 != 10 * sizeof(char*)) return 6;
    if (std::strcmp(norrath_sce_d_race_strings_view_4293e8[0], "RaceBarbarianMale") != 0) return 7;
    if (std::strcmp(norrath_sce_d_race_strings_view_4293e8[3], "RaceWoodElfFemale") != 0) return 8;
    if (std::strcmp(norrath_sce_d_race_strings_view_4293e8[9], "RaceDarkElfFemale") != 0) return 9;
    char word[4];
    std::memcpy(word, &norrath_sce_d_unknown_sdata_word_view_4852d8, 4);
    if (std::memcmp(word, "Colo", 4) != 0) return 10;
    std::puts("data views contract passed");
    return 0;
}
