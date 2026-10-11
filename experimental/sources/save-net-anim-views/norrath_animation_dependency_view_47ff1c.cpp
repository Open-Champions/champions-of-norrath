// Observed: four little-endian .lit4 bytes 83 f9 22 46, the single-precision value used as the atan2 scale.
// Inferred: this view is one pool slot, not an ELF object symbol of its own.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
alignas(4) unsigned char norrath_animation_dependency_view_47ff1c[4] = { 0x83, 0xf9, 0x22, 0x46 };
}
static_assert(sizeof(norrath_animation_dependency_view_47ff1c) == 4, "complete object size");
