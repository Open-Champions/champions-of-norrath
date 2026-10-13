#include "rpc_get_fpacket.cpp"

int main() {
    static unsigned char ring[64 * 3];
    RpcServiceList list = {};
    list.packets = ring;
    list.packet_count = 3;
    list.next_packet = 0;
    // Entries are handed out in order and the counter wraps to the entry count.
    if (_sceRpcGetFPacket(&list) != ring) return 1;
    if (list.next_packet != 1) return 2;
    if (_sceRpcGetFPacket(&list) != ring + 64) return 3;
    if (_sceRpcGetFPacket(&list) != ring + 128) return 4;
    if (list.next_packet != 3) return 5;
    // The counter is reduced on use: 3 % 3 is entry 0, and the stored value restarts at 1.
    if (_sceRpcGetFPacket(&list) != ring) return 6;
    if (list.next_packet != 1) return 7;
    // A stored counter beyond the count is reduced, and a negative one keeps a
    // negative (signed) remainder.
    list.next_packet = 7;
    if (_sceRpcGetFPacket(&list) != ring + 64) return 8;
    if (list.next_packet != 2) return 9;
    list.next_packet = -2;
    if (_sceRpcGetFPacket(&list) != ring - 128) return 10;
    if (list.next_packet != -1) return 11;
    return 0;
}
