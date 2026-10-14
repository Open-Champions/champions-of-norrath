// Observed: fromSRS is one byte, initial value 0, immediately before
// fromSRSButEnteringGame. The original binding is local.
// Inferred: none. The recorded size is 1.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char fromSRS[1] = {};
}
static_assert(sizeof(fromSRS) == 1, "complete object size");
