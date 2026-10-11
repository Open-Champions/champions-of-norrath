// Observed: the bytes %s.lmp and a terminating NUL.
// Inferred: nothing past the NUL belongs to this string.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
extern const unsigned char norrath_mc_lump_format_bytes[];
}
extern "C" alignas(1) const unsigned char norrath_mc_lump_format_bytes[7] = { 0x25, 0x73, 0x2e, 0x6c, 0x6d, 0x70, 0x00 };
static_assert(sizeof(norrath_mc_lump_format_bytes) == 7, "complete object size");
