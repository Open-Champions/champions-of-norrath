#include "rpc_free_packet.cpp"

#include <cstdio>

int main() {
    unsigned packet[16];
    for (unsigned i = 0; i < 16; ++i) packet[i] = 0xa5000000u + i;
    packet[4] = 0x00030005u;
    _sceRpcFreePacket(packet);
    if (packet[4] != 0x00030004u || packet[6] != 0) return 1;
    for (unsigned i = 0; i < 16; ++i)
        if (i != 4 && i != 6 && packet[i] != 0xa5000000u + i) return 2;
    packet[4] = 0xffffffffu;
    _sceRpcFreePacket(packet);
    if (packet[4] != 0xfffffffeu) return 3;
    packet[4] = 0;
    _sceRpcFreePacket(packet);
    if (packet[4] != 0) return 4;
    std::puts("_sceRpcFreePacket passed");
    return 0;
}
