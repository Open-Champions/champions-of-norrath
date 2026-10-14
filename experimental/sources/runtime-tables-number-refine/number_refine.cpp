// Observed three doubles immediately before the formatter text cluster at
// 0x47ed60: 0x47ed48 holds 0.4999999, 0x47ed50 holds 0.5000001, and 0x47ed58
// holds 0.24999995. Bytes below are the image at those addresses,
// little-endian. The strtod correction loop compares its fractional
// adjustment against the first two when the value has leftover digits and
// against the third otherwise, repeating the correction while the fraction
// stays within those bounds.
// Inferred: they are norrath_player_number_half_lower_view,
// norrath_player_number_half_upper_view, and
// norrath_player_number_quarter_lower_view.
// Unknown: placement at the original addresses and section identity with
// .rodata are not claimed by these relocatable objects.
extern "C" const unsigned char norrath_player_number_half_lower_view[8] = {
    0x95, 0x35, 0xa0, 0x94, 0xff, 0xff, 0xdf, 0x3f,
};
extern "C" const unsigned char norrath_player_number_half_upper_view[8] = {
    0x35, 0xe5, 0xaf, 0x35, 0x00, 0x00, 0xe0, 0x3f,
};
extern "C" const unsigned char norrath_player_number_quarter_lower_view[8] = {
    0x95, 0x35, 0xa0, 0x94, 0xff, 0xff, 0xcf, 0x3f,
};
// Observed __infinity at 0x47f608, 8 bytes in .rodata: positive infinity.
// The strtod overflow path stores this word as the converted value after
// setting errno to ERANGE. Bytes below are the image at that address.
// Inferred: it is norrath_player_number_overflow_view.
extern "C" const unsigned char norrath_player_number_overflow_view[8] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f,
};
