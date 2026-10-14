// Observed: fromSRSButEnteringGame is one byte, initial value 0, between
// fromSRS and coreBeenInSRS. The original binding is local.
// Inferred: none. The recorded size is 1.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char fromSRSButEnteringGame[1] = {};
}
static_assert(sizeof(fromSRSButEnteringGame) == 1, "complete object size");
