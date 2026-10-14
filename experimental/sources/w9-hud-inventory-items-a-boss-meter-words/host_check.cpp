// Host contract for the boss meter words. Not an EE plan input.
#include "boss_meter_words.cpp"
int main() {
    if (bossMeterCurrent != 0.0f || bossMeterMax != 0.0f || bossMeterAlpha != 0) return 1;
    if (sizeof(bossMeterCurrent) != 4 || sizeof(bossMeterMax) != 4 || sizeof(bossMeterAlpha) != 4) return 2;
    return 0;
}
