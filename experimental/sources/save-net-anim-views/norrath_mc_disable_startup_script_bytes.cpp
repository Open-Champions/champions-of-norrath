// Observed: one-byte object mcDisableStartupScript, initial value 0.
// Inferred: none; the size is the symbol size.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char norrath_mc_disable_startup_script_bytes[1] = {};
}
static_assert(sizeof(norrath_mc_disable_startup_script_bytes) == 1, "complete object size");
