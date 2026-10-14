// Host contract for the boss-meter words and hudNoDraw. Not an EE plan input.
#include "boss_meter_words.cpp"
static_assert(sizeof(hudNoDraw) == 1, "hudNoDraw extent");
int main() {
    if (bossMeterCurrent || bossMeterMax || bossMeterAlpha || hudNoDraw) return 1;
    return 0;
}
