#include "rpc_get_fpacket2.cpp"

static RpcServiceList* fallback_seen;
static unsigned char fallback_packet[64];
extern "C" unsigned char* _sceRpcGetFPacket(RpcServiceList* services) {
    fallback_seen = services;
    return fallback_packet;
}

int main() {
    static unsigned char indexed[64 * 4];
    RpcServiceList list = {};
    list.indexed_packets = indexed;
    list.indexed_count = 4;
    if (_sceRpcGetFPacket2(&list, 0) != indexed || fallback_seen) return 1;
    if (_sceRpcGetFPacket2(&list, 3) != indexed + 192 || fallback_seen) return 2;
    // The count itself and anything above it, and every negative index, use the ring.
    if (_sceRpcGetFPacket2(&list, 4) != fallback_packet || fallback_seen != &list) return 3;
    fallback_seen = nullptr;
    if (_sceRpcGetFPacket2(&list, -1) != fallback_packet || fallback_seen != &list) return 4;
    fallback_seen = nullptr;
    if (_sceRpcGetFPacket2(&list, 0x7fffffff) != fallback_packet || fallback_seen != &list) return 5;
    // An empty second array sends even index 0 to the ring.
    fallback_seen = nullptr;
    list.indexed_count = 0;
    if (_sceRpcGetFPacket2(&list, 0) != fallback_packet || fallback_seen != &list) return 6;
    return 0;
}
