// Observed: netInitState is one byte, initial value 0.
// Inferred: none. The recorded size is 1.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char netInitState[1] = {};
}
static_assert(sizeof(netInitState) == 1, "complete object size");
