// Observed: each player ring is addressed with a 13-bit mask, so 8192 bytes are reachable from that player's ring base, and records are 17632 bytes apart.
// Inferred: four records, from the same partners extent. The array is the contiguous span from player 0's ring through the 8192 reachable bytes of player 3. Bytes between one ring and the next player's ring are inside that span.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(4) unsigned char norrath_network_send_ring_bytes_view[61088] = {};
}
static_assert(sizeof(norrath_network_send_ring_bytes_view) == 61088, "complete object size");
