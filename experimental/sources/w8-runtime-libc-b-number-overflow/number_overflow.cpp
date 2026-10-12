// Observed __infinity at 0x0047f608, 8 bytes: the little-endian double
// positive infinity (0x7ff0000000000000). The number parser reads it as the
// result of an overflowing conversion.
extern "C" const unsigned char norrath_player_number_overflow_view[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x7f
};
