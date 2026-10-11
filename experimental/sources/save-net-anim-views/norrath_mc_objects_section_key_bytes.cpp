// Observed: the bytes objects.ob and a terminating NUL.
// Inferred: padding before the next literal is not part of this key.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
extern const unsigned char norrath_mc_objects_section_key_bytes[];
}
extern "C" alignas(1) const unsigned char norrath_mc_objects_section_key_bytes[11] = { 0x6f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x73, 0x2e, 0x6f, 0x62, 0x00 };
static_assert(sizeof(norrath_mc_objects_section_key_bytes) == 11, "complete object size");
