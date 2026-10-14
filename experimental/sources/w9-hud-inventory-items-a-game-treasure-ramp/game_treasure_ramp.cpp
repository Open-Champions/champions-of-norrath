// Observed gameTreasureRamp__Fv at 0x001d0ff8, 16 bytes, in the pinned SLUS_205.65:
// returns worldDifficulty (0x00484cac) times a float constant stored in small data
// at 0x0048068c, whose bit pattern in the image is 0x3ecccccc (just under 0.4).
// The constant is reproduced by its bits; the image's literal is one ulp below
// the nearest float to 0.4.
// Inferred: single-precision multiply, operands in either order.
// Unknown: the meaning of the constant and of the result for the callers.
extern "C" float worldDifficulty;

static inline float single_from_bits(unsigned bits) {
    union { unsigned u; float f; } v;
    v.u = bits;
    return v.f;
}

extern "C" float gameTreasureRamp__Fv() {
    return worldDifficulty * single_from_bits(0x3ecccccc);
}
