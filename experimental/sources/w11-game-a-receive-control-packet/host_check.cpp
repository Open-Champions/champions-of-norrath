// Host contract for receiveControlPacket; not an EE input.
#include <cstdio>
#include <cstring>
#include <vector>
#include "receive_control_packet.cpp"
signed char netNetworkMode;
int nmPartners;
unsigned char partners[4 * 17632 + 64];
unsigned char norrath_net_flag_bytes_48681c[8];
static int compares, comparePartner;
static unsigned char* compareHeld;
static unsigned char* compareReceived;
static int analCalls;
static std::vector<unsigned char*> analStates;
void compareStatePackets__FP11statePacketT0i(void* held, void* received, int partner) {
    ++compares; compareHeld = static_cast<unsigned char*>(held); compareReceived = static_cast<unsigned char*>(received); comparePartner = partner;
}
void analDecompress__11CDecompressP15BitBufferReaderPQ211CDecompress9analStatePUcii(void*, void* reader, void* state, unsigned char* out, int count, int skip) {
    BitBufferReader* r = static_cast<BitBufferReader*>(reader);
    analStates.push_back(static_cast<unsigned char*>(state));
    r->position += 8;
    for (int i = 0; i < count; ++i) out[i] = static_cast<unsigned char>(0x10 * (++analCalls > 0) + i + skip * 100);
}
struct Bits {
    std::vector<int> b;
    void put(unsigned v, int n) { for (int i = n - 1; i >= 0; --i) b.push_back((v >> i) & 1); }
    void bytes(unsigned char* to) {
        while (b.size() % 16) b.push_back(0);
        for (size_t i = 0; i < b.size(); i += 16) {
            unsigned h = 0;
            for (int j = 0; j < 16; ++j) h = h * 2 + b[i + j];
            to[i / 8] = h & 0xff; to[i / 8 + 1] = h >> 8;
        }
    }
};
static int w(const unsigned char* p) { int v; std::memcpy(&v, p, 4); return v; }
static void put(unsigned char* p, int v) { std::memcpy(p, &v, 4); }
int main() {
    int bad = 0;
    alignas(16) static unsigned char packet[1024];
    unsigned char* P = partners + 17632;
    nmPartners = 2; netNetworkMode = 1;
    partners[17620] = 1;
    int now = 1000;
    // compressed path: 3 frames, nothing held yet beyond now - 3
    put(P + 17596, now - 3);
    put(P + 16 + 66, 10 | (2 << 16));  // position 10, velocity 2
    put(P + 17580, 8190);
    std::memset(packet, 0, sizeof packet);
    put(packet + 4, now); packet[8] = 1; packet[9] = 2; packet[10] = 3; packet[11] = 5;
    Bits bits;
    bits.put(0x0102, 16); bits.put(3, 5);
    bits.put(0, 32);
    bits.put(3, 5);                   // entry word run covering all three entries
    bits.put(1, 1); bits.put(3, 5);   // bit 1, run of 3
    bits.put(0, 16);
    bits.bytes(packet + 12);
    // bit position at the end = 16 + 5 + 32 + 5 + 1 + 5 + 16 = 80 -> 10 bytes consumed
    size_t consumed = (80 + 7) >> 3;
    unsigned char* after = packet + 12 + consumed;
    unsigned char* aligned = reinterpret_cast<unsigned char*>((reinterpret_cast<unsigned long>(after) + 3) & ~3ul);
    put(aligned, 0xbd40);
    unsigned char* st = aligned + 4;
    st[0] = 3; st[1] = 9; st[4] = 7;
    // the local partner (index 0) holds a state packet with the same first word
    std::memcpy(partners + 6316 + 12 * 3, st, 4);
    unsigned char* t = st + 12;
    t[0] = 1; t[1] = 3; t[2] = 0xaa; t[3] = 0xbb; t[4] = 0xcc;   // record now-1: 3 bytes
    t[5] = 2; t[6] = 2; t[7] = 0x11; t[8] = 0x22;
    int length = static_cast<int>(t + 9 - packet);
    receiveControlPacket__FPiib(reinterpret_cast<int*>(packet), length, false);
    if (w(P + 17596) != now || w(P + 17608) != now - 2 || w(P + 17604) != 2 || w(P + 17600) != now + 5 || w(P + 17616) != 1) { std::puts("fail: header state"); bad = 1; }
    for (int i = 0; i < 3; ++i) {
        int f = now - 2 + i;
        unsigned char* rec = P + 172 + 24 * (f & 0xff);
        short s0; std::memcpy(&s0, rec + 4, 2);
        int pos = w(rec + 12);
        if (w(rec) != f || s0 != 0x0102 || pos != ((12 + 2 * i) | 1)) { std::printf("fail: record %d (%d %d)\n", i, w(rec), pos); bad = 1; }
    }
    if (analStates.size() != 4 || analStates[0] != P + 16 + 32 || analStates[1] != P + 16 + 48 || analStates[2] != P + 16 || analStates[3] != P + 16 + 16) { std::puts("fail: plane states"); bad = 1; }
    if (compares != 1 || comparePartner != 1 || compareReceived != st || compareHeld != partners + 6316 + 36) { std::puts("fail: compare"); bad = 1; }
    if (std::memcmp(P + 6316 + 36, st, 12) != 0) { std::puts("fail: state slot"); bad = 1; }
    // trailer: record now-1 got 3 bytes in the ring at 8190, 8191, 0; record now-2 got 2 bytes at 1, 2
    unsigned char* ring = P + 9388;
    int r1 = w(P + 192 + 24 * ((now - 1) & 0xff));
    if (r1 != 3 || ring[8190] != 0xaa || ring[8191] != 0xbb || ring[0] != 0xcc || w(P + 17580) != 3 || ring[1] != 0x11 || ring[2] != 0x22 || w(P + 192 + 24 * ((now - 2) & 0xff)) != 2) { std::puts("fail: ring"); bad = 1; }
    if (w(P + 188 + 24 * ((now - 1) & 0xff)) != 8190 || w(P + 188 + 24 * ((now - 2) & 0xff)) != 1) { std::puts("fail: trailer records"); bad = 1; }
    if (w(P + 16 + 72) != 3) { std::puts("fail: counter"); bad = 1; }
    // flag path: memcpy of 12-byte entries; local index 0 gets the flag byte; duplicate frame is only noted
    std::memset(packet, 0, sizeof packet);
    put(packet + 4, now + 2); packet[8] = 1; packet[9] = 3; packet[10] = 4; packet[11] = 0;
    for (int i = 0; i < 48; ++i) packet[12 + i] = (unsigned char)(i + 1);
    put(P + 17596, now);      // last = now; new now + 2 -> skip = last - start + 1 = 1000 - 999 + 1 = 2 -> two fresh entries
    P[17622] = 0;
    partners[17620] = 0; partners[17620 + 17632 * 0] = 0;
    // no partner has the local byte: index -1 for netNetworkMode != 0
    receiveControlPacket__FPiib(reinterpret_cast<int*>(packet), 12 + 48 + 4 + 0, true);
    if (w(P + 17596) != now + 2 || w(P + 17624) != now + 3) { std::puts("fail: flag header"); bad = 1; }
    unsigned char* rec = P + 172 + 24 * ((now + 1) & 0xff);
    if (w(rec) != now + 1 || rec[4] != 2 * 12 + 1 || rec[15] != 2 * 12 + 12) { std::puts("fail: flag record"); bad = 1; }
    // partner 0 local: a later duplicate packet only sets +17624
    put(P + 17596, now + 9);
    receiveControlPacket__FPiib(reinterpret_cast<int*>(packet), 12 + 48 + 4, true);
    if (w(P + 17624) != now + 10 || norrath_net_flag_bytes_48681c[3] != 0) { std::puts("fail: duplicate"); bad = 1; }
    partners[17620] = 1;
    put(P + 17624, 0);
    receiveControlPacket__FPiib(reinterpret_cast<int*>(packet), 12 + 48 + 4, true);
    if (w(P + 17624) != 0 || norrath_net_flag_bytes_48681c[3] != 1) { std::puts("fail: local duplicate"); bad = 1; }
    P[17623] = 1;
    put(P + 17596, 0);
    receiveControlPacket__FPiib(reinterpret_cast<int*>(packet), 12 + 48 + 4, true);
    if (w(P + 17596) != 0) { std::puts("fail: refused partner"); bad = 1; }
    if (!bad) std::puts("receiveControlPacket ok");
    return bad;
}
