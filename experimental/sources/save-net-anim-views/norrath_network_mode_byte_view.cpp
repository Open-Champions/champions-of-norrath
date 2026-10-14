// Observed: one-byte object netNetworkMode, initial value 0.
// Inferred: none; the size is the symbol size.
// Unknown: which startup routine first writes this storage.
// netNetworkMode is this same byte. The second name is an alias, not a copy.
extern "C" {
alignas(1) unsigned char norrath_network_mode_byte_view[1] = {};
extern unsigned char netNetworkMode __attribute__((alias("norrath_network_mode_byte_view")));
}
static_assert(sizeof(norrath_network_mode_byte_view) == 1, "complete object size");
