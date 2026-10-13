// Host check for sendControlPacket__FiiiiiR9CCompressb. Not an EE plan input.
// Build on the host with: c++ -std=c++17 -fno-builtin host_check.cpp
// analCompress is replaced by a recorder that writes nothing, so the checks below
// cover the header, the raw-frame path, the bit-count bookkeeping of the encoder
// (via the sent length), the alignment, the 0xbd40/-1 word, the message bytes and
// the send arguments. The compression stage output itself is covered by the
// private paired run against the image.
#include "send_control_packet.cpp"
#include <cstdio>
#include <cstring>
#include <vector>

extern "C" {
signed char netNetworkMode;
int nmPartners;
alignas(8) unsigned char partners[4 * 17632];
int trueNetFrameNm;
int sentStatePacketFrameNm;
int broadcastConnection;
int broadcastConnectionFlags;
unsigned char cd[8], g_net_buf[8];
}
struct Sent { std::vector<unsigned char> bytes; int length, flags, port, second; unsigned char block[16]; int connection; };
static std::vector<Sent> sent;
static int compress_calls;
static int compress_counts[8];
extern "C" void* memcpy(void* d, const void* s, unsigned long n) {
    unsigned char* b = (unsigned char*)d; const unsigned char* c = (const unsigned char*)s;
    for (unsigned long i = 0; i < n; ++i) b[i] = c[i];
    return d;
}
extern "C" void* memset(void* d, int v, unsigned long n) {
    unsigned char* b = (unsigned char*)d;
    for (unsigned long i = 0; i < n; ++i) b[i] = (unsigned char)v;
    return d;
}
extern "C" int sceInetSendTo(void*, void*, int connection, void* data, int length, int flags, void* address, int first, int second) {
    Sent s;
    s.bytes.assign((unsigned char*)data, (unsigned char*)data + length);
    s.length = length; s.flags = flags; s.port = first; s.second = second; s.connection = connection;
    memcpy(s.block, address, 16);
    sent.push_back(s);
    return 0;
}
extern "C" void analCompress__9CCompressP15BitBufferWriterPQ29CCompress9analStatePUci(void*, void*, void*, unsigned char*, int count) {
    if (compress_calls < 8) compress_counts[compress_calls] = count;
    ++compress_calls;
}
static unsigned char* ring(int local, int frame) { return partners + 17632 * local + 172 + 24 * (frame & 255); }
static void reset() {
    for (auto& b : partners) b = 0;
    sent.clear(); compress_calls = 0;
    netNetworkMode = 1; nmPartners = 3; trueNetFrameNm = 77; sentStatePacketFrameNm = 77;
    broadcastConnection = 0x99;
}
int main() {
    CCompress compression;
    for (auto& w : compression.words) w = 0;
    // Raw path (flag true), two frames 10..11 of partner 2's ring, partner 1 gets the datagram.
    reset();
    unsigned char* r1 = partners + 17632;
    r1[0] = 10; r1[1] = 20; r1[2] = 30; r1[3] = 40; *(int*)(r1 + 4) = 4242; *(int*)(r1 + 17596) = 15;
    partners[17632 * 2 + 17620] = 1;
    for (int f = 10; f <= 11; ++f) for (int i = 0; i < 12; ++i) ring(2, f)[4 + i] = (unsigned char)(f * 16 + i);
    sendControlPacket__FiiiiiR9CCompressb(1, 10, 11, 0xbd42, 2, compression, true);
    if (sent.size() != 1) return 1;
    const Sent& a = sent[0];
    // header word, second word, local byte, first flagged partner (index 2), count 2, 15 - 11 = 4,
    // 24 raw bytes, then the -1 word (sent state equals true frame): 9 + 3 + 24 = 36, aligned to 36, + 4.
    if (a.length != 40) return 2;
    if (a.bytes[0] != 0x42 || a.bytes[1] != 0xbd || a.bytes[4] != 11 || a.bytes[8] != 2) return 3;
    if (a.bytes[9] != 2 || a.bytes[10] != 2 || a.bytes[11] != 4) return 4;
    for (int i = 0; i < 12; ++i) if (a.bytes[12 + i] != 10 * 16 + i || a.bytes[24 + i] != 11 * 16 + i) return 5;
    if (*(int*)&a.bytes[36] != -1) return 6;
    if (a.port != 4242 || a.second != 1000 || a.connection != 0x99 || a.flags != 0) return 7;
    if (a.block[4] != 10 || a.block[7] != 40 || a.block[0] != 0 || a.block[8] != 0) return 8;
    // The sent state word: different frame number gives 0xbd40 and 12 bytes of record(local)+6316+12*77.
    reset(); sentStatePacketFrameNm = 5;
    r1 = partners + 17632; *(int*)(r1 + 17596) = 11;
    unsigned char* table = partners + 17632 * 2 + 6316 + 12 * 77;
    for (int i = 0; i < 12; ++i) table[i] = (unsigned char)(0xa0 + i);
    sendControlPacket__FiiiiiR9CCompressb(1, 10, 10, 1, 2, compression, true);
    const Sent& b = sent[0];
    // 9 + 3 + 12 = 24; word 0xbd40; 12 bytes
    if (b.length != 24 + 4 + 12 || *(int*)&b.bytes[24] != 0xbd40 || b.bytes[28] != 0xa0 || b.bytes[39] != 0xab) return 9;
    if (sentStatePacketFrameNm != 77) return 10;
    // netNetworkMode zero gives index byte 0; no flagged partner gives 255 (-1).
    reset(); netNetworkMode = 0; partners[17632 * 2 + 17620] = 1;
    sendControlPacket__FiiiiiR9CCompressb(0, 5, 5, 1, 0, compression, true);
    if (sent[0].bytes[9] != 0) return 11;
    reset();
    sendControlPacket__FiiiiiR9CCompressb(0, 5, 5, 1, 0, compression, true);
    if (sent[0].bytes[9] != 0xff) return 12;
    // Message bytes: frame 20 carries a 3-byte message from the ring start 8191 (wrapping).
    reset(); sentStatePacketFrameNm = 77;
    ring(1, 20)[20] = 3; *(int*)(ring(1, 20) + 16) = 8191;
    unsigned char* msg = partners + 17632 + 9388;
    msg[8191] = 0x61; msg[0] = 0x62; msg[1] = 0x63;
    sendControlPacket__FiiiiiR9CCompressb(0, 20, 21, 7, 1, compression, true);
    // 9 + 3 + 24 = 36, word -1 (4) at 36..39, then bytes: second - f = 1, length 3, three bytes.
    const Sent& c = sent[0];
    if (c.length != 40 + 5 || c.bytes[40] != 1 || c.bytes[41] != 3 || c.bytes[42] != 0x61 || c.bytes[43] != 0x62 || c.bytes[44] != 0x63) return 13;
    // Encoded path (flag false): one frame, analCompress called four times with one byte each; the
    // fresh compression state gives 16 + 5 + 5 + 1 + 5 + 16 = 48 bits = 6 bytes.
    reset(); sentStatePacketFrameNm = 77; trueNetFrameNm = 77;
    r1 = partners; *(int*)(r1 + 92) = 30;
    sendControlPacket__FiiiiiR9CCompressb(0, 30, 30, 9, 3, compression, false);
    // 9 + 3 = 12, encoded 6 bytes -> 18, aligned to 20, + 4 = 24.
    if (sent[0].length != 24 || compress_calls != 4 || compress_counts[0] != 1) return 14;
    if (sent[0].bytes[9] != (unsigned char)(30 - 77)) return 15;
    // With pending frames (92 < first) the first batch adds a second set of four calls.
    reset(); sentStatePacketFrameNm = 77; trueNetFrameNm = 77;
    *(int*)(partners + 92) = 28;
    sendControlPacket__FiiiiiR9CCompressb(0, 30, 30, 9, 3, compression, false);
    if (compress_calls != 8 || compress_counts[0] != 2 || compress_counts[4] != 1) return 16;
    return 0;
}
