// Observed: msgHandler begins at the callback view and the next symbol pktHandler begins 64 bytes later. Initial bytes are zero.
// Inferred: 16 pointer slots. A handler index is loaded from a byte and is not bounds-checked, so an index past 15 is outside this object.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(4) unsigned char norrath_network_message_callbacks_view[64] = {};
}
static_assert(sizeof(norrath_network_message_callbacks_view) == 64, "complete object size");
