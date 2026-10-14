// Observed: four little-endian .lit4 bytes 99 99 99 3e.
// Inferred: this view is one pool slot, not an ELF object symbol of its own.
// Unknown: neighboring pool bytes that are not read through this symbol.
extern "C" {
alignas(4) unsigned char norrath_inventory_animation_transition_rate_view[4] = { 0x99, 0x99, 0x99, 0x3e };
}
static_assert(sizeof(norrath_inventory_animation_transition_rate_view) == 4, "complete object size");
