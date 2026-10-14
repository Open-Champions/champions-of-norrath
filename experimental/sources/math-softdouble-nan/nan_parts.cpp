// Observed: __thenan_df at 0x47df08 is 24 bytes, and every byte in the image is 0.
// The soft-double parts routines read a class word at 0, a sign word at 4, an
// exponent word at 8, and an 8-byte fraction at 16. Class values below 2 are the
// NaN classes in those routines. A zero class word is therefore the NaN record
// they return by pointer.
// Inferred: norrath_player_softdouble_nan_parts_view is this 24-byte image.
// Unknown: no execution showed a writer, and the four padding bytes at offset 12
// are indistinguishable from the zero fraction's neighboring zeros.
// The explicit extern keeps this const record externally linked. A plain const
// definition would have internal linkage and could be dropped.
extern "C" alignas(8) const unsigned char norrath_player_softdouble_nan_parts_view[24] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
