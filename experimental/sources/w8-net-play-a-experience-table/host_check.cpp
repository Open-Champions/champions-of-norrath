#include "experience_table.cpp"
int main() {
    if (sizeof(experienceTable) != 204) return 1;
    if (experienceTable[0] != 0 || experienceTable[50] != 99999999) return 2;
    for (int i = 1; i < 51; ++i) if (experienceTable[i] < experienceTable[i - 1]) return 3;
    // Spot values read from the image.
    if (experienceTable[1] != 1100 || experienceTable[2] != 3200 || experienceTable[25] != 327500) return 4;
    return 0;
}
