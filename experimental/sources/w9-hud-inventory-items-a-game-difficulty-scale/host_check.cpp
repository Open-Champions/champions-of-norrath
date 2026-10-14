// Host contract for gameDifficultyScale__Fv. Not an EE plan input.
#include "game_difficulty_scale.cpp"
extern "C" float worldDifficulty = 0.0f;
int main() {
    float v[] = {0.0f, 1.0f, 1.5f, -2.25f, 100.0f};
    for (int i = 0; i < 5; ++i) {
        worldDifficulty = v[i];
        if (gameDifficultyScale__Fv() != v[i]) return 1 + i;
    }
    return 0;
}
