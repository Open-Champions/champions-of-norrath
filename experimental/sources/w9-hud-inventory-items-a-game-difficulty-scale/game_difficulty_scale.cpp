// Observed gameDifficultyScale__Fv at 0x001d12b0, 8 bytes, in the pinned SLUS_205.65:
// returns the float worldDifficulty (0x00484cac) unchanged.
// Unknown: what the callers use the value for beyond scaling damage and prop numbers.
extern "C" float worldDifficulty;

extern "C" float gameDifficultyScale__Fv() {
    return worldDifficulty;
}
