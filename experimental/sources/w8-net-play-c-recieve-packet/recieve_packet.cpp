// Observed recievePacket__FiPUc at 0x002cc2f0, 440 bytes (source index, packet
// pointer). packetbuffer (0x00442668, 8512 bytes) is read as 4 sources of 2128
// bytes; each source holds 16 ring slots of 133 bytes: 132 bytes of packet data
// followed by one flag byte. The word packetshift is the ring origin. For ring
// position i = 0..15 the slot used is (i + packetshift) mod 16 (signed). The
// scan keeps the last position whose flag byte is nonzero. The target position
// is that value plus one, or 4 if no flag is set. If the target position is 16
// the packet is dropped. Otherwise the 132 packet bytes are copied into the slot
// for that position and its flag byte is set to 1. The source index and the
// packetshift value are not range checked; the modulus is signed, so a negative
// sum would index below the ring (the host check uses non-negative origins). Why an empty ring starts at position
// 4 is not established.
extern "C" {
extern unsigned char packetbuffer[];
extern int packetshift;

void recievePacket__FiPUc(int source, unsigned char* packet) {
    int shift = packetshift;
    unsigned char* ring = packetbuffer + source * 2128;
    int last = -1;
    for (int i = 0; i < 16; ++i) {
        if ((signed char)ring[((i + shift) % 16) * 133 + 132] != 0) last = i;
    }
    int position = last == -1 ? 4 : last + 1;
    if (position == 16) return;
    unsigned char* slot = ring + ((position + shift) % 16) * 133;
    for (int i = 0; i < 132; ++i) slot[i] = packet[i];
    slot[132] = 1;
}
}
