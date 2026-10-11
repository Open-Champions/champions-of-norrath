// Observed: one-byte object netNetworkMode, initial value 0.
// Inferred: none; the size is the symbol size.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char norrath_network_mode_byte_view[1] = {};
}
static_assert(sizeof(norrath_network_mode_byte_view) == 1, "complete object size");
