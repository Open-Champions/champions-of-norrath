// Observed p05.0 at 0x47f4e0, 12 bytes in .rodata: three native words 5,
// 25, 125. The bigint power-of-five path multiplies by the word at index
// remainder - 1 for remainders 1 through 3, then continues from 625 by
// squaring, so these are 5^1 through 5^3. Bytes below are the image at
// that address, little-endian.
// Inferred: it is norrath_player_bigint_small_powers5_view.
// Unknown: placement at the original address and section identity with
// .rodata are not claimed by this relocatable object.
extern "C" const unsigned char norrath_player_bigint_small_powers5_view[12] = {
    0x05, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x7d, 0x00, 0x00, 0x00,
};
