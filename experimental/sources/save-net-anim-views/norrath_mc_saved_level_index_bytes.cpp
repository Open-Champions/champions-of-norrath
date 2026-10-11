// Observed: the index base and the saved-level payload base are 648 bytes apart, and 27 records of 24 bytes fill that span. The loader reads the first byte of record 26.
// Inferred: the whole 648-byte span is the index object. Initial bytes are zero. No ELF symbol records a size for it.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(8) unsigned char norrath_mc_saved_level_index_bytes[648] = {};
}
static_assert(sizeof(norrath_mc_saved_level_index_bytes) == 648, "complete object size");
