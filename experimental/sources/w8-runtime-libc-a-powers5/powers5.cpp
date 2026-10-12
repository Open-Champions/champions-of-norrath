// Observed `p05.0` at 0x0047f4e0, a local read-only object of 12 bytes: three
// little-endian words 5, 25 and 125 (the first three powers of five). The
// bigint power-of-five routine indexes it by remainder minus one. The 16 bytes
// from this address hold a further zero word of padding before __mprec_tens at
// 0x0047f4f0, which this unit does not define.
// Inferred: the values are exactly 5^1, 5^2 and 5^3.
extern "C" alignas(4) const unsigned char norrath_player_bigint_small_powers5_view[12] = {
    0x05, 0x00, 0x00, 0x00,
    0x19, 0x00, 0x00, 0x00,
    0x7d, 0x00, 0x00, 0x00,
};
