// Observed: four little-endian .lit4 bytes 88 88 88 3c.
// Inferred: this view is one pool slot, not an ELF object symbol of its own.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
alignas(4) unsigned char norrath_animation_transition_threshold_view[4] = { 0x88, 0x88, 0x88, 0x3c };
}
static_assert(sizeof(norrath_animation_transition_threshold_view) == 4, "complete object size");
