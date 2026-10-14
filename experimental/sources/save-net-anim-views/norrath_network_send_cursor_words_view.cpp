// Observed: player 0 cursor is one 4-byte word; records are 17632 bytes apart. Initial bytes are zero.
// Inferred: four records, from the same partners extent.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(4) unsigned char norrath_network_send_cursor_words_view[52900] = {};
}
static_assert(sizeof(norrath_network_send_cursor_words_view) == 52900, "complete object size");
