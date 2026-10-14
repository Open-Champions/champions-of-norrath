// Host contract for gameItemTreasureRamp. Not an EE plan input.
#include "game_item_treasure_ramp.cpp"

extern "C" {
float worldDifficulty = 0.0f;
int gameDifficulty = 0;
}

int main() {
    worldDifficulty = 2.0f;
    int level[] = {0, 1, 2, 3, 4, -1};
    float expect[] = {6.0f, 6.0f, 8.0f, 10.0f, 6.0f, 6.0f};
    for (int i = 0; i < 6; ++i) {
        gameDifficulty = level[i];
        if (gameItemTreasureRamp__Fv() != expect[i]) return 1 + i;
    }
    worldDifficulty = 0.5f;
    gameDifficulty = 3;
    if (gameItemTreasureRamp__Fv() != 2.5f) return 10;
    return 0;
}
