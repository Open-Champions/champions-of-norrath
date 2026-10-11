// Observed: player 0 flag is one byte; player records are 17632 bytes apart.
// Inferred: four records, because the partners symbol runs exactly four strides before the next symbol. The array is the contiguous span from player 0's flag through player 3's flag, so a 17632-byte stride stays inside one object. Bytes between those flags are inside the span. Other views cover some of those bytes and are separate objects.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(4) unsigned char norrath_network_player_local_flags_view[52897] = {};
}
static_assert(sizeof(norrath_network_player_local_flags_view) == 52897, "complete object size");
