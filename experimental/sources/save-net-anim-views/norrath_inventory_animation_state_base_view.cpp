// Observed: one animation state is 264 bytes. The object animState runs exactly 1056 bytes before the next symbol.
// Inferred: four states. Initial bytes are zero.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(8) unsigned char norrath_inventory_animation_state_base_view[1056] = {};
}
static_assert(sizeof(norrath_inventory_animation_state_base_view) == 1056, "complete object size");
