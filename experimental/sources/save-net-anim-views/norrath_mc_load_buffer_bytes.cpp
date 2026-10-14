// Observed: symbol loadBuffer has no recorded size. The next symbol, tempBuffer, begins 60032 bytes later. Initial bytes are zero.
// Inferred: the gap is the object extent. No separate object was established inside the gap.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(8) unsigned char norrath_mc_load_buffer_bytes[60032] = {};
}
static_assert(sizeof(norrath_mc_load_buffer_bytes) == 60032, "complete object size");
