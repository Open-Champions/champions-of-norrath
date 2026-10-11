// Observed: coreBeenInSRS is one byte, initial value 0, immediately after
// fromSRSButEnteringGame.
// Inferred: none. The recorded size is 1.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char coreBeenInSRS[1] = {};
}
static_assert(sizeof(coreBeenInSRS) == 1, "complete object size");
