// Observed 8-byte constant at 0x0047ed50: the little-endian double 0.5000001.
// The number parser reads it as one double and uses it as a bound (compared with the fraction; a larger fraction stops the loop).
// Its neighbours at this address are the other two rounding bounds, which are
// separate views.
extern "C" const unsigned char norrath_player_number_half_upper_view[] = {
    0x35, 0xe5, 0xaf, 0x35, 0x00, 0x00, 0xe0, 0x3f
};
