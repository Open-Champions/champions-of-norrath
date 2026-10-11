// Observed: netInterfaceIsUp is the one byte immediately before netNetworkMode.
// Its initial value is 0.
// Inferred: none. The recorded size is 1.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(1) unsigned char netInterfaceIsUp[1] = {};
}
static_assert(sizeof(netInterfaceIsUp) == 1, "complete object size");
