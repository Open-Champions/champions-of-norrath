// Observed: each player ring is addressed with a 13-bit mask, so 8192 bytes are reachable, and records are 17632 bytes apart.
// Inferred: four records, from the same partners extent. Bytes between the 8192-byte ring and the next player's ring are not part of this view.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(4) unsigned char norrath_network_send_ring_bytes_view[61088] = {};
}
static_assert(sizeof(norrath_network_send_ring_bytes_view) == 61088, "complete object size");
