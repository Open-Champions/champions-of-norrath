// Observed hudGetRemappedButtonIcon__Fii at 0x001c7b30, 200 bytes.
//
// Observed: the word read is MC_GameConfig + 52 + 36 * first + 4 * second (the
// per-player 36-byte button records used elsewhere in the config). It is
// compared in order with a 12-entry constant table that the function copies to
// its stack; the result is 123 + index of the first equal entry, or 0 when no
// entry matches.
// Inferred: the table holds button mask values and 123.. are icon numbers.
// Unknown: what the icons look like and the valid range of the arguments.
extern "C" unsigned char MC_GameConfig[];

extern "C" int hudGetRemappedButtonIcon__Fii(int player, int slot) {
    const int masks[12] = {32, 16, 128, 64, 32768, 8192, 4096, 16384, 4, 1, 8, 2};
    int word = *reinterpret_cast<const int*>(MC_GameConfig + 52 + (player * 9 + slot) * 4);
    for (int i = 0; i < 12; ++i)
        if (masks[i] == word) return 123 + i;
    return 0;
}
