// Observed _sceRpcGetFPacket2 at 0x002d7bc8, 64 bytes, a local routine of the SIF
// RPC library, called by _request_rdata when bit 2 of the packet's flags word is
// set. Arguments are the service list (s) and a signed index.
//
// Observed layout of the fields it touches:
//   s +28  base of a second packet array (64-byte entries)
//   s +32  number of entries in that array
// An index from 0 up to but excluding the entry count (signed compare) returns
// base + index * 64. Any other index, including every negative one, falls back to
// _sceRpcGetFPacket(s) and returns what it returns. Nothing is written here. What
// the second array is for is unknown.
extern "C" {
struct RpcServiceList {
    unsigned char before_base[28];
    unsigned char* indexed_packets;
    int indexed_count;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcServiceList, indexed_packets) == 28, "layout");
static_assert(__builtin_offsetof(RpcServiceList, indexed_count) == 32, "layout");
#endif

unsigned char* _sceRpcGetFPacket(RpcServiceList* services);

unsigned char* _sceRpcGetFPacket2(RpcServiceList* services, int index) {
    if (index < 0 || index >= services->indexed_count)
        return _sceRpcGetFPacket(services);
    return services->indexed_packets + (index << 6);
}
}
