// Observed: netGameLaunchInfo is 328 bytes and every initial byte is 0.
// Inferred: none. The recorded size is the whole object.
// Unknown: which startup routine first writes this storage.
extern "C" {
alignas(4) unsigned char netGameLaunchInfo[328] = {};
}
static_assert(sizeof(netGameLaunchInfo) == 328, "complete object size");
