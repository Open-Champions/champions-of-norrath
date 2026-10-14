// Observed _sceRpcGetPacket at 0x002d7ad0, 168 bytes (an identical-shape copy
// sits at 0x002e4700 in the CD library; this one stands for both). The argument
// is a packet pool: word 0 is a running id, word 1 points at an array of 64-byte
// packets, word 2 is the packet count. With interrupts disabled it scans for the
// first packet whose word 4 has bit 0 clear. It sets that word to
// (index << 16) | 5, advances the id, stores the new id in packet word 6 and
// the packet's own address in word 5, and returns the packet, or null when every
// packet is taken. The id step skips ahead by two when the increment lands on 1.
extern "C" {
int DIntr();
int EIntr();

struct RpcPacket {
    unsigned words[16];
};

struct RpcPacketPool {
    unsigned next_id;
    RpcPacket* packets;
    int count;
};

RpcPacket* _sceRpcGetPacket(RpcPacketPool* pool) {
    DIntr();
    int count = pool->count;
    RpcPacket* packet = pool->packets;
    for (int index = 0; index < count; ++index, ++packet) {
        if (packet->words[4] & 1) continue;
        packet->words[4] = (static_cast<unsigned>(index) << 16) | 5u;
        unsigned id = pool->next_id + 1;
        pool->next_id = id;
        if (id == 1) {
            pool->next_id = id + 1;
        }
        packet->words[5] = reinterpret_cast<unsigned long>(packet);
        packet->words[6] = id;
        EIntr();
        return packet;
    }
    EIntr();
    return 0;
}
}
