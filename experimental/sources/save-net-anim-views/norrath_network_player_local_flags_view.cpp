// Observed: player 0 flag is one byte; player records are 17632 bytes apart.
// Inferred: four records, because the partners symbol runs exactly four strides before the next symbol. Only the flag byte of each record belongs to this view.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(4) unsigned char norrath_network_player_local_flags_view[52897] = {};
}
static_assert(sizeof(norrath_network_player_local_flags_view) == 52897, "complete object size");
