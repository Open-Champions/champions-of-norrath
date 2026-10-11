// Observed: symbol _3map$maptiles is 93312 bytes and every initial byte is zero.
// Inferred: none for the size. The original object is file-backed zero in .data; this definition is a zero object under the reviewed -G0 profile.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(16) unsigned char norrath_saved_map_tiles_bytes[93312] = {};
}
static_assert(sizeof(norrath_saved_map_tiles_bytes) == 93312, "complete object size");
