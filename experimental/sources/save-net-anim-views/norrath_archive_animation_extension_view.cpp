// Observed: the bytes anm and a terminating NUL.
// Inferred: the following extension literals vif, tex, and the rest are separate strings.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
extern const unsigned char norrath_archive_animation_extension_view[];
}
extern "C" alignas(1) const unsigned char norrath_archive_animation_extension_view[4] = { 0x61, 0x6e, 0x6d, 0x00 };
static_assert(sizeof(norrath_archive_animation_extension_view) == 4, "complete object size");
