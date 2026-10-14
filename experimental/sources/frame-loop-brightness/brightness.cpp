// Observed engineBrightnessCorrection at 0x00484420, 4 bytes in .sdata.
// The file bytes are 00 00 00 3f, the word 0x3f000000.
// gameInit replaces this word from MC_GameConfig+4. This unit is the
// initial file word, not that later store.
extern "C" {
unsigned engineBrightnessCorrection = 0x3f000000u;
}
static_assert(sizeof(engineBrightnessCorrection) == 4, "brightness word");
