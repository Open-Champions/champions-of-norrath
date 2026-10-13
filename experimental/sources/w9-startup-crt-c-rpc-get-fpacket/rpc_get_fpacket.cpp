// Observed _sceRpcGetFPacket at 0x002d7b98, 48 bytes, a local routine of the SIF RPC
// library. The argument is the service list that _request_bind and _request_rdata
// are registered with (called s here).
//
// Observed layout of the fields it touches:
//   s +20  base of a packet array (64-byte entries)
//   s +24  number of entries
//   s +36  running counter
// It takes counter % entries as a signed remainder, stores that remainder plus one
// back to the counter, and returns base + remainder * 64. A zero entry count traps
// with break code 7 (the compiler's divide-check). The entries themselves are not
// touched. What the library calls the array (a reply ring) is inferred only from
// the way the callers fill the returned packet.
extern "C" {
struct RpcServiceList {
    unsigned char before_base[20];
    unsigned char* packets;
    int packet_count;
    unsigned char between[8];
    int next_packet;
};

#if defined(__mips__)
static_assert(__builtin_offsetof(RpcServiceList, packets) == 20, "layout");
static_assert(__builtin_offsetof(RpcServiceList, packet_count) == 24, "layout");
static_assert(__builtin_offsetof(RpcServiceList, next_packet) == 36, "layout");
#endif

unsigned char* _sceRpcGetFPacket(RpcServiceList* services) {
    int slot = services->next_packet % services->packet_count;
    services->next_packet = slot + 1;
    return services->packets + (slot << 6);
}
}
