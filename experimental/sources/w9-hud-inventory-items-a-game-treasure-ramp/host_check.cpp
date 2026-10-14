// Host contract for gameTreasureRamp__Fv. Not an EE plan input.
#include "game_treasure_ramp.cpp"
extern "C" float worldDifficulty = 0.0f;
int main() {
    worldDifficulty = 1.0f;
    if (gameTreasureRamp__Fv() != single_from_bits(0x3ecccccc)) return 1;
    worldDifficulty = 2.0f;
    if (gameTreasureRamp__Fv() != single_from_bits(0x3ecccccc) * 2.0f) return 2;
    worldDifficulty = 0.0f;
    if (gameTreasureRamp__Fv() != 0.0f) return 3;
    worldDifficulty = 10.0f;
    if (gameTreasureRamp__Fv() != 10.0f * single_from_bits(0x3ecccccc)) return 4;
    return 0;
}
