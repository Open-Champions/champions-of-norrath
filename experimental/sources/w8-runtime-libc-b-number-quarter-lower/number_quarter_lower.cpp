// Observed 8-byte constant at 0x0047ed58: the little-endian double 0.24999995.
// The number parser reads it as one double and uses it as a bound (compared with the fraction on the other rounding branch; a smaller fraction stops the loop).
// Its neighbours at this address are the other two rounding bounds, which are
// separate views.
extern "C" const unsigned char norrath_player_number_quarter_lower_view[] = {
    0x95, 0x35, 0xa0, 0x94, 0xff, 0xff, 0xcf, 0x3f
};
