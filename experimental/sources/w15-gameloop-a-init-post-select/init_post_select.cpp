// Observed initPostSelect__Fv at 0x00280660, 84 bytes, 16-byte frame. It calls initPosition__Fii with the signed
// byte at offset 32 of the current player's 41-byte playerSelectInfo record and 0, then stores a float constant
// (bit pattern 0x3d5d2f1a) into `delta`, 7 into `selectMode` and 1 into `lockAlpha`. It returns nothing.
// Inferred: mode 7 is the "advance to the next player" step of the selection screens (the same switch that
// playerSelectionTask implements). Unknown: what the float constant means, and the result of initPosition.
extern "C" {
extern int curPlayer;
extern int selectMode;
extern int lockAlpha;
extern float delta;
extern unsigned char playerSelectInfo[];
void initPosition__Fii(int current, int target);
}

namespace {
constexpr float bits(unsigned word) { return __builtin_bit_cast(float, word); }
}

extern "C" void initPostSelect__Fv() {
    initPosition__Fii(static_cast<signed char>(playerSelectInfo[41 * curPlayer + 32]), 0);
    delta = bits(0x3d5d2f1au);
    selectMode = 7;
    lockAlpha = 1;
}
