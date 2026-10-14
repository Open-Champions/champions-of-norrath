// Observed: the bytes script.scr and a terminating NUL.
// Inferred: the rodata slot is padded out to the next literal; those pad bytes are not read as part of the key.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
extern const unsigned char norrath_mc_script_section_key_bytes[];
}
extern "C" alignas(1) const unsigned char norrath_mc_script_section_key_bytes[11] = { 0x73, 0x63, 0x72, 0x69, 0x70, 0x74, 0x2e, 0x73, 0x63, 0x72, 0x00 };
static_assert(sizeof(norrath_mc_script_section_key_bytes) == 11, "complete object size");
