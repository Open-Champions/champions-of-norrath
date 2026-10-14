// Observed: the bytes start and a terminating NUL.
// Inferred: two following zero bytes are padding before the next sdata symbol, not this string.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
extern const unsigned char norrath_mc_start_key_bytes[];
}
extern "C" alignas(1) const unsigned char norrath_mc_start_key_bytes[6] = { 0x73, 0x74, 0x61, 0x72, 0x74, 0x00 };
static_assert(sizeof(norrath_mc_start_key_bytes) == 6, "complete object size");
