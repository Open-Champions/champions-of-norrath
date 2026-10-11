// Observed: the payload base is 0x1b898 bytes into ramSlot, and the save routine rejects a write that would pass ramSlot plus 0xb29c8. That limit is the address of the next symbol, slotInfoHeaders.
// Inferred: the payload view occupies every byte from its base through the byte before slotInfoHeaders, 618800 bytes, initially zero.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(8) unsigned char norrath_mc_saved_level_payload_bytes[618800] = {};
}
static_assert(sizeof(norrath_mc_saved_level_payload_bytes) == 618800, "complete object size");
