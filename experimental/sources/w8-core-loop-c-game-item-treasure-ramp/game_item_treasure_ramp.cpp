// gameItemTreasureRamp at 0x001d1008, 60 bytes, in SLUS_205.65.
// Observed: returns the float worldDifficulty (0x00484cac) times a factor chosen by the word
// gameDifficulty (0x00484cd8): 4.0 when it is 2, 5.0 when it is 3, and 3.0 for any other value.
// The multiply is a single-precision multiply.
// Unknown: what the callers do with the result, and what the difficulty values mean.
extern "C" float worldDifficulty;
extern "C" int gameDifficulty;

extern "C" float gameItemTreasureRamp__Fv() {
    float factor = 3.0f;
    if (gameDifficulty == 2)
        factor = 4.0f;
    else if (gameDifficulty == 3)
        factor = 5.0f;
    return worldDifficulty * factor;
}
