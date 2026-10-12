#include "recieve_packet.cpp"
#include <cstring>
unsigned char packetbuffer[8512 + 16];
int packetshift;
static int ref_position(const unsigned char* ring, int shift) {
    int last = -1;
    for (int i = 0; i < 16; ++i) if (ring[((((i + shift) % 16) + 16) % 16) * 133 + 132]) last = i;
    return last < 0 ? 4 : last + 1;
}
int main() {
    unsigned char pkt[132];
    int checked = 0;
    for (int shift = 0; shift < 60; shift += 3)
        for (int src = 0; src < 4; ++src)
            for (int pattern = 0; pattern < 40; ++pattern) {
                memset(packetbuffer, 0x5a, sizeof packetbuffer);
                unsigned char* ring = packetbuffer + src * 2128;
                for (int s = 0; s < 16; ++s) { ring[s * 133 + 132] = 0; memset(ring + s * 133, 0x11, 132); }
                // flags by ring position; pattern chooses a deterministic subset
                for (int i = 0; i < 16; ++i) {
                    int sl = (((i + shift) % 16) + 16) % 16;
                    if (pattern == 39 ? 1 : ((pattern * 7 + i * 3) % 5 == 0 && pattern < 20) ? 1 : (pattern >= 20 && pattern < 38 && i == pattern - 20 ? 1 : 0))
                        ring[sl * 133 + 132] = 0x80 + (i & 1);  // any nonzero byte counts
                }
                for (int i = 0; i < 132; ++i) pkt[i] = (unsigned char)(i * 3 + pattern + 1);
                unsigned char before[sizeof packetbuffer];
                memcpy(before, packetbuffer, sizeof before);
                int pos = ref_position(ring, shift);
                packetshift = shift;
                recievePacket__FiPUc(src, pkt);
                ++checked;
                if (pos == 16) {
                    if (memcmp(before, packetbuffer, sizeof before)) return 1;
                    continue;
                }
                int sl = (((pos + shift) % 16) + 16) % 16;
                memcpy(before + src * 2128 + sl * 133, pkt, 132);
                before[src * 2128 + sl * 133 + 132] = 1;
                if (memcmp(before, packetbuffer, sizeof before)) return 2;
            }
    // full ring drops: pattern 39 sets every flag
    return checked == 20 * 4 * 40 ? 0 : 3;
}
