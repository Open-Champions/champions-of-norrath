// Observed _sceRpcFreePacket at 0x002d7b78, 28 bytes (a second identical-shape
// copy sits at 0x002e47a8 in the CD library; this one stands for both). The
// argument is a 64-byte RPC packet. Word 4 loses its low bit, which the packet
// getter sets to mark the packet as taken, and word 6 is cleared.
extern "C" void _sceRpcFreePacket(unsigned* packet) {
    unsigned flags = packet[4];
    packet[6] = 0;
    packet[4] = flags & 0xfffffffeu;
}
